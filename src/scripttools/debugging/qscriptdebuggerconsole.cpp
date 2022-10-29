/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtSCriptTools module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qscriptdebuggerconsole_p.h"
#include "qscriptdebuggerconsolecommandjob_p.h"
#include "qscriptdebuggerconsolecommandmanager_p.h"
#include "qscriptdebuggerscriptedconsolecommand_p.h"
#include "qscriptmessagehandlerinterface_p.h"
#include "qscriptbreakpointdata_p.h"
#include "qscriptdebuggerresponse_p.h"
#include "qscriptdebuggervalueproperty_p.h"
#include "qscriptscriptdata_p.h"
#include "qscripttoolscommon_p.h"
#include "qscripttoolsresources_p.h"

#include <QtCore/qdir.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qdebug.h>
#include <QtScript/qscriptcontextinfo.h>
#include <QtScript/qscriptengine.h>

QT_BEGIN_NAMESPACE

static QScriptValue debuggerResponseToScriptValue(QScriptEngine *eng, const QScriptDebuggerResponse &in)
{
    QScriptValue out = eng->newObject();
    out.setProperty(QString::fromLatin1("result"), eng->toScriptValue(in.result()));
    out.setProperty(QString::fromLatin1("error"), QScriptValue(eng, in.error()));
    out.setProperty(QString::fromLatin1("async"), QScriptValue(eng, in.async()));
    return out;
}

static void debuggerResponseFromScriptValue(const QScriptValue &, QScriptDebuggerResponse &)
{
    Q_ASSERT(false);
}

static QScriptValue breakpointDataToScriptValue(QScriptEngine *eng, const QScriptBreakpointData &in)
{
    QScriptValue out = eng->newObject();
    out.setProperty(QString::fromLatin1("scriptId"), QScriptValue(eng, qsreal(in.scriptId())));
    out.setProperty(QString::fromLatin1("fileName"), QScriptValue(eng, in.fileName()));
    out.setProperty(QString::fromLatin1("lineNumber"), QScriptValue(eng, in.lineNumber()));
    out.setProperty(QString::fromLatin1("enabled"), QScriptValue(eng, in.isEnabled()));
    out.setProperty(QString::fromLatin1("singleShot"), QScriptValue(eng, in.isSingleShot()));
    out.setProperty(QString::fromLatin1("ignoreCount"), QScriptValue(eng, in.ignoreCount()));
    out.setProperty(QString::fromLatin1("condition"), QScriptValue(eng, in.condition()));
    return out;
}

static void breakpointDataFromScriptValue(const QScriptValue &in, QScriptBreakpointData &out)
{
    QScriptValue scriptId = in.property(QString::fromLatin1("scriptId"));
    if (scriptId.isValid())
        out.setScriptId((qint64)scriptId.toNumber());
    out.setFileName(in.property(QString::fromLatin1("fileName")).toString());
    out.setLineNumber(in.property(QString::fromLatin1("lineNumber")).toInt32());
    QScriptValue enabled = in.property(QString::fromLatin1("enabled"));
    if (enabled.isValid())
        out.setEnabled(enabled.toBoolean());
    QScriptValue singleShot = in.property(QString::fromLatin1("singleShot"));
    if (singleShot.isValid())
        out.setSingleShot(singleShot.toBoolean());
    out.setIgnoreCount(in.property(QString::fromLatin1("ignoreCount")).toInt32());
    out.setCondition(in.property(QString::fromLatin1("condition")).toString());
}

static QScriptValue breakpointMapToScriptValue(QScriptEngine *eng, const QScriptBreakpointMap &in)
{
    QScriptValue out = eng->newObject();
    QScriptBreakpointMap::const_iterator it;
    for (it = in.constBegin(); it != in.constEnd(); ++it) {
        out.setProperty(QString::number(it.key()), eng->toScriptValue(it.value()));
    }
    return out;
}

static void breakpointMapFromScriptValue(const QScriptValue &, QScriptBreakpointMap &)
{
    Q_ASSERT(false);
}

static QScriptValue scriptDataToScriptValue(QScriptEngine *eng, const QScriptScriptData &in)
{
    QScriptValue out = eng->newObject();
    out.setProperty(QString::fromLatin1("contents"), QScriptValue(eng, in.contents()));
    out.setProperty(QString::fromLatin1("fileName"), QScriptValue(eng, in.fileName()));
    out.setProperty(QString::fromLatin1("baseLineNumber"), QScriptValue(eng, in.baseLineNumber()));
    return out;
}

static void scriptDataFromScriptValue(const QScriptValue &in, QScriptScriptData &out)
{
    QString contents = in.property(QString::fromLatin1("contents")).toString();
    QString fileName = in.property(QString::fromLatin1("fileName")).toString();
    int baseLineNumber = in.property(QString::fromLatin1("baseLineNumber")).toInt32();
    QScriptScriptData tmp(contents, fileName, baseLineNumber);
    out = tmp;
}

static QScriptValue scriptMapToScriptValue(QScriptEngine *eng, const QScriptScriptMap &in)
{
    QScriptValue out = eng->newObject();
    QScriptScriptMap::const_iterator it;
    for (it = in.constBegin(); it != in.constEnd(); ++it) {
        out.setProperty(QString::number(it.key()), eng->toScriptValue(it.value()));
    }
    return out;
}

static void scriptMapFromScriptValue(const QScriptValue &, QScriptScriptMap &)
{
    Q_ASSERT(false);
}

static QScriptValue consoleCommandToScriptValue(
    QScriptEngine *eng, QScriptDebuggerConsoleCommand* const &in)
{
    if (!in)
        return eng->undefinedValue();
    QScriptValue out = eng->newObject();
    out.setProperty(QString::fromLatin1("name"), QScriptValue(eng, in->name()));
    out.setProperty(QString::fromLatin1("group"), QScriptValue(eng, in->group()));
    out.setProperty(QString::fromLatin1("shortDescription"), QScriptValue(eng, in->shortDescription()));
    out.setProperty(QString::fromLatin1("longDescription"), QScriptValue(eng, in->longDescription()));
    out.setProperty(QString::fromLatin1("aliases"), eng->toScriptValue(in->aliases()));
    out.setProperty(QString::fromLatin1("seeAlso"), eng->toScriptValue(in->seeAlso()));
    return out;
}

static void consoleCommandFromScriptValue(
    const QScriptValue &, QScriptDebuggerConsoleCommand* &)
{
    Q_ASSERT(false);
}

static QScriptValue consoleCommandGroupDataToScriptValue(
    QScriptEngine *eng, const QScriptDebuggerConsoleCommandGroupData &in)
{
    QScriptValue out = eng->newObject();
    out.setProperty(QString::fromLatin1("longDescription"), QScriptValue(eng, in.longDescription()));
    out.setProperty(QString::fromLatin1("shortDescription"), QScriptValue(eng, in.shortDescription()));
    return out;
}

static void consoleCommandGroupDataFromScriptValue(
    const QScriptValue &, QScriptDebuggerConsoleCommandGroupData &)
{
    Q_ASSERT(false);
}

static QScriptValue consoleCommandGroupMapToScriptValue(
    QScriptEngine *eng, const QScriptDebuggerConsoleCommandGroupMap &in)
{
    QScriptValue out = eng->newObject();
    QScriptDebuggerConsoleCommandGroupMap::const_iterator it;
    for (it = in.constBegin(); it != in.constEnd(); ++it) {
        out.setProperty(it.key(), eng->toScriptValue(it.value()));
    }
    return out;
}

static void consoleCommandGroupMapFromScriptValue(
    const QScriptValue &, QScriptDebuggerConsoleCommandGroupMap &)
{
    Q_ASSERT(false);
}

static QScriptValue contextInfoToScriptValue(QScriptEngine *eng, const QScriptContextInfo &in)
{
    QScriptValue out = eng->newObject();
    out.setProperty(QString::fromLatin1("scriptId"), QScriptValue(eng, qsreal(in.scriptId())));
    out.setProperty(QString::fromLatin1("fileName"), QScriptValue(eng, in.fileName()));
    out.setProperty(QString::fromLatin1("lineNumber"), QScriptValue(eng, in.lineNumber()));
    out.setProperty(QString::fromLatin1("functionName"), QScriptValue(eng, in.functionName()));
    return out;
}

static void contextInfoFromScriptValue(const QScriptValue &, QScriptContextInfo &)
{
    Q_ASSERT(false);
}

static QScriptValue debuggerScriptValuePropertyToScriptValue(QScriptEngine *eng, const QScriptDebuggerValueProperty &in)
{
    QScriptValue out = eng->newObject();
    out.setProperty(QString::fromLatin1("name"), QScriptValue(eng, in.name()));
    out.setProperty(QString::fromLatin1("value"), eng->toScriptValue(in.value()));
    out.setProperty(QString::fromLatin1("valueAsString"), QScriptValue(eng, in.valueAsString()));
    out.setProperty(QString::fromLatin1("flags"), QScriptValue(eng, static_cast<int>(in.flags())));
    return out;
}

static void debuggerScriptValuePropertyFromScriptValue(const QScriptValue &in, QScriptDebuggerValueProperty &out)
{
    QString name = in.property(QString::fromLatin1("name")).toString();
    QScriptDebuggerValue value = qscriptvalue_cast<QScriptDebuggerValue>(in.property(QString::fromLatin1("value")));
    QString valueAsString = in.property(QString::fromLatin1("valueAsString")).toString();
    int flags = in.property(QString::fromLatin1("flags")).toInt32();
    QScriptDebuggerValueProperty tmp(name, value, valueAsString, QScriptValue::PropertyFlags(flags));
    out = tmp;
}

/*!
  \since 4.5
  \class QScriptDebuggerConsole
  \internal

  \brief The QScriptDebuggerConsole class provides the core functionality of a debugger console.
*/

class QScriptDebuggerConsolePrivate
{
    Q_DECLARE_PUBLIC(QScriptDebuggerConsole)
public:
    QScriptDebuggerConsolePrivate(QScriptDebuggerConsole*);
    ~QScriptDebuggerConsolePrivate();

    void loadScriptedCommands(QScriptMessageHandlerInterface *messageHandler);
    QScriptDebuggerConsoleCommandJob *createJob(
        const QString &command,
        QScriptMessageHandlerInterface *messageHandler,
        QScriptDebuggerCommandSchedulerInterface *commandScheduler);

    QScriptEngine *commandEngine;
    QScriptDebuggerConsoleCommandManager *commandManager;
    QString commandPrefix;
    QString input;
    QStringList commandHistory;
    int currentFrameIndex;
    qint64 currentScriptId;
    int currentLineNumber;
    int evaluateAction;
    qint64 sessionId;

    QScriptDebuggerConsole *q_ptr;
};

QScriptDebuggerConsolePrivate::QScriptDebuggerConsolePrivate(QScriptDebuggerConsole* parent)
    : q_ptr(parent)
{
    sessionId = 0;
    currentFrameIndex = 0;
    currentScriptId = -1;
    currentLineNumber = -1;
    evaluateAction = 0;
    commandPrefix = QLatin1String(".");
    commandManager = new QScriptDebuggerConsoleCommandManager();

    commandEngine = new QScriptEngine;
    qScriptRegisterMetaType<QScriptBreakpointData>(commandEngine, breakpointDataToScriptValue, breakpointDataFromScriptValue);
    qScriptRegisterMetaType<QScriptBreakpointMap>(commandEngine, breakpointMapToScriptValue, breakpointMapFromScriptValue);
    qScriptRegisterMetaType<QScriptScriptData>(commandEngine, scriptDataToScriptValue, scriptDataFromScriptValue);
    qScriptRegisterMetaType<QScriptScriptMap>(commandEngine, scriptMapToScriptValue, scriptMapFromScriptValue);
    qScriptRegisterMetaType<QScriptContextInfo>(commandEngine, contextInfoToScriptValue, contextInfoFromScriptValue);
    qScriptRegisterMetaType<QScriptDebuggerValueProperty>(commandEngine, debuggerScriptValuePropertyToScriptValue, debuggerScriptValuePropertyFromScriptValue);
    qScriptRegisterSequenceMetaType<QScriptDebuggerValuePropertyList>(commandEngine);
    qScriptRegisterMetaType<QScriptDebuggerResponse>(commandEngine, debuggerResponseToScriptValue, debuggerResponseFromScriptValue);
    qScriptRegisterMetaType<QScriptDebuggerConsoleCommand*>(commandEngine, consoleCommandToScriptValue, consoleCommandFromScriptValue);
    qScriptRegisterSequenceMetaType<QScriptDebuggerConsoleCommandList>(commandEngine);
    qScriptRegisterMetaType<QScriptDebuggerConsoleCommandGroupData>(commandEngine, consoleCommandGroupDataToScriptValue, consoleCommandGroupDataFromScriptValue);
    qScriptRegisterMetaType<QScriptDebuggerConsoleCommandGroupMap>(commandEngine, consoleCommandGroupMapToScriptValue, consoleCommandGroupMapFromScriptValue);
// ### can't do this, if it's an object ID the conversion will be incorrect since
// ### the object ID refers to an object in a different engine!
//    qScriptRegisterMetaType(commandEngine, debuggerScriptValueToScriptValue, debuggerScriptValueFromScriptValue);
}

QScriptDebuggerConsolePrivate::~QScriptDebuggerConsolePrivate()
{
    delete commandManager;
    delete commandEngine;
}

/*!
  Loads command definitions from scripts.
*/
void QScriptDebuggerConsolePrivate::loadScriptedCommands(
    QScriptMessageHandlerInterface *messageHandler)
{
    static const struct ScriptsTblData {
        const unsigned char* script;
        const unsigned int scriptlen;
        const char* scriptname;
    } ScriptsTbl[] = {
        { scripts_advance_qs, scripts_advance_qs_len, "advance.qs" },
        { scripts_backtrace_qs, scripts_backtrace_qs_len, "backtrace.qs" },
        { scripts_break_qs, scripts_break_qs_len, "break.qs" },
        { scripts_clear_qs, scripts_clear_qs_len, "clear.qs" },
        { scripts_complete_qs, scripts_complete_qs_len, "complete.qs" },
        { scripts_condition_qs, scripts_condition_qs_len, "condition.qs" },
        { scripts_continue_qs, scripts_continue_qs_len, "continue.qs" },
        { scripts_delete_qs, scripts_delete_qs_len, "delete.qs" },
        { scripts_disable_qs, scripts_disable_qs_len, "disable.qs" },
        { scripts_down_qs, scripts_down_qs_len, "down.qs" },
        { scripts_enable_qs, scripts_enable_qs_len, "enable.qs" },
        { scripts_eval_qs, scripts_eval_qs_len, "eval.qs" },
        { scripts_finish_qs, scripts_finish_qs_len, "finish.qs" },
        { scripts_frame_qs, scripts_frame_qs_len, "frame.qs" },
        { scripts_help_qs, scripts_help_qs_len, "help.qs" },
        { scripts_ignore_qs, scripts_ignore_qs_len, "ignore.qs" },
        { scripts_info_qs, scripts_info_qs_len, "info.qs" },
        { scripts_interrupt_qs, scripts_interrupt_qs_len, "interrupt.qs" },
        { scripts_list_qs, scripts_list_qs_len, "list.qs" },
        { scripts_next_qs, scripts_next_qs_len, "next.qs" },
        { scripts_print_qs, scripts_print_qs_len, "print.qs" },
        { scripts_return_qs, scripts_return_qs_len, "return.qs" },
        { scripts_step_qs, scripts_step_qs_len, "step.qs" },
        { scripts_tbreak_qs, scripts_tbreak_qs_len, "tbreak.qs" },
        { scripts_up_qs, scripts_up_qs_len, "up.qs" }
    };
    static const qint16 ScriptsTblSize = sizeof(ScriptsTbl) / sizeof(ScriptsTblData);

    for (int i = 0; i < ScriptsTblSize; ++i) {

        QString program = QString::fromLocal8Bit(
            reinterpret_cast<const char*>(ScriptsTbl[i].script),
            ScriptsTbl[i].scriptlen
        );
        QString fileName = QString::fromLocal8Bit(ScriptsTbl[i].scriptname);
        QScriptDebuggerScriptedConsoleCommand *command;
        command = QScriptDebuggerScriptedConsoleCommand::parse(
            program, fileName, commandEngine, messageHandler);
        if (!command)
            continue;
        commandManager->addCommand(command);
    }
}


/*!
  Creates a job that will execute the given debugger \a command.
  Returns the new job, or 0 if the command is undefined.
*/
QScriptDebuggerConsoleCommandJob *QScriptDebuggerConsolePrivate::createJob(
    const QString &command, QScriptMessageHandlerInterface *messageHandler,
    QScriptDebuggerCommandSchedulerInterface *commandScheduler)
{
    QString name;
    int i = command.indexOf(QLatin1Char(' '));
    if (i == -1) {
        name = command;
        i = name.size();
    } else {
        name = command.left(i);
    }
    if (name.isEmpty())
        return 0;
    QScriptDebuggerConsoleCommand *cmd = commandManager->findCommand(name);
    if (!cmd) {
        // try to auto-complete
        QStringList completions = commandManager->completions(name);
        if (!completions.isEmpty()) {
            if (completions.size() > 1) {
                QString msg;
                msg.append(QString::fromLatin1("Ambiguous command \"%0\": ")
                           .arg(name));
                for (int j = 0; j < completions.size(); ++j) {
                    if (j > 0)
                        msg.append(QLatin1String(", "));
                    msg.append(completions.at(j));
                }
                msg.append(QLatin1Char('.'));
                messageHandler->message(QtWarningMsg, msg);
                return 0;
            }
            cmd = commandManager->findCommand(completions.at(0));
            Q_ASSERT(cmd != 0);
        }
        if (!cmd) {
            messageHandler->message(
                QtWarningMsg,
                QString::fromLatin1("Undefined command \"%0\". Try \"help\".")
                .arg(name));
            return 0;
        }
    }
    QStringList args;
    QString tmp = command.mid(i+1);
    if (cmd->argumentTypes().contains(QString::fromLatin1("script"))) {
        if (!tmp.isEmpty())
            args.append(tmp);
    } else {
        args = tmp.split(QLatin1Char(' '), QString::SkipEmptyParts);
    }
    return cmd->createJob(args, q_func(), messageHandler, commandScheduler);
}

QScriptDebuggerConsole::QScriptDebuggerConsole()
    : d_ptr(new QScriptDebuggerConsolePrivate(this))
{
}

QScriptDebuggerConsole::~QScriptDebuggerConsole()
{
}

void QScriptDebuggerConsole::loadScriptedCommands(QScriptMessageHandlerInterface *messageHandler)
{
    Q_D(QScriptDebuggerConsole);
    d->loadScriptedCommands(messageHandler);
}

QScriptDebuggerConsoleCommandManager *QScriptDebuggerConsole::commandManager() const
{
    Q_D(const QScriptDebuggerConsole);
    return d->commandManager;
}

bool QScriptDebuggerConsole::hasIncompleteInput() const
{
    Q_D(const QScriptDebuggerConsole);
    return !d->input.isEmpty();
}

QString QScriptDebuggerConsole::incompleteInput() const
{
    Q_D(const QScriptDebuggerConsole);
    return d->input;
}

void QScriptDebuggerConsole::setIncompleteInput(const QString &input)
{
    Q_D(QScriptDebuggerConsole);
    d->input = input;
}

QString QScriptDebuggerConsole::commandPrefix() const
{
    Q_D(const QScriptDebuggerConsole);
    return d->commandPrefix;
}

/*!
  Consumes the given line of \a input.  If the input starts with the
  command prefix, it is regarded as a debugger command; otherwise the
  input is evaluated as a plain script.
*/
QScriptDebuggerConsoleCommandJob *QScriptDebuggerConsole::consumeInput(
    const QString &input, QScriptMessageHandlerInterface *messageHandler,
    QScriptDebuggerCommandSchedulerInterface *commandScheduler)
{
    Q_D(QScriptDebuggerConsole);
    static const int maximumHistoryCount = 100;
    QString cmd;
    if (d->input.isEmpty() && input.isEmpty()) {
        if (d->commandHistory.isEmpty())
            return 0;
        cmd = d->commandHistory.first();
    } else {
        cmd = input;
    }
    if (d->input.isEmpty() && cmd.startsWith(d->commandPrefix)) {
        if (!input.isEmpty()) {
            d->commandHistory.prepend(cmd);
            if (d->commandHistory.size() > maximumHistoryCount)
                d->commandHistory.removeLast();
        }
        cmd.remove(0, d->commandPrefix.length());
        return d->createJob(cmd, messageHandler, commandScheduler);
    }
    d->input += cmd;
    d->input += QLatin1Char('\n');
    QScriptSyntaxCheckResult check = QScriptEngine::checkSyntax(d->input);
    if (check.state() == QScriptSyntaxCheckResult::Intermediate)
        return 0;
    d->input.chop(1); // remove the last \n
    cmd = QString();
    cmd.append(d->commandPrefix);
    cmd.append(QString::fromLatin1("eval "));
    cmd.append(d->input);
    d->commandHistory.prepend(cmd);
    if (d->commandHistory.size() > maximumHistoryCount)
        d->commandHistory.removeLast();
    d->input.clear();
    cmd.remove(0, d->commandPrefix.length());
    return d->createJob(cmd, messageHandler, commandScheduler);
}

int QScriptDebuggerConsole::currentFrameIndex() const
{
    Q_D(const QScriptDebuggerConsole);
    return d->currentFrameIndex;
}

void QScriptDebuggerConsole::setCurrentFrameIndex(int index)
{
    Q_D(QScriptDebuggerConsole);
    d->currentFrameIndex = index;
}

qint64 QScriptDebuggerConsole::currentScriptId() const
{
    Q_D(const QScriptDebuggerConsole);
    return d->currentScriptId;
}

void QScriptDebuggerConsole::setCurrentScriptId(qint64 id)
{
    Q_D(QScriptDebuggerConsole);
    d->currentScriptId = id;
}

int QScriptDebuggerConsole::currentLineNumber() const
{
    Q_D(const QScriptDebuggerConsole);
    return d->currentLineNumber;
}

void QScriptDebuggerConsole::setCurrentLineNumber(int lineNumber)
{
    Q_D(QScriptDebuggerConsole);
    d->currentLineNumber = lineNumber;
}

int QScriptDebuggerConsole::evaluateAction() const
{
    Q_D(const QScriptDebuggerConsole);
    return d->evaluateAction;
}

void QScriptDebuggerConsole::setEvaluateAction(int action)
{
    Q_D(QScriptDebuggerConsole);
    d->evaluateAction = action;
}

qint64 QScriptDebuggerConsole::sessionId() const
{
    Q_D(const QScriptDebuggerConsole);
    return d->sessionId;
}

void QScriptDebuggerConsole::bumpSessionId()
{
    Q_D(QScriptDebuggerConsole);
    ++d->sessionId;
}

void QScriptDebuggerConsole::showDebuggerInfoMessage(
    QScriptMessageHandlerInterface *messageHandler)
{
    messageHandler->message(
        QtDebugMsg,
        QString::fromLatin1(
            "Welcome to the Qt Script debugger.\n"
            "Debugger commands start with a . (period).\n"
            "Any other input will be evaluated by the script interpreter.\n"
            "Type \".help\" for help.\n"));
}

/*!
  \reimp
*/
int QScriptDebuggerConsole::historyCount() const
{
    Q_D(const QScriptDebuggerConsole);
    return d->commandHistory.size();
}

/*!
  \reimp
*/
QString QScriptDebuggerConsole::historyAt(int index) const
{
    Q_D(const QScriptDebuggerConsole);
    return d->commandHistory.value(index);
}

/*!
  \reimp
*/
void QScriptDebuggerConsole::changeHistoryAt(int index, const QString &newHistory)
{
    Q_D(QScriptDebuggerConsole);
    d->commandHistory[index] = newHistory;
}

QT_END_NAMESPACE

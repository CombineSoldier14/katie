/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#include "qdeclarativeinclude_p.h"

#include <QtScript/qscriptengine.h>
#include <QtCore/qfile.h>

#include "qdeclarativeengine_p.h"
#include "qdeclarativeglobalscriptclass_p.h"

QT_BEGIN_NAMESPACE

QDeclarativeInclude::QDeclarativeInclude(const QUrl &url, 
                                         QDeclarativeEngine *engine, 
                                         QScriptContext *ctxt)
    : QObject(engine), m_engine(engine), m_url(url)
{
    QDeclarativeEnginePrivate *ep = QDeclarativeEnginePrivate::get(engine);
    m_context = ep->contextClass->contextFromValue(QScriptDeclarativeClass::scopeChainValue(ctxt, -3));

    m_scope[0] = QScriptDeclarativeClass::scopeChainValue(ctxt, -4);
    m_scope[1] = QScriptDeclarativeClass::scopeChainValue(ctxt, -5);

    m_scriptEngine = QDeclarativeEnginePrivate::getScriptEngine(engine);

    m_result = resultValue(m_scriptEngine);

    include(ctxt, m_scriptEngine);
}

QDeclarativeInclude::~QDeclarativeInclude()
{
}

QScriptValue QDeclarativeInclude::resultValue(QScriptEngine *engine, Status status)
{
    QScriptValue result = engine->newObject();
    result.setProperty(QLatin1String("OK"), QScriptValue(engine, Ok));
    result.setProperty(QLatin1String("LOADING"), QScriptValue(engine, Loading));
    result.setProperty(QLatin1String("NETWORK_ERROR"), QScriptValue(engine, NetworkError));
    result.setProperty(QLatin1String("EXCEPTION"), QScriptValue(engine, Exception));

    result.setProperty(QLatin1String("status"), QScriptValue(engine, status));
    return result;
}

QScriptValue QDeclarativeInclude::result() const
{
    return m_result;
}

void QDeclarativeInclude::setCallback(const QScriptValue &c)
{
    m_callback = c;
}

QScriptValue QDeclarativeInclude::callback() const
{
    return m_callback;
}

void QDeclarativeInclude::callback(QScriptEngine *engine, QScriptValue &callback, QScriptValue &status)
{
    if (callback.isValid()) {
        QScriptValue args = engine->newArray(1);
        args.setProperty(0, status);
        callback.call(QScriptValue(), args);
    }
}

/*
    Documented in qdeclarativeengine.cpp
*/
QScriptValue QDeclarativeInclude::include(QScriptContext *ctxt, QScriptEngine *engine)
{
    if (ctxt->argumentCount() == 0)
        return engine->undefinedValue();

    QDeclarativeEnginePrivate *ep = QDeclarativeEnginePrivate::get(engine);

    QUrl contextUrl = ep->contextClass->urlFromValue(QScriptDeclarativeClass::scopeChainValue(ctxt, -3));
    if (contextUrl.isEmpty()) 
        return ctxt->throwError(QLatin1String("Qt.include(): Can only be called from JavaScript files"));

    QString urlString = ctxt->argument(0).toString();
    QUrl url(urlString);
    if (url.isRelative()) {
        url = QUrl(contextUrl).resolved(url);
        urlString = url.toString();
    }

    QString localFile = QDeclarativeEnginePrivate::urlToLocalFile(url);

    QScriptValue func = ctxt->argument(1);
    if (!func.isFunction())
        func = QScriptValue();

    QScriptValue result;
    if (localFile.isEmpty()) {
        QDeclarativeInclude *i = 
            new QDeclarativeInclude(url, QDeclarativeEnginePrivate::getEngine(engine), ctxt);

        if (func.isValid())
            i->setCallback(func);

        result = i->result();
    } else {

        QFile f(localFile);
        if (f.open(QIODevice::ReadOnly)) {
            QByteArray data = f.readAll();
            QString code = QString::fromUtf8(data);

            QDeclarativeContextData *context = 
                ep->contextClass->contextFromValue(QScriptDeclarativeClass::scopeChainValue(ctxt, -3));

            QScriptContext *scriptContext = QScriptDeclarativeClass::pushCleanContext(engine);
            scriptContext->pushScope(ep->contextClass->newUrlContext(context, 0, urlString));
            scriptContext->pushScope(ep->globalClass->staticGlobalObject());
            QScriptValue scope = QScriptDeclarativeClass::scopeChainValue(ctxt, -5);
            scriptContext->pushScope(scope);
            scriptContext->setActivationObject(scope);
            QDeclarativeScriptParser::extractPragmas(code);

            engine->evaluate(code, urlString, 1);

            engine->popContext();

            if (engine->hasUncaughtException()) {
                result = resultValue(engine, Exception);
                result.setProperty(QLatin1String("exception"), engine->uncaughtException());
                engine->clearExceptions();
            } else {
                result = resultValue(engine, Ok);
            }
            callback(engine, func, result);
        } else {
            result = resultValue(engine, NetworkError);
            callback(engine, func, result);
        }
    }

    return result;
}

QScriptValue QDeclarativeInclude::worker_include(QScriptContext *ctxt, QScriptEngine *engine)
{
    if (ctxt->argumentCount() == 0)
        return engine->undefinedValue();

    QString urlString = ctxt->argument(0).toString();
    QUrl url(ctxt->argument(0).toString());
    if (url.isRelative()) {
        QString contextUrl = QScriptDeclarativeClass::scopeChainValue(ctxt, -3).data().toString();
        Q_ASSERT(!contextUrl.isEmpty());

        url = QUrl(contextUrl).resolved(url);
        urlString = url.toString();
    }

    QString localFile = QDeclarativeEnginePrivate::urlToLocalFile(url);

    QScriptValue func = ctxt->argument(1);
    if (!func.isFunction())
        func = QScriptValue();

    QScriptValue result;
    if (!localFile.isEmpty()) {

        QFile f(localFile);
        if (f.open(QIODevice::ReadOnly)) {
            QByteArray data = f.readAll();
            QString code = QString::fromUtf8(data);

            QScriptContext *scriptContext = QScriptDeclarativeClass::pushCleanContext(engine);
            QScriptValue urlContext = engine->newObject();
            urlContext.setData(QScriptValue(engine, urlString));
            scriptContext->pushScope(urlContext);

            QScriptValue scope = QScriptDeclarativeClass::scopeChainValue(ctxt, -4);
            scriptContext->pushScope(scope);
            scriptContext->setActivationObject(scope);
            QDeclarativeScriptParser::extractPragmas(code);

            engine->evaluate(code, urlString, 1);

            engine->popContext();

            if (engine->hasUncaughtException()) {
                result = resultValue(engine, Exception);
                result.setProperty(QLatin1String("exception"), engine->uncaughtException());
                engine->clearExceptions();
            } else {
                result = resultValue(engine, Ok);
            }
            callback(engine, func, result);
        } else {
            result = resultValue(engine, NetworkError);
            callback(engine, func, result);
        }
    }

    return result;
}

QT_END_NAMESPACE
#include "moc_qdeclarativeinclude_p.h"

/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qscriptbreakpointswidget_p.h"
#include "qscriptbreakpointswidgetinterface_p_p.h"
#include "qscriptbreakpointsmodel_p.h"
#include "qscriptdebuggerscriptsmodel_p.h"

#include <QtCore/qdebug.h>
#include <QtGui/qaction.h>
#include <QtGui/qcompleter.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qtoolbar.h>
#include <QtGui/qtoolbutton.h>
#include <QtGui/qtreeview.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qstyleditemdelegate.h>
#include <QtGui/qevent.h>
#include <QtScript/qscriptengine.h>

QT_BEGIN_NAMESPACE

class QScriptNewBreakpointWidget : public QWidget
{
    Q_OBJECT
public:
    QScriptNewBreakpointWidget(QWidget *parent = Q_NULLPTR)
        : QWidget(parent) {
        QHBoxLayout *hboxLayout = new QHBoxLayout(this);
        hboxLayout->setSpacing(6);
        hboxLayout->setMargin(0);

        toolClose = new QToolButton(this);
        toolClose->setIcon(QIcon(QLatin1String(":/qt/scripttools/debugging/images/win/closetab.png")));
        toolClose->setAutoRaise(true);
        toolClose->setText(tr("Close"));
        hboxLayout->addWidget(toolClose);

        fileNameEdit = new QLineEdit();
        setFocusProxy(fileNameEdit);
        QRegExp locationRegExp(QLatin1String(".+:[0-9]+"));
        QRegExpValidator *validator = new QRegExpValidator(locationRegExp, fileNameEdit);
        fileNameEdit->setValidator(validator);
        hboxLayout->addWidget(fileNameEdit);

        toolOk = new QToolButton(this);
        toolOk->setIcon(QIcon(QLatin1String(":/qt/scripttools/debugging/images/win/plus.png")));
        toolOk->setAutoRaise(true);
        toolOk->setEnabled(false);
        hboxLayout->addWidget(toolOk);

        QObject::connect(toolClose, SIGNAL(clicked()), this, SLOT(hide()));
        QObject::connect(toolOk, SIGNAL(clicked()), this, SLOT(onOkClicked()));
        QObject::connect(fileNameEdit, SIGNAL(textChanged(QString)),
                         this, SLOT(onTextChanged()));
        QObject::connect(fileNameEdit, SIGNAL(returnPressed()),
                         this, SLOT(onOkClicked()));
    }

    void setCompleter(QCompleter *comp)
        { fileNameEdit->setCompleter(comp); }

Q_SIGNALS:
    void newBreakpointRequest(const QString &fileName, int lineNumber);

protected:
    void keyPressEvent(QKeyEvent *e)
    {
        if (e->key() == Qt::Key_Escape)
            hide();
        else
            QWidget::keyPressEvent(e);
    }

private Q_SLOTS:
    void onOkClicked()
    {
        QString location = fileNameEdit->text();
        fileNameEdit->clear();
        QString fileName = location.left(location.lastIndexOf(QLatin1Char(':')));
        int lineNumber = location.mid(fileName.length()+1).toInt();
        emit newBreakpointRequest(fileName, lineNumber);
    }

    void onTextChanged()
    {
        toolOk->setEnabled(fileNameEdit->hasAcceptableInput());
    }

private:
    QLineEdit *fileNameEdit;
    QToolButton *toolClose;
    QToolButton *toolOk;
};



class QScriptBreakpointsWidgetPrivate
    : public QScriptBreakpointsWidgetInterfacePrivate
{
    Q_DECLARE_PUBLIC(QScriptBreakpointsWidget)
public:
    QScriptBreakpointsWidgetPrivate();
    ~QScriptBreakpointsWidgetPrivate();

    void _q_newBreakpoint();
    void _q_deleteBreakpoint();
    void _q_onCurrentChanged(const QModelIndex &index);
    void _q_onNewBreakpointRequest(const QString &fileName, int lineNumber);

    static QPixmap pixmap(const QString &path)
    {
        static QString prefix = QLatin1String(":/qt/scripttools/debugging/images/");
        return QPixmap(prefix + path);
    }

    QTreeView *view;
    QScriptNewBreakpointWidget *newBreakpointWidget;
    QAction *deleteBreakpointAction;
    QScriptDebuggerScriptsModel *scriptsModel;
};

QScriptBreakpointsWidgetPrivate::QScriptBreakpointsWidgetPrivate()
{
}

QScriptBreakpointsWidgetPrivate::~QScriptBreakpointsWidgetPrivate()
{
}

void QScriptBreakpointsWidgetPrivate::_q_newBreakpoint()
{
    newBreakpointWidget->show();
    newBreakpointWidget->setFocus(Qt::OtherFocusReason);
}

void QScriptBreakpointsWidgetPrivate::_q_deleteBreakpoint()
{
    Q_Q(QScriptBreakpointsWidget);
    QModelIndex index = view->currentIndex();
    if (index.isValid()) {
        int id = q->breakpointsModel()->breakpointIdAt(index.row());
        q->breakpointsModel()->deleteBreakpoint(id);
    }
}

void QScriptBreakpointsWidgetPrivate::_q_onCurrentChanged(const QModelIndex &index)
{
    deleteBreakpointAction->setEnabled(index.isValid());
}

void QScriptBreakpointsWidgetPrivate::_q_onNewBreakpointRequest(const QString &fileName, int lineNumber)
{
    QScriptBreakpointData data(fileName, lineNumber);
    q_func()->breakpointsModel()->setBreakpoint(data);
}

class QScriptBreakpointsItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    QScriptBreakpointsItemDelegate(QObject *parent = Q_NULLPTR)
        : QStyledItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
        QWidget *editor = QStyledItemDelegate::createEditor(parent, option, index);
        if (index.column() == 2) {
            // condition
            QLineEdit *le = qobject_cast<QLineEdit*>(editor);
            if (le) {
                QObject::connect(le, SIGNAL(textEdited(QString)),
                                 this, SLOT(validateInput(QString)));
            }
        }
        return editor;
    }

    bool eventFilter(QObject *editor, QEvent *event)
    {
        if (QLineEdit *le = qobject_cast<QLineEdit*>(editor)) {
            if (event->type() == QEvent::KeyPress) {
                int key = static_cast<QKeyEvent*>(event)->key();
                if ((key == Qt::Key_Enter) || (key == Qt::Key_Return)) {
                    if (QScriptEngine::checkSyntax(le->text()).state() != QScriptSyntaxCheckResult::Valid) {
                        // ignore when script contains syntax error
                        return true;
                    }
                }
            }
        }
        return QStyledItemDelegate::eventFilter(editor, event);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        if (index.column() == 2) {
            // check that the syntax is OK
            QString condition = qobject_cast<QLineEdit*>(editor)->text();
            if (QScriptEngine::checkSyntax(condition).state() != QScriptSyntaxCheckResult::Valid)
                return;
        }
        QStyledItemDelegate::setModelData(editor, model, index);
    }

private Q_SLOTS:
    void validateInput(const QString &text)
    {
        QWidget *editor = qobject_cast<QWidget*>(sender());
        QPalette pal = editor->palette();
        QColor col;
        bool ok = (QScriptEngine::checkSyntax(text).state() == QScriptSyntaxCheckResult::Valid);
        if (ok) {
            col = Qt::white;
        } else {
            QScriptSyntaxCheckResult result = QScriptEngine::checkSyntax(
                text + QLatin1Char('\n'));
            if (result.state() == QScriptSyntaxCheckResult::Intermediate)
                col = QColor(255, 240, 192);
            else
                col = QColor(255, 102, 102);
        }
        pal.setColor(QPalette::Active, QPalette::Base, col);
        editor->setPalette(pal);
    }
};

QScriptBreakpointsWidget::QScriptBreakpointsWidget(QWidget *parent)
    : QScriptBreakpointsWidgetInterface(*new QScriptBreakpointsWidgetPrivate, parent, 0)
{
    Q_D(QScriptBreakpointsWidget);
    d->view = new QTreeView();
//    d->view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    d->view->setEditTriggers(QAbstractItemView::AllEditTriggers);
//    d->view->setAlternatingRowColors(true);
    d->view->setRootIsDecorated(false);
    d->view->setSelectionBehavior(QAbstractItemView::SelectRows);
//    d->view->header()->hide();
//    d->view->header()->setDefaultAlignment(Qt::AlignLeft);
//    d->view->header()->setResizeMode(QHeaderView::ResizeToContents);
    d->view->setItemDelegate(new QScriptBreakpointsItemDelegate(this));

    d->newBreakpointWidget = new QScriptNewBreakpointWidget();
    d->newBreakpointWidget->hide();
    QObject::connect(d->newBreakpointWidget, SIGNAL(newBreakpointRequest(QString,int)),
                     this, SLOT(_q_onNewBreakpointRequest(QString,int)));

    QIcon newBreakpointIcon;
    newBreakpointIcon.addPixmap(d->pixmap(QLatin1String("new.png")), QIcon::Normal);
    QAction *newBreakpointAction = new QAction(newBreakpointIcon, tr("New"), this);
    QObject::connect(newBreakpointAction, SIGNAL(triggered()),
                     this, SLOT(_q_newBreakpoint()));

    QIcon deleteBreakpointIcon;
    deleteBreakpointIcon.addPixmap(d->pixmap(QLatin1String("delete.png")), QIcon::Normal);
    d->deleteBreakpointAction = new QAction(deleteBreakpointIcon, tr("Delete"), this);
    d->deleteBreakpointAction->setEnabled(false);
    QObject::connect(d->deleteBreakpointAction, SIGNAL(triggered()),
                     this, SLOT(_q_deleteBreakpoint()));

#ifndef QT_NO_TOOLBAR
    QToolBar *toolBar = new QToolBar();
    toolBar->addAction(newBreakpointAction);
    toolBar->addAction(d->deleteBreakpointAction);
#endif

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setMargin(0);
#ifndef QT_NO_TOOLBAR
    vbox->addWidget(toolBar);
#endif
    vbox->addWidget(d->newBreakpointWidget);
    vbox->addWidget(d->view);
}

QScriptBreakpointsWidget::~QScriptBreakpointsWidget()
{
}

/*!
  \reimp
*/
QScriptBreakpointsModel *QScriptBreakpointsWidget::breakpointsModel() const
{
    Q_D(const QScriptBreakpointsWidget);
    return qobject_cast<QScriptBreakpointsModel*>(d->view->model());
}

/*!
  \reimp
*/
void QScriptBreakpointsWidget::setBreakpointsModel(QScriptBreakpointsModel *model)
{
    Q_D(QScriptBreakpointsWidget);
    d->view->setModel(model);
    d->view->header()->resizeSection(0, 50);
    QObject::connect(d->view->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
                     this, SLOT(_q_onCurrentChanged(QModelIndex)));
}

/*!
  \reimp
*/
QScriptDebuggerScriptsModel *QScriptBreakpointsWidget::scriptsModel() const
{
    Q_D(const QScriptBreakpointsWidget);
    return d->scriptsModel;
}

/*!
  \reimp
*/
void QScriptBreakpointsWidget::setScriptsModel(QScriptDebuggerScriptsModel *model)
{
    Q_D(QScriptBreakpointsWidget);
    d->scriptsModel = model;
    QCompleter *completer = new QCompleter(model, this);
    completer->setCompletionRole(Qt::DisplayRole);
    d->newBreakpointWidget->setCompleter(completer);
}

/*!
  \reimp
*/
void QScriptBreakpointsWidget::keyPressEvent(QKeyEvent *e)
{
    Q_D(QScriptBreakpointsWidget);
    if (e->key() == Qt::Key_Delete) {
        QModelIndex index = d->view->currentIndex();
        if (!index.isValid())
            return;
        int id = breakpointsModel()->breakpointIdAt(index.row());
        breakpointsModel()->deleteBreakpoint(id);
    }
}

QT_END_NAMESPACE

#include "moc_qscriptbreakpointswidget.cpp"
#include "moc_qscriptbreakpointswidget_p.h"

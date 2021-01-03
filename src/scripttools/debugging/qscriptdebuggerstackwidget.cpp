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

#include "qscriptdebuggerstackwidget_p.h"
#include "qscriptdebuggerstackwidgetinterface_p_p.h"

#include <QtCore/qdebug.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qtreeview.h>
#include <QtGui/qboxlayout.h>

QT_BEGIN_NAMESPACE

class QScriptDebuggerStackWidgetPrivate
    : public QScriptDebuggerStackWidgetInterfacePrivate
{
    Q_DECLARE_PUBLIC(QScriptDebuggerStackWidget)
public:
    QScriptDebuggerStackWidgetPrivate();
    ~QScriptDebuggerStackWidgetPrivate();

    // private slots
    void _q_onCurrentChanged(const QModelIndex &index);

    QTreeView *view;
};

QScriptDebuggerStackWidgetPrivate::QScriptDebuggerStackWidgetPrivate()
{
}

QScriptDebuggerStackWidgetPrivate::~QScriptDebuggerStackWidgetPrivate()
{
}

void QScriptDebuggerStackWidgetPrivate::_q_onCurrentChanged(const QModelIndex &index)
{
    Q_Q(QScriptDebuggerStackWidget);
    emit q->currentFrameChanged(index.row());
}

QScriptDebuggerStackWidget::QScriptDebuggerStackWidget(QWidget *parent)
    : QScriptDebuggerStackWidgetInterface(*new QScriptDebuggerStackWidgetPrivate, parent, 0)
{
    Q_D(QScriptDebuggerStackWidget);
    d->view = new QTreeView();
    d->view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    d->view->setAlternatingRowColors(true);
    d->view->setRootIsDecorated(false);
    d->view->setSelectionBehavior(QAbstractItemView::SelectRows);
    d->view->header()->setDefaultAlignment(Qt::AlignLeft);
//    d->view->header()->setResizeMode(QHeaderView::ResizeToContents);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setMargin(0);
    vbox->addWidget(d->view);
}

QScriptDebuggerStackWidget::~QScriptDebuggerStackWidget()
{
}

/*!
  \reimp
*/
QAbstractItemModel *QScriptDebuggerStackWidget::stackModel() const
{
    Q_D(const QScriptDebuggerStackWidget);
    return d->view->model();
}

/*!
  \reimp
*/
void QScriptDebuggerStackWidget::setStackModel(QAbstractItemModel *model)
{
    Q_D(QScriptDebuggerStackWidget);
    d->view->setModel(model);
    QObject::connect(d->view->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
                     this, SLOT(_q_onCurrentChanged(QModelIndex)));
    d->view->header()->resizeSection(0, 50);
}

/*!
  \reimp
*/
int QScriptDebuggerStackWidget::currentFrameIndex() const
{
    Q_D(const QScriptDebuggerStackWidget);
    return d->view->currentIndex().row();
}

/*!
  \reimp
*/
void QScriptDebuggerStackWidget::setCurrentFrameIndex(int frameIndex)
{
    Q_D(QScriptDebuggerStackWidget);
    d->view->setCurrentIndex(d->view->model()->index(frameIndex, 0));
}

QT_END_NAMESPACE

#include "moc_qscriptdebuggerstackwidget_p.h"

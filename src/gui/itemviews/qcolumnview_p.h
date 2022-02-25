/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QCOLUMNVIEW_P_H
#define QCOLUMNVIEW_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qcolumnview.h"

#ifndef QT_NO_COLUMNVIEW

#include "qabstractitemview_p.h"
#include "qabstractitemmodel.h"
#include "qpropertyanimation.h"
#include "qabstractitemdelegate.h"
#include "qabstractitemview.h"
#include "qitemdelegate.h"
#include "qlistview.h"
#include "qevent.h"
#include "qscrollbar.h"

QT_BEGIN_NAMESPACE

class QColumnViewPreviewColumn : public QAbstractItemView {

public:
    QColumnViewPreviewColumn(QWidget *parent) : QAbstractItemView(parent), previewWidget(0) {
    }

    void setPreviewWidget(QWidget *widget) {
        previewWidget = widget;
        setMinimumWidth(previewWidget->minimumWidth());
    }

    void resizeEvent(QResizeEvent * event){
        if (!previewWidget)
            return;
        previewWidget->resize(
                qMax(previewWidget->minimumWidth(), event->size().width()),
                previewWidget->height());
        QSize p = viewport()->size();
        QSize v = previewWidget->size();
        horizontalScrollBar()->setRange(0, v.width() - p.width());
        horizontalScrollBar()->setPageStep(p.width());
        verticalScrollBar()->setRange(0, v.height() - p.height());
        verticalScrollBar()->setPageStep(p.height());

        QAbstractScrollArea::resizeEvent(event);
    }

    QRect visualRect(const QModelIndex &) const
    {
        return QRect();
    }
    void scrollTo(const QModelIndex &, ScrollHint)
    {
    }
    QModelIndex indexAt(const QPoint &) const
    {
        return QModelIndex();
    }
    QModelIndex moveCursor(CursorAction, Qt::KeyboardModifiers)
    {
        return QModelIndex();
    }
    int horizontalOffset () const {
        return 0;
    }
    int verticalOffset () const {
        return 0;
    }
    QRegion visualRegionForSelection(const QItemSelection &) const
    {
        return QRegion();
    }
    bool isIndexHidden(const QModelIndex &) const
    {
        return false;
    }
    void setSelection(const QRect &, QItemSelectionModel::SelectionFlags)
    {
    }
private:
    QWidget *previewWidget;
};

class Q_AUTOTEST_EXPORT QColumnViewPrivate : public QAbstractItemViewPrivate
{
    Q_DECLARE_PUBLIC(QColumnView)

public:
    QColumnViewPrivate();
    ~QColumnViewPrivate();
    void initialize();

    QAbstractItemView *createColumn(const QModelIndex &index, bool show);

    void updateScrollbars();
    void closeColumns(const QModelIndex &parent = QModelIndex(), bool build = false);
    void doLayout();
    void setPreviewWidget(QWidget *widget);
    void checkColumnCreation(const QModelIndex &parent);


    void _q_gripMoved(int offset);
    void _q_changeCurrentColumn();
    void _q_clicked(const QModelIndex &index);
    void _q_columnsInserted(const QModelIndex &parent, int start, int end);

    QList<QAbstractItemView*> columns;
    QVector<int> columnSizes; // used during init and corner moving
    bool showResizeGrips;
    int offset;
#ifndef QT_NO_ANIMATION
    QPropertyAnimation currentAnimation;
#endif
    QWidget *previewWidget;
    QAbstractItemView *previewColumn;
};

/*!
 * This is a delegate that will paint the triangle
 */
class QColumnViewDelegate : public QItemDelegate
{

public:
    explicit QColumnViewDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}
    ~QColumnViewDelegate() {}

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
};

QT_END_NAMESPACE

#endif // QT_NO_COLUMNVIEW

#endif // QCOLUMNVIEW_P_H


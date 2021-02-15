/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the examples of the Katie Toolkit.
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

#ifndef RECYCLEDLISTITEM_H
#define RECYCLEDLISTITEM_H

#include "abstractviewitem.h"

class ListItem;
class QGraphicsWidget;
class QGraphicsGridLayout;

class RecycledListItem : public AbstractViewItem
{
    Q_OBJECT
public:
    RecycledListItem(QGraphicsWidget *parent=0);
    virtual ~RecycledListItem();

    virtual void setModel(QAbstractItemModel *model);

    virtual AbstractViewItem *newItemInstance();
    virtual void updateItemContents();

    virtual QVariant data(int role) const;
    virtual void setData(const QVariant &value, int role = Qt::DisplayRole);

    ListItem *item() { return m_item; }

    void setTwoColumns(const bool enabled);

protected:
    virtual QSizeF effectiveSizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
    Q_DISABLE_COPY(RecycledListItem)

    ListItem *m_item;
    ListItem *m_item2;
    QAbstractItemModel *m_model;
    QGraphicsGridLayout *m_layout;
};

#endif // RECYCLEDLISTITEM_H

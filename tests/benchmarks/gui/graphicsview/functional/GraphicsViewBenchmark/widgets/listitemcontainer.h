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

#ifndef LISTITEMCONTAINER_H
#define LISTITEMCONTAINER_H

#include <QGraphicsWidget>
#include <QColor>

#include "abstractitemcontainer.h"

class QGraphicsLinearLayout;
class AbstractViewItem;
class ListItemCache;
class ListItem;
class ItemRecyclingList;

class ListItemContainer : public AbstractItemContainer
{
    Q_OBJECT

public:
    ListItemContainer(int bufferSize, ItemRecyclingList *view, QGraphicsWidget *parent=0);
    virtual ~ListItemContainer();

    virtual void setTwoColumns(const bool twoColumns);

#if (QT_VERSION >= 0x040600)
    bool listItemCaching() const;
    void setListItemCaching(const bool enabled);
    virtual void setListItemCaching(const bool enabled, const int index);
#endif

protected:

   virtual void addItemToVisibleLayout(int index, AbstractViewItem *item);
   virtual void removeItemFromVisibleLayout(AbstractViewItem *item);

   virtual void adjustVisibleContainerSize(const QSizeF &size);
   virtual int maxItemCountInItemBuffer() const;

private:
    Q_DISABLE_COPY(ListItemContainer)

    ItemRecyclingList *m_view;
    QGraphicsLinearLayout *m_layout;
#if (QT_VERSION >= 0x040600)
    void setListItemCaching(const bool enabled, ListItem *listItem);
    bool m_listItemCaching;
#endif
};


#endif // LISTITEMCONTAINER_H

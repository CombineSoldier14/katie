/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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

#include <QtGui>

#include "label.h"

Label::Label(const QString& text, QGraphicsItem *parent) 
    : GvbWidget(parent)
{
    m_textItem = new QGraphicsSimpleTextItem(this);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    setContentsMargins(0, 0, 0, 0);
    setText(text);
#if QT_VERSION >= 0x040600
    // This flag was introduced in Qt 4.6.
    setFlag(QGraphicsItem::ItemHasNoContents, true);
#endif
}

Label::~Label()
{
}

void Label::setText(const QString& text) 
{ 
    m_textItem->setText(text);
    prepareGeometryChange();
}

QString Label::text() const
{
    return m_textItem->text();
}

void Label::setFont(const QFont font)
{
    m_textItem->setFont(font);
}

void Label::resizeEvent(QGraphicsSceneResizeEvent *event) 
{
    GvbWidget::resizeEvent(event);
}

QSizeF Label::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    switch (which)
    {
    case Qt::MinimumSize:
        // fall thru
    case Qt::PreferredSize:
        {
        QFontMetricsF fm(m_textItem->font());
        return QSizeF(fm.width(m_textItem->text()), fm.height());
        }
    default:
        return GvbWidget::sizeHint(which, constraint);
    }
}

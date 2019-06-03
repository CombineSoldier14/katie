/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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

#ifndef QDRAG_H
#define QDRAG_H

#include <QtCore/qobject.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


#ifndef QT_NO_DRAGANDDROP
class QMimeData;
class QDragPrivate;
class QWidget;
class QPixmap;
class QPoint;
class QDragManager;

class Q_GUI_EXPORT QDrag : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDrag)
public:
    explicit QDrag(QWidget *dragSource);
    ~QDrag();

    void setMimeData(QMimeData *data);
    QMimeData *mimeData() const;

    void setPixmap(const QPixmap &);
    QPixmap pixmap() const;

    void setHotSpot(const QPoint &hotspot);
    QPoint hotSpot() const;

    QWidget *source() const;
    QWidget *target() const;

    Qt::DropAction start(Qt::DropActions supportedActions = Qt::CopyAction);
    Qt::DropAction exec(Qt::DropActions supportedActions = Qt::MoveAction,
                        Qt::DropAction defaultAction = Qt::IgnoreAction);

    void setDragCursor(const QPixmap &cursor, Qt::DropAction action);

Q_SIGNALS:
    void actionChanged(Qt::DropAction action);
    void targetChanged(QWidget *newTarget);

private:
    friend class QDragManager;
    Q_DISABLE_COPY(QDrag)
};

#endif // QT_NO_DRAGANDDROP

QT_END_NAMESPACE

QT_END_HEADER

#endif // QDRAG_H

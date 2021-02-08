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

#ifndef QGRAPHICSTRANSFORM_P_H
#define QGRAPHICSTRANSFORM_P_H

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

#include "qobject_p.h"
#ifndef QT_NO_GRAPHICSVIEW
QT_BEGIN_NAMESPACE

class QGraphicsItem;

class QGraphicsTransformPrivate : public QObjectPrivate {
public:
    Q_DECLARE_PUBLIC(QGraphicsTransform)

    QGraphicsTransformPrivate()
        : QObjectPrivate(), item(0) {}

    QGraphicsItem *item;

    void setItem(QGraphicsItem *item);
    static void updateItem(QGraphicsItem *item);
};

QT_END_NAMESPACE
#endif //QT_NO_GRAPHCISVIEW

#endif // QGRAPHICSTRANSFORM_P_H

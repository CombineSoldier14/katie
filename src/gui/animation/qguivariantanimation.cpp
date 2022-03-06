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
#include "qvariantanimation.h"
#include "qvariantanimation_p.h"

#ifndef QT_NO_ANIMATION

#include <QtGui/qcolor.h>
#include <QtGui/qvector2d.h>
#include <QtGui/qvector3d.h>
#include <QtGui/qvector4d.h>

QT_BEGIN_NAMESPACE

template<> Q_INLINE_TEMPLATE QColor _q_interpolate(const QColor &f,const QColor &t, qreal progress)
{
    return QColor(qBound(0,_q_interpolate(f.red(), t.red(), progress),255),
                  qBound(0,_q_interpolate(f.green(), t.green(), progress),255),
                  qBound(0,_q_interpolate(f.blue(), t.blue(), progress),255),
                  qBound(0,_q_interpolate(f.alpha(), t.alpha(), progress),255));
}

static int qRegisterGuiGetInterpolator()
{
    qRegisterAnimationInterpolator<QColor>(_q_interpolateVariant<QColor>);
#ifndef QT_NO_VECTOR2D
    qRegisterAnimationInterpolator<QVector2D>(_q_interpolateVariant<QVector2D>);
#endif // QT_NO_VECTOR2D
#ifndef QT_NO_VECTOR3D
    qRegisterAnimationInterpolator<QVector3D>(_q_interpolateVariant<QVector3D>);
#endif // QT_NO_VECTOR3D
#ifndef QT_NO_VECTOR4D
    qRegisterAnimationInterpolator<QVector4D>(_q_interpolateVariant<QVector4D>);
#endif // QT_NO_VECTOR4D
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(qRegisterGuiGetInterpolator)

static int qUnregisterGuiGetInterpolator()
{
    // casts required by Sun CC 5.5
    qRegisterAnimationInterpolator<QColor>(
        (QVariant (*)(const QColor &, const QColor &, qreal))0);
#ifndef QT_NO_VECTOR2D
    qRegisterAnimationInterpolator<QVector2D>(
        (QVariant (*)(const QVector2D &, const QVector2D &, qreal))0);
#endif // QT_NO_VECTOR2D
#ifndef QT_NO_VECTOR3D
    qRegisterAnimationInterpolator<QVector3D>(
        (QVariant (*)(const QVector3D &, const QVector3D &, qreal))0);
#endif // QT_NO_VECTOR3D
#ifndef QT_NO_VECTOR4D
    qRegisterAnimationInterpolator<QVector4D>(
        (QVariant (*)(const QVector4D &, const QVector4D &, qreal))0);
#endif // QT_NO_VECTOR4D

    return 1;
}
Q_DESTRUCTOR_FUNCTION(qUnregisterGuiGetInterpolator)

QT_END_NAMESPACE

#endif //QT_NO_ANIMATION



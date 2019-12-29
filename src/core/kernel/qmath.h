/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QMATH_H
#define QMATH_H

#include <cmath>

#include <QtCore/qglobal.h>


QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


#define QT_SINE_TABLE_SIZE 256

extern Q_CORE_EXPORT const qreal qt_sine_table[QT_SINE_TABLE_SIZE];

inline int qRound(qreal d)
{
    return int(std::round(d));
}

inline qint64 qRound64(qreal d)
{
    return qint64(std::round(d));
}

inline int qCeil(qreal v)
{
#ifdef QT_USE_MATH_H_FLOATS
    if (sizeof(qreal) == sizeof(float))
        return int(ceilf(float(v)));
    else
#endif
        return int(ceil(v));
}

inline int qFloor(qreal v)
{
#ifdef QT_USE_MATH_H_FLOATS
    if (sizeof(qreal) == sizeof(float))
        return int(floorf(float(v)));
    else
#endif
        return int(floor(v));
}

inline qreal qFabs(qreal v)
{
#ifdef QT_USE_MATH_H_FLOATS
    if(sizeof(qreal) == sizeof(float))
        return fabsf(float(v));
    else
#endif
        return fabs(v);
}

inline qreal qSin(qreal v)
{
#    ifdef QT_USE_MATH_H_FLOATS
        if (sizeof(qreal) == sizeof(float))
            return sinf(float(v));
        else
#    endif
            return sin(v);
}

inline qreal qCos(qreal v)
{
#    ifdef QT_USE_MATH_H_FLOATS
        if (sizeof(qreal) == sizeof(float))
            return cosf(float(v));
        else
#    endif
            return cos(v);
}

inline qreal qTan(qreal v)
{
#    ifdef QT_USE_MATH_H_FLOATS
        if (sizeof(qreal) == sizeof(float))
            return tanf(float(v));
        else
#    endif
            return tan(v);
}

inline qreal qAcos(qreal v)
{
#    ifdef QT_USE_MATH_H_FLOATS
        if (sizeof(qreal) == sizeof(float))
            return acosf(float(v));
        else
#    endif
           return acos(v);
}

inline qreal qAsin(qreal v)
{
#    ifdef QT_USE_MATH_H_FLOATS
        if (sizeof(qreal) == sizeof(float))
            return asinf(float(v));
        else
#    endif
            return asin(v);
}

inline qreal qAtan(qreal v)
{
#    ifdef QT_USE_MATH_H_FLOATS
        if(sizeof(qreal) == sizeof(float))
            return atanf(float(v));
        else
#    endif
            return atan(v);
}

inline qreal qAtan2(qreal x, qreal y)
{
#    ifdef QT_USE_MATH_H_FLOATS
        if(sizeof(qreal) == sizeof(float))
            return atan2f(float(x), float(y));
        else
#    endif
            return atan2(x, y);
}

inline qreal qSqrt(qreal v)
{
#    ifdef QT_USE_MATH_H_FLOATS
        if (sizeof(qreal) == sizeof(float))
            return sqrtf(float(v));
        else
#    endif
            return sqrt(v);
}

inline qreal qLn(qreal v)
{
#ifdef QT_USE_MATH_H_FLOATS
    if (sizeof(qreal) == sizeof(float))
        return logf(float(v));
    else
#endif
        return log(v);
}

inline qreal qExp(qreal v)
{
    // only one signature
    // exists, exp(double)
    return exp(v);
}

inline qreal qPow(qreal x, qreal y)
{
#    ifdef QT_USE_MATH_H_FLOATS
        if (sizeof(qreal) == sizeof(float))
            return powf(float(x), float(y));
        else
#    endif
            return pow(x, y);
}

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

inline qreal qFastSin(qreal x)
{
    int si = int(x * (0.5 * QT_SINE_TABLE_SIZE / M_PI)); // Would be more accurate with qRound, but slower.
    qreal d = x - si * (2.0 * M_PI / QT_SINE_TABLE_SIZE);
    int ci = si + QT_SINE_TABLE_SIZE / 4;
    si &= QT_SINE_TABLE_SIZE - 1;
    ci &= QT_SINE_TABLE_SIZE - 1;
    return qt_sine_table[si] + (qt_sine_table[ci] - 0.5 * qt_sine_table[si] * d) * d;
}

inline qreal qFastCos(qreal x)
{
    int ci = int(x * (0.5 * QT_SINE_TABLE_SIZE / M_PI)); // Would be more accurate with qRound, but slower.
    qreal d = x - ci * (2.0 * M_PI / QT_SINE_TABLE_SIZE);
    int si = ci + QT_SINE_TABLE_SIZE / 4;
    si &= QT_SINE_TABLE_SIZE - 1;
    ci &= QT_SINE_TABLE_SIZE - 1;
    return qt_sine_table[si] - (qt_sine_table[ci] + 0.5 * qt_sine_table[si] * d) * d;
}

QT_END_NAMESPACE

QT_END_HEADER

#endif // QMATH_H

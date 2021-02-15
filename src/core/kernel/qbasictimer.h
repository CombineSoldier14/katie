/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QBASICTIMER_H
#define QBASICTIMER_H

#include <QtCore/qglobal.h>


QT_BEGIN_NAMESPACE

class QObject;

class Q_CORE_EXPORT QBasicTimer
{
    int id;
public:
    inline QBasicTimer() : id(0) {}
    inline ~QBasicTimer() { if (id) stop(); }

    inline bool isActive() const { return id != 0; }
    inline int timerId() const { return id; }

    void start(int msec, QObject *obj);
    void stop();
};
Q_DECLARE_TYPEINFO(QBasicTimer, Q_MOVABLE_TYPE);

QT_END_NAMESPACE


#endif // QBASICTIMER_H

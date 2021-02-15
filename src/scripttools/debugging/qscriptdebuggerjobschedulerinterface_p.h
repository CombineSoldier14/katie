/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QSCRIPTDEBUGGERJOBSCHEDULERINTERFACE_P_H
#define QSCRIPTDEBUGGERJOBSCHEDULERINTERFACE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qobjectdefs.h>

QT_BEGIN_NAMESPACE

class QScriptDebuggerJob;

class Q_AUTOTEST_EXPORT QScriptDebuggerJobSchedulerInterface
{
public:
    virtual ~QScriptDebuggerJobSchedulerInterface() {}

    virtual int scheduleJob(QScriptDebuggerJob *job) = 0;
    virtual void finishJob(QScriptDebuggerJob *job) = 0;
    virtual void hibernateUntilEvaluateFinished(QScriptDebuggerJob *job) = 0;
};

QT_END_NAMESPACE

#endif

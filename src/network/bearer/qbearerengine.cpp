/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
**
** This file is part of the QtNetwork module of the Katie Toolkit.
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

#include "qbearerengine_p.h"

#ifndef QT_NO_BEARERMANAGEMENT

QT_BEGIN_NAMESPACE

QBearerEngine::QBearerEngine(QObject *parent)
    : QObject(parent), mutex(QMutex::Recursive)
{
}

QBearerEngine::~QBearerEngine()
{
    QMutexLocker locker(&mutex);

    QHash<QString, QNetworkConfigurationPrivatePointer>::Iterator it;
    QHash<QString, QNetworkConfigurationPrivatePointer>::Iterator end;

    for (it = accessPointConfigurations.begin(), end = accessPointConfigurations.end();
         it != end; ++it) {
        it.value()->isValid = false;
        it.value()->id.clear();
    }
    accessPointConfigurations.clear();
}

bool QBearerEngine::requiresPolling() const
{
    return false;
}

/*
    Returns true if configurations are in use; otherwise returns false.

    If configurations are in use and requiresPolling() returns true, polling will be enabled for
    this engine.
*/
bool QBearerEngine::configurationsInUse() const
{
    QHash<QString, QNetworkConfigurationPrivatePointer>::ConstIterator it;
    QHash<QString, QNetworkConfigurationPrivatePointer>::ConstIterator end;

    for (it = accessPointConfigurations.constBegin(),
         end = accessPointConfigurations.constEnd(); it != end; ++it) {
        if (it.value()->ref > 1)
            return true;
    }

    return false;
}

#include "moc_qbearerengine_p.h"

#endif // QT_NO_BEARERMANAGEMENT

QT_END_NAMESPACE



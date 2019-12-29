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

#ifndef QNETWORKCONFIGURATION_H
#define QNETWORKCONFIGURATION_H

#include <QtCore/qshareddata.h>
#include <QtCore/qstring.h>
#include <QtCore/qlist.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QNetworkConfigurationPrivate;
class Q_NETWORK_EXPORT QNetworkConfiguration
{
public:
    QNetworkConfiguration();
    QNetworkConfiguration(const QNetworkConfiguration& other);
    QNetworkConfiguration &operator=(const QNetworkConfiguration &other);
    ~QNetworkConfiguration();

    bool operator==(const QNetworkConfiguration &other) const;
    inline bool operator!=(const QNetworkConfiguration &other) const
    { return !operator==(other); }

    enum Type {
        InternetAccessPoint = 0,
        ServiceNetwork,
        UserChoice,
        Invalid
    };

    enum Purpose {
        UnknownPurpose = 0,
        PublicPurpose,
        PrivatePurpose,
        ServiceSpecificPurpose
    };

    enum StateFlag {
        Undefined        = 0x0000001,
        Defined          = 0x0000002,
        Discovered       = 0x0000006,
        Active           = 0x000000e
    };
    Q_DECLARE_FLAGS(StateFlags, StateFlag)

    enum BearerType {
        BearerUnknown,
        BearerEthernet,
        BearerWLAN,
        Bearer2G,
        BearerCDMA2000,
        BearerWCDMA,
        BearerHSPA,
        BearerBluetooth,
        BearerWiMAX
    };

    StateFlags state() const;
    Type type() const;
    Purpose purpose() const;

    BearerType bearerType() const;
    QString bearerTypeName() const;

    QString identifier() const;
    bool isRoamingAvailable() const;
    QList<QNetworkConfiguration> children() const;

    QString name() const;
    bool isValid() const;

private:
    friend class QNetworkConfigurationPrivate;
    friend class QNetworkConfigurationManager;
    friend class QNetworkConfigurationManagerPrivate;
    friend class QNetworkSessionPrivate;
    QExplicitlySharedDataPointer<QNetworkConfigurationPrivate> d;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QNETWORKCONFIGURATION_H

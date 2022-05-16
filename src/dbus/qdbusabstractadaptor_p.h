/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDBus module of the Katie Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the public API.  This header file may
// change from version to version without notice, or even be
// removed.
//
// We mean it.
//
//

#ifndef QDBUSABSTRACTADAPTORPRIVATE_H
#define QDBUSABSTRACTADAPTORPRIVATE_H

#include "qdbusabstractadaptor.h"

#include <QtCore/qobject.h>
#include <QtCore/qmap.h>
#include <QtCore/qhash.h>
#include <QtCore/qvariant.h>
#include <QtCore/qvector.h>
#include "qobject_p.h"

#define QCLASSINFO_DBUS_INTERFACE       "D-Bus Interface"
#define QCLASSINFO_DBUS_INTROSPECTION   "D-Bus Introspection"


QT_BEGIN_NAMESPACE

class QDBusAbstractAdaptor;
class QDBusAdaptorConnector;
class QDBusAdaptorManager;
class QDBusConnectionPrivate;

class QDBusAbstractAdaptorPrivate: public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QDBusAbstractAdaptor)
public:
    QDBusAbstractAdaptorPrivate() : autoRelaySignals(false) {}
    QString xml;
    bool autoRelaySignals;

    static QString retrieveIntrospectionXml(QDBusAbstractAdaptor *adaptor);
    static void saveIntrospectionXml(QDBusAbstractAdaptor *adaptor, const QString &xml);
};

class QDBusAdaptorConnector: public QObject
{
    Q_OBJECT_FAKE

public: // typedefs
    struct AdaptorData
    {
        const char *interface;
        QDBusAbstractAdaptor *adaptor;

        inline bool operator<(const AdaptorData &other) const
        { return QByteArray(interface) < other.interface; }
        inline bool operator<(const QString &other) const
        { return QLatin1String(interface) < other; }
        inline bool operator<(const QByteArray &other) const
        { return interface < other; }
    };
    typedef QVector<AdaptorData> AdaptorMap;

public: // methods
    explicit QDBusAdaptorConnector(QObject *parent);
    ~QDBusAdaptorConnector();

    void addAdaptor(QDBusAbstractAdaptor *adaptor);
    void connectAllSignals(QObject *object);
    void disconnectAllSignals(QObject *object);
    void relay(QObject *sender, int id, void **);

//public slots:
    void relaySlot(void **);
    void polish();

protected:
//signals:
    void relaySignal(QObject *obj, const QMetaObject *metaObject, int sid, const QVariantList &args);

public: // member variables
    AdaptorMap adaptors;
    bool waitingForPolish : 1;

private:
    static int relaySlotMethodIndex();
};

extern QDBusAdaptorConnector *qDBusFindAdaptorConnector(QObject *object);
extern QDBusAdaptorConnector *qDBusCreateAdaptorConnector(QObject *object);

QT_END_NAMESPACE

#endif // QDBUSABSTRACTADAPTORPRIVATE_H

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

#ifndef QABSTRACTSOCKETENGINE_P_H
#define QABSTRACTSOCKETENGINE_P_H

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

#include "QtCore/qhostaddress.h"
#include "QtCore/qabstractsocket.h"
#include "qobject_p.h"

QT_BEGIN_NAMESPACE

class QAbstractSocketEnginePrivate;

class QAbstractSocketEngineReceiver {
public:
    virtual ~QAbstractSocketEngineReceiver(){}
    virtual void readNotification()= 0;
    virtual void writeNotification()= 0;
    virtual void exceptionNotification()= 0;
    virtual void connectionNotification()= 0;
};

class Q_AUTOTEST_EXPORT QAbstractSocketEngine : public QObject
{
    Q_OBJECT
public:

    static QAbstractSocketEngine *createSocketEngine(QAbstractSocket::SocketType socketType, QObject *parent);
    static QAbstractSocketEngine *createSocketEngine(int socketDescripter, QObject *parent);

    QAbstractSocketEngine(QObject *parent = nullptr);

    enum SocketOption {
        NonBlockingSocketOption,
        BroadcastSocketOption,
        ReceiveBufferSocketOption,
        SendBufferSocketOption,
        AddressReusable,
        ReceiveOutOfBandData,
        LowDelayOption,
        KeepAliveOption,
        MulticastTtlOption,
        MulticastLoopbackOption
    };

    virtual bool initialize(QAbstractSocket::SocketType type, QAbstractSocket::NetworkLayerProtocol protocol = QAbstractSocket::IPv4Protocol) = 0;

    virtual bool initialize(int socketDescriptor, QAbstractSocket::SocketState socketState = QAbstractSocket::ConnectedState) = 0;

    virtual int socketDescriptor() const = 0;

    virtual bool isValid() const = 0;

    virtual bool connectToHost(const QHostAddress &address, quint16 port) = 0;
    virtual bool connectToHostByName(const QString &name, quint16 port) = 0;
    virtual bool bind(const QHostAddress &address, quint16 port) = 0;
    virtual bool listen() = 0;
    virtual int accept() = 0;
    virtual void close() = 0;

    virtual qint64 bytesAvailable() const = 0;

    virtual qint64 read(char *data, qint64 maxlen) = 0;
    virtual qint64 write(const char *data, qint64 len) = 0;

#ifndef QT_NO_UDPSOCKET
    virtual qint64 readDatagram(char *data, qint64 maxlen, QHostAddress *addr = 0,
                                quint16 *port = 0) = 0;
    virtual qint64 writeDatagram(const char *data, qint64 len, const QHostAddress &addr,
                                 quint16 port) = 0;
    virtual bool hasPendingDatagrams() const = 0;
    virtual qint64 pendingDatagramSize() const = 0;
#endif // QT_NO_UDPSOCKET

    virtual qint64 bytesToWrite() const = 0;

    virtual int option(SocketOption option) const = 0;
    virtual bool setOption(SocketOption option, int value) = 0;

    virtual bool waitForRead(int msecs = 30000, bool *timedOut = 0) = 0;
    virtual bool waitForWrite(int msecs = 30000, bool *timedOut = 0) = 0;
    virtual bool waitForReadOrWrite(bool *readyToRead, bool *readyToWrite,
                                    bool checkRead, bool checkWrite,
                                    int msecs = 30000, bool *timedOut = 0) = 0;

    QAbstractSocket::SocketError error() const;
    QString errorString() const;
    QAbstractSocket::SocketState state() const;
    QAbstractSocket::SocketType socketType() const;
    QAbstractSocket::NetworkLayerProtocol protocol() const;

    QHostAddress localAddress() const;
    quint16 localPort() const;
    QHostAddress peerAddress() const;
    quint16 peerPort() const;

    virtual bool isReadNotificationEnabled() const = 0;
    virtual void setReadNotificationEnabled(bool enable) = 0;
    virtual bool isWriteNotificationEnabled() const = 0;
    virtual void setWriteNotificationEnabled(bool enable) = 0;
    virtual bool isExceptionNotificationEnabled() const = 0;
    virtual void setExceptionNotificationEnabled(bool enable) = 0;

public Q_SLOTS:
    void readNotification();
    void writeNotification();
    void exceptionNotification();
    void connectionNotification();

public:
    void setReceiver(QAbstractSocketEngineReceiver *receiver);
protected:
    QAbstractSocketEngine(QAbstractSocketEnginePrivate &dd, QObject *parent = nullptr);

    void setError(QAbstractSocket::SocketError error, const QString &errorString);
    void setState(QAbstractSocket::SocketState state);
    void setSocketType(QAbstractSocket::SocketType socketType);
    void setProtocol(QAbstractSocket::NetworkLayerProtocol protocol);
    void setLocalAddress(const QHostAddress &address);
    void setLocalPort(quint16 port);
    void setPeerAddress(const QHostAddress &address);
    void setPeerPort(quint16 port);

private:
    Q_DECLARE_PRIVATE(QAbstractSocketEngine)
    Q_DISABLE_COPY(QAbstractSocketEngine)
};

class QAbstractSocketEnginePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QAbstractSocketEngine)
public:
    QAbstractSocketEnginePrivate();

    QAbstractSocket::SocketError socketError;
    bool hasSetSocketError;
    QString socketErrorString;
    QAbstractSocket::SocketState socketState;
    QAbstractSocket::SocketType socketType;
    QAbstractSocket::NetworkLayerProtocol socketProtocol;
    QHostAddress localAddress;
    quint16 localPort;
    QHostAddress peerAddress;
    quint16 peerPort;
    QAbstractSocketEngineReceiver *receiver;
};


class Q_AUTOTEST_EXPORT QSocketEngineHandler
{
protected:
    QSocketEngineHandler();
    virtual ~QSocketEngineHandler();
    virtual QAbstractSocketEngine *createSocketEngine(QAbstractSocket::SocketType socketType,
                                                      QObject *parent) = 0;
    virtual QAbstractSocketEngine *createSocketEngine(int socketDescripter, QObject *parent) = 0;

private:
    friend class QAbstractSocketEngine;
};

QT_END_NAMESPACE

#endif // QABSTRACTSOCKETENGINE_P_H

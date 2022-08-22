/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qlocalsocket.h"
#include "qlocalsocket_p.h"
#include "qnet_unix_p.h"

#ifndef QT_NO_LOCALSOCKET

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "qdir.h"
#include "qdebug.h"
#include "qelapsedtimer.h"

#define QT_CONNECT_TIMEOUT 30000

QT_BEGIN_NAMESPACE

QLocalSocketPrivate::QLocalSocketPrivate()
    : QIODevicePrivate(),
    connectingSocket(-1),
    connectingOpenMode(0),
    state(QLocalSocket::UnconnectedState)
{
}

void QLocalSocketPrivate::init()
{
    Q_Q(QLocalSocket);
    // QIODevice signals
    q->connect(&unixSocket, SIGNAL(aboutToClose()), q, SIGNAL(aboutToClose()));
    q->connect(&unixSocket, SIGNAL(bytesWritten(qint64)),
               q, SIGNAL(bytesWritten(qint64)));
    q->connect(&unixSocket, SIGNAL(readyRead()), q, SIGNAL(readyRead()));
    // QAbstractSocket signals
    q->connect(&unixSocket, SIGNAL(connected()), q, SIGNAL(connected()));
    q->connect(&unixSocket, SIGNAL(disconnected()), q, SIGNAL(disconnected()));
    q->connect(&unixSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
               q, SLOT(_q_stateChanged(QAbstractSocket::SocketState)));
    q->connect(&unixSocket, SIGNAL(error(QAbstractSocket::SocketError)),
               q, SLOT(_q_error(QAbstractSocket::SocketError)));
    q->connect(&unixSocket, SIGNAL(readChannelFinished()), q, SIGNAL(readChannelFinished()));
    unixSocket.setParent(q);
}

void QLocalSocketPrivate::_q_error(QAbstractSocket::SocketError socketError)
{
    Q_Q(QLocalSocket);
    QLocalSocket::LocalSocketError error = (QLocalSocket::LocalSocketError)socketError;
    q->setErrorString(generateErrorString(error, QLatin1String("QLocalSocket")));
    emit q->error(error);
}

void QLocalSocketPrivate::_q_stateChanged(QAbstractSocket::SocketState newState)
{
    Q_Q(QLocalSocket);
    QLocalSocket::LocalSocketState currentState = state;
    switch(newState) {
    case QAbstractSocket::UnconnectedState:
        state = QLocalSocket::UnconnectedState;
        serverName.clear();
        fullServerName.clear();
        break;
    case QAbstractSocket::ConnectingState:
        state = QLocalSocket::ConnectingState;
        break;
    case QAbstractSocket::ConnectedState:
        state = QLocalSocket::ConnectedState;
        break;
    case QAbstractSocket::ClosingState:
        state = QLocalSocket::ClosingState;
        break;
    default:
#if defined QLOCALSOCKET_DEBUG
        qWarning() << "QLocalSocket::Unhandled socket state change:" << newState;
#endif
        return;
    }
    if (currentState != state)
        emit q->stateChanged(state);
}

QString QLocalSocketPrivate::generateErrorString(QLocalSocket::LocalSocketError error, const QString &function) const
{
    QString errorString;
    switch (error) {
    case QLocalSocket::ConnectionRefusedError:
        errorString = QLocalSocket::tr("%1: Connection refused").arg(function);
        break;
    case QLocalSocket::PeerClosedError:
        errorString = QLocalSocket::tr("%1: Remote closed").arg(function);
        break;
    case QLocalSocket::ServerNotFoundError:
        errorString = QLocalSocket::tr("%1: Invalid name").arg(function);
        break;
    case QLocalSocket::SocketAccessError:
        errorString = QLocalSocket::tr("%1: Socket access error").arg(function);
        break;
    case QLocalSocket::SocketResourceError:
        errorString = QLocalSocket::tr("%1: Socket resource error").arg(function);
        break;
    case QLocalSocket::SocketTimeoutError:
        errorString = QLocalSocket::tr("%1: Socket operation timed out").arg(function);
        break;
    case QLocalSocket::DatagramTooLargeError:
        errorString = QLocalSocket::tr("%1: Datagram too large").arg(function);
        break;
    case QLocalSocket::ConnectionError:
        errorString = QLocalSocket::tr("%1: Connection error").arg(function);
        break;
    case QLocalSocket::UnsupportedSocketOperationError:
        errorString = QLocalSocket::tr("%1: The socket operation is not supported").arg(function);
        break;
    case QLocalSocket::UnknownSocketError:
    default:
        errorString = QLocalSocket::tr("%1: Unknown error %2").arg(function).arg(errno);
    }
    return errorString;
}

void QLocalSocketPrivate::errorOccurred(QLocalSocket::LocalSocketError error, const QString &function)
{
    Q_Q(QLocalSocket);
    switch (error) {
    case QLocalSocket::ConnectionRefusedError:
        unixSocket.setSocketError(QAbstractSocket::ConnectionRefusedError);
        break;
    case QLocalSocket::PeerClosedError:
        unixSocket.setSocketError(QAbstractSocket::RemoteHostClosedError);
        break;
    case QLocalSocket::ServerNotFoundError:
        unixSocket.setSocketError(QAbstractSocket::HostNotFoundError);
        break;
    case QLocalSocket::SocketAccessError:
        unixSocket.setSocketError(QAbstractSocket::SocketAccessError);
        break;
    case QLocalSocket::SocketResourceError:
        unixSocket.setSocketError(QAbstractSocket::SocketResourceError);
        break;
    case QLocalSocket::SocketTimeoutError:
        unixSocket.setSocketError(QAbstractSocket::SocketTimeoutError);
        break;
    case QLocalSocket::DatagramTooLargeError:
        unixSocket.setSocketError(QAbstractSocket::DatagramTooLargeError);
        break;
    case QLocalSocket::ConnectionError:
        unixSocket.setSocketError(QAbstractSocket::NetworkError);
        break;
    case QLocalSocket::UnsupportedSocketOperationError:
        unixSocket.setSocketError(QAbstractSocket::UnsupportedSocketOperationError);
        break;
    case QLocalSocket::UnknownSocketError:
    default:
        unixSocket.setSocketError(QAbstractSocket::UnknownSocketError);
        break;
    }

    q->setErrorString(generateErrorString(error, function));
    emit q->error(error);

    // errors cause a disconnect
    unixSocket.setSocketState(QAbstractSocket::UnconnectedState);
    bool stateChanged = (state != QLocalSocket::UnconnectedState);
    state = QLocalSocket::UnconnectedState;
    q->close();
    if (stateChanged)
        q->emit stateChanged(state);
}

void QLocalSocket::connectToServer(const QString &name, OpenMode openMode)
{
    Q_D(QLocalSocket);
    if (state() == ConnectedState
        || state() == ConnectingState)
        return;

    d->errorString.clear();
    d->unixSocket.setSocketState(QAbstractSocket::ConnectingState);
    d->state = ConnectingState;
    emit stateChanged(d->state);

    if (name.isEmpty()) {
        d->errorOccurred(ServerNotFoundError,
                QLatin1String("QLocalSocket::connectToServer"));
        return;
    }

    // create the socket
#ifdef SOCK_NONBLOCK
    // Linux specific
    d->connectingSocket = qt_safe_socket(PF_UNIX, SOCK_NONBLOCK | SOCK_STREAM, 0);
#else
    d->connectingSocket = qt_safe_socket(PF_UNIX, SOCK_STREAM, 0);
#endif
    if (d->connectingSocket == -1) {
        d->errorOccurred(UnsupportedSocketOperationError,
                        QLatin1String("QLocalSocket::connectToServer"));
        return;
    }
#ifndef SOCK_NONBLOCK
    // set non blocking so we can try to connect and it wont wait
    int flags = ::fcntl(d->connectingSocket, F_GETFL, 0);
    if (flags == -1 || ::fcntl(d->connectingSocket, F_SETFL, flags | O_NONBLOCK) == -1) {
        d->errorOccurred(UnknownSocketError,
                QLatin1String("QLocalSocket::connectToServer"));
        return;
    }
#endif

    // _q_connectToSocket does the actual connecting
    d->connectingName = name;
    d->connectingOpenMode = openMode;
    d->_q_connectToSocket();
}

/*!
    \internal

    Tries to connect connectingName and connectingOpenMode

    \sa connectToServer() waitForConnected()
  */
void QLocalSocketPrivate::_q_connectToSocket()
{
    Q_Q(QLocalSocket);
    QString connectingPathName;

    // determine the full server path
    if (connectingName.startsWith(QLatin1Char('/'))) {
        connectingPathName = connectingName;
    } else {
        connectingPathName = QDir::tempPath();
        connectingPathName += QLatin1Char('/') + connectingName;
    }

    const QByteArray connectingPathNameLatin = connectingPathName.toLatin1();

    struct sockaddr_un name;
    name.sun_family = PF_UNIX;
    if (sizeof(name.sun_path) < (uint)connectingPathNameLatin.size() + 1) {
        QString function = QLatin1String("QLocalSocket::connectToServer");
        errorOccurred(QLocalSocket::ServerNotFoundError, function);
        return;
    }
    ::memcpy(name.sun_path, connectingPathNameLatin.constData(),
             connectingPathNameLatin.size() + 1);
    if (qt_safe_connect(connectingSocket, (struct sockaddr *)&name, sizeof(name)) == -1) {
        QString function = QLatin1String("QLocalSocket::connectToServer");
        switch (errno)
        {
        case EINVAL:
        case ECONNREFUSED:
            errorOccurred(QLocalSocket::ConnectionRefusedError, function);
            break;
        case ENOENT:
            errorOccurred(QLocalSocket::ServerNotFoundError, function);
            break;
        case EACCES:
        case EPERM:
            errorOccurred(QLocalSocket::SocketAccessError, function);
            break;
        case ETIMEDOUT:
            errorOccurred(QLocalSocket::SocketTimeoutError, function);
            break;
        case EAGAIN:
            errorOccurred(QLocalSocket::UnfinishedSocketOperationError, function);
            break;
        default:
            errorOccurred(QLocalSocket::UnknownSocketError, function);
            break;
        }
        return;
    }

    // connected!
    serverName = connectingName;
    fullServerName = connectingPathName;
    if (unixSocket.setSocketDescriptor(connectingSocket,
        QAbstractSocket::ConnectedState, connectingOpenMode)) {
        q->QIODevice::open(connectingOpenMode);
        q->emit connected();
    } else {
        QString function = QLatin1String("QLocalSocket::connectToServer");
        errorOccurred(QLocalSocket::UnknownSocketError, function);
    }
    connectingSocket = -1;
    connectingName.clear();
    connectingOpenMode = 0;
}

bool QLocalSocket::setSocketDescriptor(int socketDescriptor,
        LocalSocketState socketState, OpenMode openMode)
{
    Q_D(QLocalSocket);
    QAbstractSocket::SocketState newSocketState = QAbstractSocket::UnconnectedState;
    switch (socketState) {
    case ConnectingState:
        newSocketState = QAbstractSocket::ConnectingState;
        break;
    case ConnectedState:
        newSocketState = QAbstractSocket::ConnectedState;
        break;
    case ClosingState:
        newSocketState = QAbstractSocket::ClosingState;
        break;
    case UnconnectedState:
        newSocketState = QAbstractSocket::UnconnectedState;
        break;
    }
    QIODevice::open(openMode);
    d->state = socketState;
    return d->unixSocket.setSocketDescriptor(socketDescriptor,
                                             newSocketState, openMode);
}

void QLocalSocketPrivate::_q_abortConnectionAttempt()
{
    Q_Q(QLocalSocket);
    q->close();
}

int QLocalSocket::socketDescriptor() const
{
    Q_D(const QLocalSocket);
    return d->unixSocket.socketDescriptor();
}

qint64 QLocalSocket::readData(char *data, qint64 c)
{
    Q_D(QLocalSocket);
    return d->unixSocket.readData(data, c);
}

qint64 QLocalSocket::writeData(const char *data, qint64 c)
{
    Q_D(QLocalSocket);
    return d->unixSocket.writeData(data, c);
}

void QLocalSocket::abort()
{
    Q_D(QLocalSocket);
    d->unixSocket.abort();
}

qint64 QLocalSocket::bytesAvailable() const
{
    Q_D(const QLocalSocket);
    return QIODevice::bytesAvailable() + d->unixSocket.bytesAvailable();
}

qint64 QLocalSocket::bytesToWrite() const
{
    Q_D(const QLocalSocket);
    return d->unixSocket.bytesToWrite();
}

bool QLocalSocket::canReadLine() const
{
    Q_D(const QLocalSocket);
    return QIODevice::canReadLine() || d->unixSocket.canReadLine();
}

void QLocalSocket::close()
{
    Q_D(QLocalSocket);
    d->unixSocket.close();
    if (d->connectingSocket != -1)
        qt_safe_close(d->connectingSocket);
    d->connectingSocket = -1;
    d->connectingName.clear();
    d->connectingOpenMode = 0;
    d->serverName.clear();
    d->fullServerName.clear();
    QIODevice::close();
}

bool QLocalSocket::waitForBytesWritten(int msecs)
{
    Q_D(QLocalSocket);
    return d->unixSocket.waitForBytesWritten(msecs);
}

bool QLocalSocket::flush()
{
    Q_D(QLocalSocket);
    return d->unixSocket.flush();
}

void QLocalSocket::disconnectFromServer()
{
    Q_D(QLocalSocket);
    d->unixSocket.disconnectFromHost();
}

QLocalSocket::LocalSocketError QLocalSocket::error() const
{
    Q_D(const QLocalSocket);
    switch (d->unixSocket.error()) {
        case QAbstractSocket::ConnectionRefusedError:
            return QLocalSocket::ConnectionRefusedError;
        case QAbstractSocket::RemoteHostClosedError:
            return QLocalSocket::PeerClosedError;
        case QAbstractSocket::HostNotFoundError:
            return QLocalSocket::ServerNotFoundError;
        case QAbstractSocket::SocketAccessError:
            return QLocalSocket::SocketAccessError;
        case QAbstractSocket::SocketResourceError:
            return QLocalSocket::SocketResourceError;
        case QAbstractSocket::SocketTimeoutError:
            return QLocalSocket::SocketTimeoutError;
        case QAbstractSocket::DatagramTooLargeError:
            return QLocalSocket::DatagramTooLargeError;
        case QAbstractSocket::NetworkError:
            return QLocalSocket::ConnectionError;
        case QAbstractSocket::UnsupportedSocketOperationError:
            return QLocalSocket::UnsupportedSocketOperationError;
        case QAbstractSocket::UnknownSocketError:
            return QLocalSocket::UnknownSocketError;
        default:
#if defined QLOCALSOCKET_DEBUG
            qWarning() << "QLocalSocket error not handled:" << d->unixSocket.error();
#endif
            break;
    }
    return QLocalSocket::UnknownSocketError;
}

bool QLocalSocket::isValid() const
{
    Q_D(const QLocalSocket);
    return d->unixSocket.isValid();
}

qint64 QLocalSocket::readBufferSize() const
{
    Q_D(const QLocalSocket);
    return d->unixSocket.readBufferSize();
}

void QLocalSocket::setReadBufferSize(qint64 size)
{
    Q_D(QLocalSocket);
    d->unixSocket.setReadBufferSize(size);
}

bool QLocalSocket::waitForConnected(int msec)
{
    Q_D(QLocalSocket);
    if (state() != ConnectingState)
        return (state() == ConnectedState);

    if (state() == ConnectingState) {
        // if timeout is 0 poll() will return immidiatly.
        if (msec == 0)
            msec = 1000;

        struct pollfd fds;
        ::memset(&fds, 0, sizeof(struct pollfd));
        fds.fd = d->connectingSocket;
        fds.events = POLLIN;
        int result = qt_safe_poll(&fds, msec);
        if (result == -1) {
            d->errorOccurred( QLocalSocket::UnknownSocketError,
                    QLatin1String("QLocalSocket::waitForConnected"));
            return false;
        }
        if (result > 0)
            d->_q_connectToSocket();
    }

    return (state() == ConnectedState);
}

bool QLocalSocket::waitForDisconnected(int msecs)
{
    Q_D(QLocalSocket);
    if (Q_UNLIKELY(state() == UnconnectedState)) {
        qWarning() << "QLocalSocket::waitForDisconnected() is not allowed in UnconnectedState";
        return false;
    }
    return (d->unixSocket.waitForDisconnected(msecs));
}

bool QLocalSocket::waitForReadyRead(int msecs)
{
    Q_D(QLocalSocket);
    if (state() == QLocalSocket::UnconnectedState)
        return false;
    return (d->unixSocket.waitForReadyRead(msecs));
}

QT_END_NAMESPACE

#endif



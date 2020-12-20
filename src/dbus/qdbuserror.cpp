/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#include "qdbuserror.h"
#include "qdebug.h"
#include "qdbusmessage.h"
#include "qdbusmessage_p.h"

#include <dbus/dbus.h>

QT_BEGIN_NAMESPACE

// in the same order as KnownErrors!
static const struct ErrorTblData {
    const QDBusError::ErrorType code;
    const char* name;
} ErrorTbl[] = {
    { QDBusError::Other, "other\0" },
    { QDBusError::Failed, "org.freedesktop.DBus.Error.Failed\0" },
    { QDBusError::NoMemory, "org.freedesktop.DBus.Error.NoMemory\0" },
    { QDBusError::ServiceUnknown, "org.freedesktop.DBus.Error.ServiceUnknown\0" },
    { QDBusError::NoReply, "org.freedesktop.DBus.Error.NoReply\0" },
    { QDBusError::BadAddress, "org.freedesktop.DBus.Error.BadAddress\0" },
    { QDBusError::NotSupported, "org.freedesktop.DBus.Error.NotSupported\0" },
    { QDBusError::LimitsExceeded, "org.freedesktop.DBus.Error.LimitsExceeded\0" },
    { QDBusError::AccessDenied, "org.freedesktop.DBus.Error.AccessDenied\0" },
    { QDBusError::NoServer, "org.freedesktop.DBus.Error.NoServer\0" },
    { QDBusError::Timeout, "org.freedesktop.DBus.Error.Timeout\0" },
    { QDBusError::NoNetwork, "org.freedesktop.DBus.Error.NoNetwork\0" },
    { QDBusError::AddressInUse, "org.freedesktop.DBus.Error.AddressInUse\0" },
    { QDBusError::Disconnected, "org.freedesktop.DBus.Error.Disconnected\0" },
    { QDBusError::InvalidArgs, "org.freedesktop.DBus.Error.InvalidArgs\0" },
    { QDBusError::UnknownMethod, "org.freedesktop.DBus.Error.UnknownMethod\0" },
    { QDBusError::TimedOut, "org.freedesktop.DBus.Error.TimedOut\0" },
    { QDBusError::InvalidSignature, "org.freedesktop.DBus.Error.InvalidSignature\0" },
    { QDBusError::UnknownInterface, "org.freedesktop.DBus.Error.UnknownInterface\0" },
    { QDBusError::InternalError, "Katie.QtDBus.Error.InternalError\0" },
    { QDBusError::UnknownObject, "org.freedesktop.DBus.Error.UnknownObject\0" },
    { QDBusError::InvalidService, "Katie.QtDBus.Error.InvalidService\0" },
    { QDBusError::InvalidObjectPath, "Katie.QtDBus.Error.InvalidObjectPath\0" },
    { QDBusError::InvalidInterface, "Katie.QtDBus.Error.InvalidInterface\0" },
    { QDBusError::InvalidMember, "Katie.QtDBus.Error.InvalidMember\0" },
};
static const qint16 ErrorTblSize = sizeof(ErrorTbl) / sizeof(ErrorTblData);

static inline const char *getError(QDBusError::ErrorType code)
{
    for (qint16 i = 0; i < ErrorTblSize; i++) {
        if (ErrorTbl[i].code == code) {
            return ErrorTbl[i].name;
        }
    }
    return ErrorTbl[0].name;
}

static inline QDBusError::ErrorType getError(const char *name)
{
    if (!name || !*name)
        return QDBusError::NoError;
    for (qint16 i = 0; i < ErrorTblSize; i++) {
        if (strcmp(ErrorTbl[i].name, name) == 0) {
            return ErrorTbl[i].code;
        }
    }
    return QDBusError::Other;
}

/*!
    \class QDBusError
    \inmodule QtDBus
    \since 4.2

    \brief The QDBusError class represents an error received from the
    D-Bus bus or from remote applications found in the bus.

    When dealing with the D-Bus bus service or with remote
    applications over D-Bus, a number of error conditions can
    happen. This error conditions are sometimes signalled by a
    returned error value or by a QDBusError.

    C++ and Java exceptions are a valid analogy for D-Bus errors:
    instead of returning normally with a return value, remote
    applications and the bus may decide to throw an error
    condition. However, the QtDBus implementation does not use the C++
    exception-throwing mechanism, so you will receive QDBusErrors in
    the return reply (see QDBusReply::error()).

    QDBusError objects are used to inspect the error name and message
    as received from the bus and remote applications. You should not
    create such objects yourself to signal error conditions when
    called from D-Bus: instead, use QDBusMessage::createError() and
    QDBusConnection::send().

    \sa QDBusConnection::send(), QDBusMessage, QDBusReply
*/

/*!
    \enum QDBusError::ErrorType

    In order to facilitate verification of the most common D-Bus errors generated by the D-Bus
    implementation and by the bus daemon itself, QDBusError can be compared to a set of pre-defined
    values:

    \value NoError              QDBusError is invalid (i.e., the call succeeded)
    \value Other                QDBusError contains an error that is one of the well-known ones
    \value Failed               The call failed (\c org.freedesktop.DBus.Error.Failed)
    \value NoMemory             Out of memory (\c org.freedesktop.DBus.Error.NoMemory)
    \value ServiceUnknown       The called service is not known
                                (\c org.freedesktop.DBus.Error.ServiceUnknown)
    \value NoReply              The called method did not reply within the specified timeout
                                (\c org.freedesktop.DBus.Error.NoReply)
    \value BadAddress           The address given is not valid
                                (\c org.freedesktop.DBus.Error.BadAddress)
    \value NotSupported         The call/operation is not supported
                                (\c org.freedesktop.DBus.Error.NotSupported)
    \value LimitsExceeded       The limits allocated to this process/call/connection exceeded the
                                pre-defined values (\c org.freedesktop.DBus.Error.LimitsExceeded)
    \value AccessDenied         The call/operation tried to access a resource it isn't allowed to
                                (\c org.freedesktop.DBus.Error.AccessDenied)
    \value NoServer             \e {Documentation doesn't say what this is for}
                                (\c org.freedesktop.DBus.Error.NoServer)
    \value Timeout              \e {Documentation doesn't say what this is for or how it's used}
                                (\c org.freedesktop.DBus.Error.Timeout)
    \value NoNetwork            \e {Documentation doesn't say what this is for}
                                (\c org.freedesktop.DBus.Error.NoNetwork)
    \value AddressInUse         QDBusServer tried to bind to an address that is already in use
                                (\c org.freedesktop.DBus.Error.AddressInUse)
    \value Disconnected         The call/process/message was sent after QDBusConnection disconnected
                                (\c org.freedesktop.DBus.Error.Disconnected)
    \value InvalidArgs          The arguments passed to this call/operation are not valid
                                (\c org.freedesktop.DBus.Error.InvalidArgs)
    \value UnknownMethod        The method called was not found in this object/interface with the
                                given parameters (\c org.freedesktop.DBus.Error.UnknownMethod)
    \value TimedOut             \e {Documentation doesn't say...}
                                (\c org.freedesktop.DBus.Error.TimedOut)
    \value InvalidSignature     The type signature is not valid or compatible
                                (\c org.freedesktop.DBus.Error.InvalidSignature)
    \value UnknownInterface     The interface is not known
    \value InternalError        An internal error occurred
                                (\c Katie.QtDBus.Error.InternalError)

    \value InvalidObjectPath    The object path provided is invalid.

    \value InvalidService       The service requested is invalid.

    \value InvalidMember        The member is invalid.

    \value InvalidInterface     The interface is invalid.

    \value UnknownObject        The remote object could not be found.
*/

/*!
    \internal
    Constructs a QDBusError from a DBusError structure.
*/
QDBusError::QDBusError(const DBusError *error)
    : code(NoError)
{
    if (!error || !dbus_error_is_set(error))
        return;

    code = getError(error->name);
    msg = QString::fromUtf8(error->message);
    nm = QString::fromUtf8(error->name);
}

/*!
    \internal
    Constructs a QDBusError from a QDBusMessage.
*/
QDBusError::QDBusError(const QDBusMessage &qdmsg)
    : code(NoError)
{
    if (qdmsg.type() != QDBusMessage::ErrorMessage)
        return;

    code = getError(qdmsg.errorName().toUtf8().constData());
    nm = qdmsg.errorName();
    msg = qdmsg.errorMessage();
}

/*!
    \internal
    Constructs a QDBusError from a well-known error code
*/
QDBusError::QDBusError(ErrorType error, const QString &mess)
    : code(error)
{
    nm = QLatin1String(getError(error));
    msg = mess;
}

/*!
    \internal
    Constructs a QDBusError from another QDBusError object
*/
QDBusError::QDBusError(const QDBusError &other)
    : code(other.code), msg(other.msg), nm(other.nm)
{
}

/*!
  \internal
  Assignment operator
*/

QDBusError &QDBusError::operator=(const QDBusError &other)
{
    code = other.code;
    msg = other.msg;
    nm = other.nm;
    return *this;
}

/*!
    Returns this error's ErrorType.

    \sa ErrorType
*/

QDBusError::ErrorType QDBusError::type() const
{
    return code;
}

/*!
    Returns this error's name. Error names are similar to D-Bus Interface names, like
    \c org.freedesktop.DBus.InvalidArgs.

    \sa type()
*/

QString QDBusError::name() const
{
    return nm;
}

/*!
    Returns the message that the callee associated with this error. Error messages are
    implementation defined and usually contain a human-readable error code, though this does not
    mean it is suitable for your end-users.
*/

QString QDBusError::message() const
{
    return msg;
}

/*!
    Returns true if this is a valid error condition (i.e., if there was an error),
    otherwise false.
*/

bool QDBusError::isValid() const
{
    return (code != NoError);
}

/*!
    \since 4.3
    Returns the error name associated with error condition \a error.
*/
QString QDBusError::errorString(ErrorType error)
{
    return QLatin1String(getError(error));
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QDBusError &msg)
{
    dbg.nospace() << "QDBusError(" << msg.name() << ", " << msg.message() << ')';
    return dbg.space();
}
#endif

QT_END_NAMESPACE


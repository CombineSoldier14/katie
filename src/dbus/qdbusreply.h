/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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

#ifndef QDBUSREPLY_H
#define QDBUSREPLY_H

#include <QtDBus/qdbusmessage.h>
#include <QtDBus/qdbuserror.h>
#include <QtDBus/qdbusextratypes.h>
#include <QtDBus/qdbuspendingreply.h>



QT_BEGIN_NAMESPACE


Q_DBUS_EXPORT void qDBusReplyFill(const QDBusMessage &reply, QDBusError &error, QVariant &data);

template<typename T>
class Q_DBUS_EXPORT QDBusReply
{
    typedef T Type;
public:
    inline QDBusReply(const QDBusMessage &reply)
    {
        *this = reply;
    }
    inline QDBusReply& operator=(const QDBusMessage &reply)
    {
        QVariant data(qMetaTypeId<T>(), reinterpret_cast<void*>(0));
        qDBusReplyFill(reply, m_error, data);
        m_data = qvariant_cast<Type>(data);
        return *this;
    }

    inline QDBusReply(const QDBusPendingCall &pcall)
    {
        *this = pcall;
    }
    inline QDBusReply &operator=(const QDBusPendingCall &pcall)
    {
        QDBusPendingCall other(pcall);
        other.waitForFinished();
        return *this = other.reply();
    }
    inline QDBusReply(const QDBusPendingReply<T> &reply)
    {
        *this = static_cast<QDBusPendingCall>(reply);
    }

    inline QDBusReply(const QDBusError &dbusError = QDBusError())
        : m_error(dbusError), m_data(Type())
    {
    }
    inline QDBusReply& operator=(const QDBusError& dbusError)
    {
        m_error = dbusError;
        m_data = Type();
        return *this;
    }

    inline QDBusReply& operator=(const QDBusReply& other)
    {
        m_error = other.m_error;
        m_data = other.m_data;
        return *this;
    }

    inline bool isValid() const { return !m_error.isValid(); }

    inline const QDBusError& error() const { return m_error; }

    inline Type value() const
    {
        return m_data;
    }

    inline operator Type() const
    {
        return m_data;
    }

private:
    QDBusError m_error;
    Type m_data;
};

// specialize for QVariant:
template<> inline QDBusReply<QVariant>&
QDBusReply<QVariant>::operator=(const QDBusMessage &reply)
{
    void *null = Q_NULLPTR;
    QVariant data(qMetaTypeId<QDBusVariant>(), null);
    qDBusReplyFill(reply, m_error, data);
    m_data = qvariant_cast<QDBusVariant>(data).variant();
    return *this;
}

// specialize for void:
template<>
class QDBusReply<void>
{
public:
    inline QDBusReply(const QDBusMessage &reply)
        : m_error(reply)
    {
    }
    inline QDBusReply& operator=(const QDBusMessage &reply)
    {
        m_error = reply;
        return *this;
    }
    inline QDBusReply(const QDBusError &dbusError = QDBusError())
        : m_error(dbusError)
    {
    }
    inline QDBusReply(const QDBusPendingCall &pcall)
    {
        *this = pcall;
    }
    inline QDBusReply &operator=(const QDBusPendingCall &pcall)
    {
        QDBusPendingCall other(pcall);
        other.waitForFinished();
        return *this = other.reply();
    }
    inline QDBusReply& operator=(const QDBusError& dbusError)
    {
        m_error = dbusError;
        return *this;
    }

    inline QDBusReply& operator=(const QDBusReply& other)
    {
        m_error = other.m_error;
        return *this;
    }

    inline bool isValid() const { return !m_error.isValid(); }

    inline const QDBusError& error() const { return m_error; }

private:
    QDBusError m_error;
};

QT_END_NAMESPACE


#endif

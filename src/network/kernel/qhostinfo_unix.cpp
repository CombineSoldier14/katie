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

//#define QHOSTINFO_DEBUG

#include "qplatformdefs.h"

#include "qhostinfo_p.h"
#include "qnativesocketengine_p.h"
#include "qiodevice.h"
#include "qbytearray.h"
#include "qurl.h"
#include "qfile.h"
#include "qnet_unix_p.h"

#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

#ifndef QT_NO_RESOLV
#include <resolv.h>
#endif // QT_NO_RESOLV

QT_BEGIN_NAMESPACE

#if defined (QT_NO_GETADDRINFO)
#include "qmutex.h"
Q_GLOBAL_STATIC(QMutex, getHostByNameMutex)
#endif

// HP-UXi has a bug in getaddrinfo(3) that makes it thread-unsafe
// with this flag. So disable it in that platform.
#if defined(AI_ADDRCONFIG) && !defined(Q_OS_HPUX)
#  define Q_ADDRCONFIG          AI_ADDRCONFIG
#endif

QHostInfo QHostInfoAgent::fromName(const QString &hostName)
{
    QHostInfo results;

#if defined(QHOSTINFO_DEBUG)
    qDebug("QHostInfoAgent::fromName(%s) looking up...",
           hostName.toLatin1().constData());
#endif

#ifndef QT_NO_RESOLV
    // If res_init is available, poll it.
    res_init();
#endif // QT_NO_RESOLV

    QHostAddress address;
    if (address.setAddress(hostName)) {
        // Reverse lookup
#if !defined (QT_NO_GETADDRINFO)
        sockaddr_in sa4;
#ifndef QT_NO_IPV6
        sockaddr_in6 sa6;
#endif
        sockaddr *sa = 0;
        QT_SOCKLEN_T saSize = 0;
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            sa = (sockaddr *)&sa4;
            saSize = sizeof(sa4);
            memset(&sa4, 0, sizeof(sa4));
            sa4.sin_family = AF_INET;
            sa4.sin_addr.s_addr = htonl(address.toIPv4Address());
        }
#ifndef QT_NO_IPV6
        else {
            sa = (sockaddr *)&sa6;
            saSize = sizeof(sa6);
            memset(&sa6, 0, sizeof(sa6));
            sa6.sin6_family = AF_INET6;
            memcpy(sa6.sin6_addr.s6_addr, address.toIPv6Address().c, sizeof(sa6.sin6_addr.s6_addr));
        }
#endif

        char hbuf[NI_MAXHOST];
        int result = (sa ? ::getnameinfo(sa, saSize, hbuf, sizeof(hbuf), 0, 0, 0) : EAI_NONAME);
        if (result == 0) {
            results.setHostName(QString::fromLatin1(hbuf));
        } else if (result == EAI_NONAME || result == EAI_FAIL
#ifdef EAI_NODATA
               // EAI_NODATA is deprecated in RFC 3493
               || result == EAI_NODATA
#endif
               ) {
            results.setError(QHostInfo::HostNotFound);
            results.setErrorString(tr("Host not found"));
        } else {
            results.setError(QHostInfo::UnknownError);
            results.setErrorString(QString::fromLocal8Bit(gai_strerror(result)));
        }
#else
        in_addr_t inetaddr = ::inet_addr(hostName.toLatin1().constData());
        struct hostent *ent = gethostbyaddr((const char *)&inetaddr, sizeof(inetaddr), AF_INET);
        if (ent) {
            results.setHostName(QString::fromLatin1(ent->h_name));
        } else if (h_errno == HOST_NOT_FOUND || h_errno == NO_DATA || h_errno == NO_ADDRESS) {
            results.setError(QHostInfo::HostNotFound);
            results.setErrorString(tr("Host not found"));
        } else {
            results.setError(QHostInfo::UnknownError);
            results.setErrorString(tr("Unknown error"));
        }
#endif

        if (results.hostName().isEmpty())
            results.setHostName(address.toString());
        results.setAddresses(QList<QHostAddress>() << address);
        return results;
    }

    // IDN support
    QByteArray aceHostname = QUrl::toAce(hostName);
    results.setHostName(hostName);
    if (aceHostname.isEmpty()) {
        results.setError(QHostInfo::HostNotFound);
        results.setErrorString(hostName.isEmpty() ?
                               QCoreApplication::translate("QHostInfoAgent", "No host name given") :
                               QCoreApplication::translate("QHostInfoAgent", "Invalid hostname"));
        return results;
    }

#if !defined (QT_NO_GETADDRINFO)
    // Call getaddrinfo, and place all IPv4 addresses at the start and
    // the IPv6 addresses at the end of the address list in results.
    addrinfo *res = 0;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
#ifdef Q_ADDRCONFIG
    hints.ai_flags = Q_ADDRCONFIG;
#endif

    int result = getaddrinfo(aceHostname.constData(), 0, &hints, &res);
# ifdef Q_ADDRCONFIG
    if (result == EAI_BADFLAGS) {
        // if the lookup failed with AI_ADDRCONFIG set, try again without it
        hints.ai_flags = 0;
        result = getaddrinfo(aceHostname.constData(), 0, &hints, &res);
    }
# endif

    if (result == 0) {
        addrinfo *node = res;
        QList<QHostAddress> addresses;
        while (node) {
#ifdef QHOSTINFO_DEBUG
                qDebug() << "getaddrinfo node: flags:" << node->ai_flags << "family:" << node->ai_family << "ai_socktype:" << node->ai_socktype << "ai_protocol:" << node->ai_protocol << "ai_addrlen:" << node->ai_addrlen;
#endif
            if (node->ai_family == AF_INET) {
                QHostAddress addr;
                addr.setAddress(ntohl(((sockaddr_in *) node->ai_addr)->sin_addr.s_addr));
                if (!addresses.contains(addr))
                    addresses.append(addr);
            }
#ifndef QT_NO_IPV6
            else if (node->ai_family == AF_INET6) {
                QHostAddress addr;
                sockaddr_in6 *sa6 = (sockaddr_in6 *) node->ai_addr;
                addr.setAddress(sa6->sin6_addr.s6_addr);
                if (sa6->sin6_scope_id)
                    addr.setScopeId(QString::number(sa6->sin6_scope_id));
                if (!addresses.contains(addr))
                    addresses.append(addr);
            }
#endif
            node = node->ai_next;
        }
        if (addresses.isEmpty() && node == 0) {
            // Reached the end of the list, but no addresses were found; this
            // means the list contains one or more unknown address types.
            results.setError(QHostInfo::UnknownError);
            results.setErrorString(tr("Unknown address type"));
        }

        results.setAddresses(addresses);
        freeaddrinfo(res);
    } else if (result == EAI_NONAME || result ==  EAI_FAIL
#ifdef EAI_NODATA
                // EAI_NODATA is deprecated in RFC 3493
                || result == EAI_NODATA
#endif
                ) {
        results.setError(QHostInfo::HostNotFound);
        results.setErrorString(tr("Host not found"));
    } else {
        results.setError(QHostInfo::UnknownError);
        results.setErrorString(QString::fromLocal8Bit(gai_strerror(result)));
    }

#else
    // Fall back to gethostbyname for platforms that don't define
    // getaddrinfo. gethostbyname does not support IPv6, and it's not
    // reentrant on all platforms. For now this is okay since we only
    // use one QHostInfoAgent, but if more agents are introduced, locking
    // must be provided.
    QMutexLocker locker(getHostByNameMutex());
    hostent *result = gethostbyname(aceHostname.constData());
    if (result) {
        if (result->h_addrtype == AF_INET) {
            QList<QHostAddress> addresses;
            for (char **p = result->h_addr_list; *p != 0; p++) {
                QHostAddress addr;
                addr.setAddress(ntohl(*((quint32 *)*p)));
                if (!addresses.contains(addr))
                    addresses.prepend(addr);
            }
            results.setAddresses(addresses);
        } else {
            results.setError(QHostInfo::UnknownError);
            results.setErrorString(tr("Unknown address type"));
        }
    } else if (h_errno == HOST_NOT_FOUND || h_errno == NO_DATA
               || h_errno == NO_ADDRESS) {
        results.setError(QHostInfo::HostNotFound);
        results.setErrorString(tr("Host not found"));
    } else {
        results.setError(QHostInfo::UnknownError);
        results.setErrorString(tr("Unknown error"));
    }
#endif //  !defined (QT_NO_GETADDRINFO)

#if defined(QHOSTINFO_DEBUG)
    if (results.error() != QHostInfo::NoError) {
        qDebug("QHostInfoAgent::fromName(): error #%d %s",
               h_errno, results.errorString().toLatin1().constData());
    } else {
        QString tmp;
        QList<QHostAddress> addresses = results.addresses();
        for (int i = 0; i < addresses.count(); ++i) {
            if (i != 0) tmp += ", ";
            tmp += addresses.at(i).toString();
        }
        qDebug("QHostInfoAgent::fromName(): found %i entries for \"%s\": {%s}",
               addresses.count(), hostName.toLatin1().constData(),
               tmp.toLatin1().constData());
    }
#endif
    return results;
}

QString QHostInfo::localHostName()
{
    int size_max = sysconf(_SC_HOST_NAME_MAX);
    if (size_max == -1)
        size_max = _POSIX_HOST_NAME_MAX;
    char gethostbuffer[size_max];
    if (Q_LIKELY(::gethostname(gethostbuffer, size_max) == 0)) {
        gethostbuffer[size_max - 1] = '\0';
        return QString::fromLocal8Bit(gethostbuffer);
    }
    return QString();
}

QString QHostInfo::localDomainName()
{
// a dirty way to support both thread-safe/unsafe
#if !defined(QT_NO_RESOLV) && defined(res_ninit)
    // using thread-safe version
    struct __res_state state;
    res_ninit(&state);
    QString domainName = QUrl::fromAce(state.defdname);
    if (domainName.isEmpty())
        domainName = QUrl::fromAce(state.dnsrch[0]);
    res_nclose(&state);

    return domainName;
#elif !defined(QT_NO_RESOLV)
    // using thread-unsafe version

#if defined(QT_NO_GETADDRINFO)
    // We have to call res_init to be sure that _res was initialized
    // So, for systems without getaddrinfo (which is thread-safe), we lock the mutex too
    QMutexLocker locker(getHostByNameMutex());
#endif
    res_init();
    QString domainName = QUrl::fromAce(_res.defdname);
    if (domainName.isEmpty())
        domainName = QUrl::fromAce(_res.dnsrch[0]);
    return domainName;
#else

    // nothing worked, try doing it by ourselves:
#if defined(_PATH_RESCONF)
    QFile resolvconf(QFile::decodeName(_PATH_RESCONF));
#else
    QFile resolvconf(QLatin1String("/etc/resolv.conf"));
#endif
    if (!resolvconf.open(QIODevice::ReadOnly))
        return QString();       // failure

    QString domainName;
    while (!resolvconf.atEnd()) {
        QByteArray line = resolvconf.readLine().trimmed();
        if (line.startsWith("domain "))
            return QUrl::fromAce(line.mid(sizeof "domain " - 1).trimmed());

        // in case there's no "domain" line, fall back to the first "search" entry
        if (domainName.isEmpty() && line.startsWith("search ")) {
            QByteArray searchDomain = line.mid(sizeof "search " - 1).trimmed();
            int pos = searchDomain.indexOf(' ');
            if (pos != -1)
                searchDomain.truncate(pos);
            domainName = QUrl::fromAce(searchDomain);
        }
    }

    // return the fallen-back-to searched domain
    return domainName;
#endif // QT_NO_RESOLV
}

QT_END_NAMESPACE



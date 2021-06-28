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


#include "qsslkey.h"

QT_BEGIN_NAMESPACE

/*! \namespace QSsl

    \brief The QSsl namespace declares enums common to all SSL classes in QtNetwork.
    \since 4.3

    \ingroup network
    \ingroup ssl
    \inmodule QtNetwork
*/

/*!
    \enum QSsl::KeyType

    Describes the two types of keys QSslKey supports.
    
    \value PrivateKey A private key.
    \value PublicKey A public key.
*/

/*!
    \enum QSsl::KeyAlgorithm

    Describes the different key algorithms supported by QSslKey.

    \value Rsa The RSA algorithm.
    \value Dsa The DSA algorithm.
*/

/*!
    \enum QSsl::EncodingFormat

    Describes supported encoding formats for certificates and keys.

    \value Pem The PEM format.
    \value Der The DER format.
*/

/*!
    \enum QSsl::AlternateNameEntryType

    Describes the key types for alternate name entries in QSslCertificate.

    \value EmailEntry An email entry; the entry contains an email address that
    the certificate is valid for.

    \value DnsEntry A DNS host name entry; the entry contains a host name
    entry that the certificate is valid for. The entry may contain wildcards.

    \sa QSslCertificate::alternateSubjectNames()

*/

/*!
    \enum QSsl::SslProtocol

    Describes the protocol of the cipher.

    \value SslV3 SSLv3
    \value TlsV1 TLSv1
    \value UnknownProtocol The cipher's protocol cannot be determined.
    \value AnyProtocol The socket understands SSLv2, SSLv3, and TLSv1. This
    value is used by QSslSocket only.
    \value SecureProtocols The default option, using protocols known to be secure;
    currently behaves like AnyProtocol.

    Note: most servers using SSL understand both versions (2 and 3),
    but it is recommended to use the latest version only for security
    reasons. However, SSL and TLS are not compatible with each other:
    if you get unexpected handshake failures, verify that you chose
    the correct setting for your protocol.
*/

/*!
    \enum QSsl::SslOption

    Describes the options that can be used to control the details of
    SSL behaviour. These options are generally used to turn features off
    to work around buggy servers.

    \value SslOptionDisableEmptyFragments Disables the insertion of empty
    fragments into the data when using block ciphers. When enabled, this
    prevents some attacks (such as the BEAST attack), however it is
    incompatible with some servers.
    \value SslOptionDisableSessionTickets Disables the SSL session ticket
    extension. This can cause slower connection setup, however some servers
    are not compatible with the extension.
    \value SslOptionDisableCompression Disables the SSL compression
    extension. When enabled, this allows the data being passed over SSL to
    be compressed, however some servers are not compatible with this
    extension.
    \value SslOptionDisableServerNameIndication Disables the SSL server
    name indication extension. When enabled, this tells the server the virtual
    host being accessed allowing it to respond with the correct certificate.
    \value SslOptionDisableLegacyRenegotiation Disables the older insecure
    mechanism for renegotiating the connection parameters. When enabled, this
    option can allow connections for legacy servers, but it introduces the
    possibility that an attacker could inject plaintext into the SSL session.

    By default, SslOptionDisableEmptyFragments is turned on since this causes
    problems with a large number of servers. SslOptionDisableLegacyRenegotiation
    is also turned on, since it introduces a security risk.
    SslOptionDisableCompression is turned on to prevent the attack publicised by
    CRIME. The other options are turned off.

    Note: Availability of above options depends on the version of the SSL
    backend in use.
*/


QT_END_NAMESPACE



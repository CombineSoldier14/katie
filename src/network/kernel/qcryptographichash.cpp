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

#include "qplatformdefs.h"
#include "qcryptographichash.h"
#include "qiodevice.h"
#include "qcorecommon_p.h"

#include "md5.h"
#include "sha1.h"
#include "sha2.h"

QT_BEGIN_NAMESPACE

class QCryptographicHashPrivate
{
public:
    MD5_CTX md5Context;
    SHA1_CTX sha1Context;
    SHA256_CTX sha256Context;
    SHA512_CTX sha512Context;
    QCryptographicHash::Algorithm method;
    QByteArray result;
};

/*!
  \class QCryptographicHash
  \inmodule QtCore

  \brief The QCryptographicHash class provides a way to generate cryptographic hashes.

  \since 4.3

  \ingroup tools
  \reentrant

  QCryptographicHash can be used to generate cryptographic hashes of binary or text data.

  Currently MD5, SHA-1, SHA-256 and SHA-512 are supported.
*/

/*!
  \enum QCryptographicHash::Algorithm

  \value Md5 Generate an MD5 hash sum
  \value Sha1 Generate an SHA-1 hash sum
  \value Sha256 Generate an SHA-256 hash sum (SHA-2). Introduced in Katie 4.9
  \value Sha512 Generate an SHA-512 hash sum (SHA-2). Introduced in Katie 4.9
*/

/*!
  Constructs an object that can be used to create a cryptographic hash from data using \a method.
*/
QCryptographicHash::QCryptographicHash(QCryptographicHash::Algorithm method)
    : d(new QCryptographicHashPrivate)
{
    d->method = method;
    reset();
}

/*!
  Destroys the object.
*/
QCryptographicHash::~QCryptographicHash()
{
    delete d;
}

/*!
  Resets the object.
*/
void QCryptographicHash::reset()
{
    switch (d->method) {
        case QCryptographicHash::Md5: {
            MD5Init(&d->md5Context);
            break;
        }
        case QCryptographicHash::Sha1: {
            SHA1Init(&d->sha1Context);
            break;
        }
        case QCryptographicHash::Sha256: {
            SHA256_Init(&d->sha256Context);
            break;
        }
        case QCryptographicHash::Sha512: {
            SHA512_Init(&d->sha512Context);
            break;
        }
    }
    d->result.clear();
}

/*!
    Adds the first \a length chars of \a data to the cryptographic
    hash.
*/
void QCryptographicHash::addData(const char *data, int length)
{
    switch (d->method) {
        case QCryptographicHash::Md5: {
            MD5Update(&d->md5Context, reinterpret_cast<const uchar*>(data), length);
            break;
        }
        case QCryptographicHash::Sha1: {
            SHA1Update(&d->sha1Context, reinterpret_cast<const uchar*>(data), length);
            break;
        }
        case QCryptographicHash::Sha256: {
            SHA256_Update(&d->sha256Context, reinterpret_cast<const uchar*>(data), length);
            break;
        }
        case QCryptographicHash::Sha512: {
            SHA512_Update(&d->sha512Context, reinterpret_cast<const uchar*>(data), length);
            break;
        }
    }
    d->result.clear();
}

/*!
  Reads the data from the open QIODevice \a device until it ends
  and hashes it. Returns \c true if reading was successful.
  \since 4.9
 */
bool QCryptographicHash::addData(QIODevice* device)
{
    if (!device->isReadable())
        return false;

    if (!device->isOpen())
        return false;

    QSTACKARRAY(char, buffer, QT_BUFFSIZE);
    int length;
    while ((length = device->read(buffer, QT_BUFFSIZE)) > 0) {
        addData(buffer, length);
    }

    return device->atEnd();
}


/*!
  Returns the final hash value.

  \sa QByteArray::toHex()
*/
QByteArray QCryptographicHash::result() const
{
    if (!d->result.isEmpty())
        return d->result;

    switch (d->method) {
        case QCryptographicHash::Md5: {
            MD5_CTX copy = d->md5Context;
            d->result.resize(MD5_DIGEST_LENGTH);
            MD5Final(reinterpret_cast<unsigned char *>(d->result.data()), &copy);
            break;
        }
        case QCryptographicHash::Sha1: {
            SHA1_CTX copy = d->sha1Context;
            d->result.resize(SHA_DIGEST_LENGTH);
            SHA1Final(reinterpret_cast<unsigned char *>(d->result.data()), &copy);
            break;
        }
        case QCryptographicHash::Sha256:{
            SHA256_CTX copy = d->sha256Context;
            d->result.resize(SHA256_DIGEST_LENGTH);
            SHA256_Final(reinterpret_cast<unsigned char *>(d->result.data()), &copy);
            break;
        }
        case QCryptographicHash::Sha512:{
            SHA512_CTX copy = d->sha512Context;
            d->result.resize(SHA512_DIGEST_LENGTH);
            SHA512_Final(reinterpret_cast<unsigned char *>(d->result.data()), &copy);
            break;
        }
    }
    return d->result;
}

/*!
  Returns the hash of \a data using \a method.
*/
QByteArray QCryptographicHash::hash(const QByteArray &data, QCryptographicHash::Algorithm method)
{
    switch (method) {
        case QCryptographicHash::Md5: {
            QSTACKARRAY(unsigned char, result, MD5_DIGEST_LENGTH);
            MD5_CTX md5Context;
            MD5Init(&md5Context);
            MD5Update(&md5Context, reinterpret_cast<const uchar*>(data.constData()), data.length());
            MD5Final(result, &md5Context);
            return QByteArray(reinterpret_cast<char *>(result), MD5_DIGEST_LENGTH);
        }
        case QCryptographicHash::Sha1: {
            QSTACKARRAY(unsigned char, result, SHA_DIGEST_LENGTH);
            SHA1_CTX sha1Context;
            SHA1Init(&sha1Context);
            SHA1Update(&sha1Context, reinterpret_cast<const uchar*>(data.constData()), data.length());
            SHA1Final(result, &sha1Context);
            return QByteArray(reinterpret_cast<char *>(result), SHA_DIGEST_LENGTH);
        }
        case QCryptographicHash::Sha256: {
            QSTACKARRAY(unsigned char, result, SHA256_DIGEST_LENGTH);
            SHA256_CTX sha256Context;
            SHA256_Init(&sha256Context);
            SHA256_Update(&sha256Context, reinterpret_cast<const uchar*>(data.constData()), data.length());
            SHA256_Final(result, &sha256Context);
            return QByteArray(reinterpret_cast<char *>(result), SHA256_DIGEST_LENGTH);
        }
        case QCryptographicHash::Sha512: {
            QSTACKARRAY(unsigned char, result, SHA512_DIGEST_LENGTH);
            SHA512_CTX sha512Context;
            SHA512_Init(&sha512Context);
            SHA512_Update(&sha512Context, reinterpret_cast<const uchar*>(data.constData()), data.length());
            SHA512_Final(result, &sha512Context);
            return QByteArray(reinterpret_cast<char *>(result), SHA512_DIGEST_LENGTH);
        }
    }

    Q_UNREACHABLE();
    return QByteArray();
}

QT_END_NAMESPACE

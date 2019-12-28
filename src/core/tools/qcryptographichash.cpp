/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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

#include "qcryptographichash.h"
#include "qiodevice.h"

#include <openssl/md5.h>
#include <openssl/md4.h>
#include <openssl/sha.h>

QT_BEGIN_NAMESPACE

class QCryptographicHashPrivate
{
public:
    MD4_CTX md4Context;
    MD5_CTX md5Context;
    SHA_CTX sha1Context;
    SHA256_CTX sha224Context;
    SHA256_CTX sha256Context;
    SHA512_CTX sha384Context;
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

  Currently MD4, MD5, SHA-1, SHA-224, SHA-256, SHA-384, and SHA-512 are supported.
*/

/*!
  \enum QCryptographicHash::Algorithm

  \value Md4 Generate an MD4 hash sum
  \value Md5 Generate an MD5 hash sum
  \value Sha1 Generate an SHA-1 hash sum
  \value Sha224 Generate an SHA-224 hash sum (SHA-2). Introduced in Katie 4.9
  \value Sha256 Generate an SHA-256 hash sum (SHA-2). Introduced in Katie 4.9
  \value Sha384 Generate an SHA-384 hash sum (SHA-2). Introduced in Katie 4.9
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
        case QCryptographicHash::Md4: {
            MD4_Init(&d->md4Context);
            break;
        }
        case QCryptographicHash::Md5: {
            MD5_Init(&d->md5Context);
            break;
        }
        case QCryptographicHash::Sha1: {
            SHA1_Init(&d->sha1Context);
            break;
        }
        case QCryptographicHash::Sha224: {
            SHA224_Init(&d->sha224Context);
            break;
        }
        case QCryptographicHash::Sha256: {
            SHA256_Init(&d->sha256Context);
            break;
        }
        case QCryptographicHash::Sha384: {
            SHA384_Init(&d->sha384Context);
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
        case QCryptographicHash::Md4: {
            MD4_Update(&d->md4Context, data, length);
            break;
        }
        case QCryptographicHash::Md5: {
            MD5_Update(&d->md5Context, data, length);
            break;
        }
        case QCryptographicHash::Sha1: {
            SHA1_Update(&d->sha1Context, data, length);
            break;
        }
        case QCryptographicHash::Sha224: {
            SHA224_Update(&d->sha224Context, data, length);
            break;
        }
        case QCryptographicHash::Sha256: {
            SHA256_Update(&d->sha256Context, data, length);
            break;
        }
        case QCryptographicHash::Sha384: {
            SHA384_Update(&d->sha384Context, data, length);
            break;
        }
        case QCryptographicHash::Sha512: {
            SHA512_Update(&d->sha512Context, data, length);
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

    char buffer[1024];
    int length;

    while ((length = device->read(buffer,sizeof(buffer))) > 0)
        addData(buffer,length);

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
        case QCryptographicHash::Md4: {
            d->result.resize(MD4_DIGEST_LENGTH);
            MD4_Final(reinterpret_cast<unsigned char *>(d->result.data()), &d->md4Context);
            break;
        }
        case QCryptographicHash::Md5: {
            d->result.resize(MD5_DIGEST_LENGTH);
            MD5_Final(reinterpret_cast<unsigned char *>(d->result.data()), &d->md5Context);
            break;
        }
        case QCryptographicHash::Sha1: {
            d->result.resize(SHA_DIGEST_LENGTH);
            SHA1_Final(reinterpret_cast<unsigned char *>(d->result.data()), &d->sha1Context);
            break;
        }
        case QCryptographicHash::Sha224: {
            d->result.resize(SHA224_DIGEST_LENGTH);
            SHA224_Final(reinterpret_cast<unsigned char *>(d->result.data()), &d->sha224Context);
            break;
        }
        case QCryptographicHash::Sha256:{
            d->result.resize(SHA256_DIGEST_LENGTH);
            SHA256_Final(reinterpret_cast<unsigned char *>(d->result.data()), &d->sha256Context);
            break;
        }
        case QCryptographicHash::Sha384:{
            d->result.resize(SHA384_DIGEST_LENGTH);
            SHA384_Final(reinterpret_cast<unsigned char *>(d->result.data()), &d->sha384Context);
            break;
        }
        case QCryptographicHash::Sha512:{
            d->result.resize(SHA512_DIGEST_LENGTH);
            SHA512_Final(reinterpret_cast<unsigned char *>(d->result.data()), &d->sha512Context);
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
        case QCryptographicHash::Md4: {
            QByteArray result(MD4_DIGEST_LENGTH, Qt::Uninitialized);
            MD4_CTX md4Context;
            MD4_Init(&md4Context);
            MD4_Update(&md4Context, data.constData(), data.length());
            MD4_Final(reinterpret_cast<unsigned char *>(result.data()), &md4Context);
            return result;
        }
        case QCryptographicHash::Md5: {
            QByteArray result(MD5_DIGEST_LENGTH, Qt::Uninitialized);
            MD5_CTX md5Context;
            MD5_Init(&md5Context);
            MD5_Update(&md5Context, data.constData(), data.length());
            MD5_Final(reinterpret_cast<unsigned char *>(result.data()), &md5Context);
            return result;
        }
        case QCryptographicHash::Sha1: {
            QByteArray result(SHA_DIGEST_LENGTH, Qt::Uninitialized);
            SHA_CTX sha1Context;
            SHA1_Init(&sha1Context);
            SHA1_Update(&sha1Context, data.constData(), data.length());
            SHA1_Final(reinterpret_cast<unsigned char *>(result.data()), &sha1Context);
            return result;
        }
        case QCryptographicHash::Sha224: {
            QByteArray result(SHA224_DIGEST_LENGTH, Qt::Uninitialized);
            SHA256_CTX sha224Context;
            SHA224_Init(&sha224Context);
            SHA224_Update(&sha224Context, data.constData(), data.length());
            SHA224_Final(reinterpret_cast<unsigned char *>(result.data()), &sha224Context);
            return result;
        }
        case QCryptographicHash::Sha256: {
            QByteArray result(SHA256_DIGEST_LENGTH, Qt::Uninitialized);
            SHA256_CTX sha256Context;
            SHA256_Init(&sha256Context);
            SHA256_Update(&sha256Context, data.constData(), data.length());
            SHA256_Final(reinterpret_cast<unsigned char *>(result.data()), &sha256Context);
            return result;
        }
        case QCryptographicHash::Sha384: {
            QByteArray result(SHA384_DIGEST_LENGTH, Qt::Uninitialized);
            SHA512_CTX sha384Context;
            SHA384_Init(&sha384Context);
            SHA384_Update(&sha384Context, data.constData(), data.length());
            SHA384_Final(reinterpret_cast<unsigned char *>(result.data()), &sha384Context);
            return result;
        }
        case QCryptographicHash::Sha512: {
            QByteArray result(SHA512_DIGEST_LENGTH, Qt::Uninitialized);
            SHA512_CTX sha512Context;
            SHA512_Init(&sha512Context);
            SHA512_Update(&sha512Context, data.constData(), data.length());
            SHA512_Final(reinterpret_cast<unsigned char *>(result.data()), &sha512Context);
            return result;
        }
    }

    Q_UNREACHABLE();
    return QByteArray();
}

QT_END_NAMESPACE

/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#ifndef QFSFILEENGINE_H
#define QFSFILEENGINE_H

#include <QtCore/qabstractfileengine.h>

#ifndef QT_NO_FSFILEENGINE

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QFSFileEnginePrivate;

class Q_CORE_EXPORT QFSFileEngine : public QAbstractFileEngine
{
    Q_DECLARE_PRIVATE(QFSFileEngine)
public:
    QFSFileEngine();
    explicit QFSFileEngine(const QString &file);
    ~QFSFileEngine();

    bool open(QIODevice::OpenMode openMode);
    bool open(QIODevice::OpenMode flags, FILE *fh);
    bool close();
    bool flush();
    qint64 size() const;
    qint64 pos() const;
    bool seek(qint64);
    bool isSequential() const;
    bool remove();
    bool copy(const QString &newName);
    bool rename(const QString &newName);
    bool link(const QString &newName);
    bool mkdir(const QString &dirName, bool createParentDirectories) const;
    bool rmdir(const QString &dirName, bool recurseParentDirectories) const;
    bool setSize(qint64 size);
    bool caseSensitive() const;
    bool isRelativePath() const;
    FileFlags fileFlags(FileFlags type) const;
    bool setPermissions(uint perms);
    QString fileName(FileName file) const;
    uint ownerId(FileOwner) const;
    QString owner(FileOwner) const;
    QDateTime fileTime(FileTime time) const;
    void setFileName(const QString &file);
    int handle() const;

#ifndef QT_NO_FILESYSTEMITERATOR
    Iterator *beginEntryList(QDir::Filters filters, const QStringList &filterNames);
#endif

    qint64 read(char *data, qint64 maxlen);
    qint64 readLine(char *data, qint64 maxlen);
    qint64 write(const char *data, qint64 len);

    bool extension(Extension extension, const ExtensionOption *option = Q_NULLPTR, ExtensionReturn *output = Q_NULLPTR);
    bool supportsExtension(Extension extension) const;

    //FS only!!
    bool open(QIODevice::OpenMode flags, int fd);
    bool open(QIODevice::OpenMode flags, int fd, QFile::FileHandleFlags handleFlags);
    bool open(QIODevice::OpenMode flags, FILE *fh, QFile::FileHandleFlags handleFlags);
    static bool setCurrentPath(const QString &path);
    static QString currentPath(const QString &path = QString());
    static QString homePath();
    static QString rootPath();
    static QString tempPath();

protected:
    QFSFileEngine(QFSFileEnginePrivate &dd);
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QT_NO_FSFILEENGINE

#endif // QFSFILEENGINE_H

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

#ifndef QFILESYSTEMENGINE_P_H_INCLUDED
#define QFILESYSTEMENGINE_P_H_INCLUDED

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

#include "qfile.h"
#include "qfilesystementry_p.h"
#include "qfilesystemmetadata_p.h"
#include "qsystemerror_p.h"

QT_BEGIN_NAMESPACE

class QFileSystemEngine
{
public:
    static bool isCaseSensitive();

    static QFileSystemEntry getLinkTarget(const QFileSystemEntry &link, QFileSystemMetaData &data);
    static QFileSystemEntry canonicalName(const QFileSystemEntry &entry, QFileSystemMetaData &data);
    static QFileSystemEntry absoluteName(const QFileSystemEntry &entry);
    static QString resolveUserName(const QFileSystemEntry &entry, QFileSystemMetaData &data);
    static QString resolveGroupName(const QFileSystemEntry &entry, QFileSystemMetaData &data);
    static QString resolveUserName(uint userId);
    static QString resolveGroupName(uint groupId);

    static bool fillMetaData(const QFileSystemEntry &entry, QFileSystemMetaData &data,
                             QFileSystemMetaData::MetaDataFlags what);
    static bool fillMetaData(int fd, QFileSystemMetaData &data); // what = PosixStatFlags
    //homePath, rootPath and tempPath shall return clean paths
    static QString homePath();
    static QString rootPath();
    static QString tempPath();

    static bool createDirectory(const QFileSystemEntry &entry, bool createParents);
    static bool removeDirectory(const QFileSystemEntry &entry, bool removeEmptyParents);

    static bool createLink(const QFileSystemEntry &source, const QFileSystemEntry &target, QSystemError &error);

    static bool copyFile(const QFileSystemEntry &source, const QFileSystemEntry &target, QSystemError &error);
    static bool renameFile(const QFileSystemEntry &source, const QFileSystemEntry &target, QSystemError &error);
    static bool removeFile(const QFileSystemEntry &entry, QSystemError &error);

    static bool setPermissions(const QFileSystemEntry &entry, QFile::Permissions permissions, QSystemError &error);

    static bool setCurrentPath(const QFileSystemEntry &entry);
    static QFileSystemEntry currentPath();
};

QT_END_NAMESPACE

#endif // include guard

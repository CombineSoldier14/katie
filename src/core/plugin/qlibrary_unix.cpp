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

#include "qplatformdefs.h"

#include "qfile.h"
#include "qstringlist.h"
#include "qlibrary_p.h"
#include "qcoreapplication.h"

#ifndef QT_NO_LIBRARY

#if defined (Q_OS_NACL)
#define QT_NO_DYNAMIC_LIBRARY
#endif

#if !defined(QT_NO_DYNAMIC_LIBRARY)
#include <dlfcn.h>
#endif

QT_BEGIN_NAMESPACE

static inline QString qdlerror()
{
#if defined(QT_NO_DYNAMIC_LIBRARY)
    return QLatin1String("This platform does not support dynamic libraries.");
#else
    return QString::fromLatin1(::dlerror());
#endif
}

bool QLibraryPrivate::load_sys()
{
#if !defined(QT_NO_DYNAMIC_LIBRARY)
    int dlFlags = 0;
    if (loadHints & QLibrary::ResolveAllSymbolsHint) {
        dlFlags |= RTLD_NOW;
    } else {
        dlFlags |= RTLD_LAZY;
    }
    if (loadHints & QLibrary::ExportExternalSymbolsHint) {
        dlFlags |= RTLD_GLOBAL;
    } else {
        dlFlags |= RTLD_LOCAL;
    }

    pHnd = ::dlopen(QFile::encodeName(fileName).constData(), dlFlags);
#endif // QT_NO_DYNAMIC_LIBRARY
    if (!pHnd) {
        errorString = QLibrary::tr("Cannot load library %1: %2").arg(fileName).arg(qdlerror());
    } else {
        qualifiedFileName = fileName;
        errorString.clear();
    }
    return (pHnd != 0);
}

bool QLibraryPrivate::unload_sys()
{
#if !defined(QT_NO_DYNAMIC_LIBRARY)
    if (::dlclose(pHnd)) {
        errorString = QLibrary::tr("Cannot unload library %1: %2").arg(fileName).arg(qdlerror());
        return false;
    }
#endif
    errorString.clear();
    return true;
}

void* QLibraryPrivate::resolve_sys(const char* symbol)
{
#if defined (QT_NO_DYNAMIC_LIBRARY)
    void *address = Q_NULLPTR;
#else
    void* address = ::dlsym(pHnd, symbol);
#endif
    if (!address) {
        errorString = QLibrary::tr("Cannot resolve symbol \"%1\" in %2: %3").arg(
            QString::fromAscii(symbol)).arg(fileName).arg(qdlerror());
    } else {
        errorString.clear();
    }
    return address;
}

QT_END_NAMESPACE

#endif // QT_NO_LIBRARY

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

#ifndef QPLUGIN_H
#define QPLUGIN_H

#include <QtCore/qpointer.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


#ifndef Q_EXTERN_C
#  ifdef __cplusplus
#    define Q_EXTERN_C extern "C"
#  else
#    define Q_EXTERN_C extern
#  endif
#endif

typedef QObject *(*QtPluginInstanceFunction)();

#define Q_PLUGIN_INSTANCE(IMPLEMENTATION) \
        { \
            static QT_PREPEND_NAMESPACE(QPointer)<QT_PREPEND_NAMESPACE(QObject)> _instance; \
            if (!_instance)      \
                _instance = new IMPLEMENTATION; \
            return _instance; \
        }

#define Q_EXPORT_PLUGIN(PLUGIN) \
          Q_EXPORT_PLUGIN2(PLUGIN, PLUGIN)

// NOTE: if you change pattern, you MUST change the pattern in
// qlibrary.cpp as well.  changing the pattern will break all
// backwards compatibility as well (no old plugins will be loaded).
// QT5: should probably remove the entire pattern thing and do the section
//      trick for all platforms. for now, keep it and fallback to scan for it.
#ifdef QT_NO_DEBUG
#  define QPLUGIN_DEBUG_STR "false"
#  define QPLUGIN_SECTION_DEBUG_STR ""
#else
#  define QPLUGIN_DEBUG_STR "true"
#  define QPLUGIN_SECTION_DEBUG_STR ".debug"
#endif

#define Q_PLUGIN_VERIFICATION_DATA \
  static const char kt_plugin_verification_data[] = \
    "pattern=KT_PLUGIN_VERIFICATION_DATA\n" \
    "version=" QT_VERSION_HEX_STR "\n" \
    "debug=" QPLUGIN_DEBUG_STR;

#if defined(Q_OF_ELF) && (defined(Q_CC_GNU) || defined(Q_CC_CLANG))
#  define Q_PLUGIN_VERIFICATION_SECTION \
     __attribute__ ((section (".ktplugin"))) __attribute__((used))
#else
#  define Q_PLUGIN_VERIFICATION_SECTION
#endif

#define Q_EXPORT_PLUGIN2(PLUGIN, PLUGINCLASS) \
          Q_PLUGIN_VERIFICATION_SECTION Q_PLUGIN_VERIFICATION_DATA \
          Q_EXTERN_C Q_DECL_EXPORT \
          const char * kt_plugin_query_verification_data() \
          { return kt_plugin_verification_data; } \
          Q_EXTERN_C Q_DECL_EXPORT QT_PREPEND_NAMESPACE(QObject) * kt_plugin_instance() \
          Q_PLUGIN_INSTANCE(PLUGINCLASS)

QT_END_NAMESPACE

QT_END_HEADER

#endif // Q_PLUGIN_H

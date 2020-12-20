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

#define Q_EXTERN_C extern "C"

typedef QObject *(*QtPluginInstanceFunction)();

#define Q_EXPORT_PLUGIN(PLUGIN) \
          Q_EXPORT_PLUGIN2(PLUGIN, PLUGIN)

#if !defined(QT_NO_PLUGIN_CHECK)
#  define Q_PLUGIN_VERIFICATION_SECTION \
    __attribute__ ((section (".ktplugin"))) __attribute__((used)) \
    static const char kt_plugin_verification_data[] = QT_VERSION_HEX_STR;
#else
#  define Q_PLUGIN_VERIFICATION_SECTION
#endif

// NOTE: if you change pattern, you MUST change the pattern in
// qlibrary.cpp as well.  changing the pattern will break all
// backwards compatibility as well (no old plugins will be loaded).
#define Q_EXPORT_PLUGIN2(PLUGIN, PLUGINCLASS) \
  Q_PLUGIN_VERIFICATION_SECTION \
  Q_EXTERN_C Q_DECL_EXPORT QT_PREPEND_NAMESPACE(QObject) * kt_plugin_instance() \
  { \
    static QT_PREPEND_NAMESPACE(QPointer)<QT_PREPEND_NAMESPACE(QObject)> _instance; \
    if (!_instance)      \
      _instance = new PLUGINCLASS; \
    return _instance; \
  }

QT_END_NAMESPACE

QT_END_HEADER

#endif // Q_PLUGIN_H

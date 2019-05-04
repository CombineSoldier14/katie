/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
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

#include "qguiplatformplugin.h"
#include <qdebug.h>
#include <qfile.h>
#include <qdir.h>
#include <qsettings.h>
#include "qfactoryloader_p.h"
#include "qstylefactory.h"
#include "qapplication.h"
#include "qplatformdefs.h"
#include "qicon.h"

QT_BEGIN_NAMESPACE


/*! \internal
    Return (an construct if necesseray) the Gui Platform plugin.

    The plugin key to be loaded is inside the QT_PLATFORM_PLUGIN environment variable.
    If it is not set, it will be the DESKTOP_SESSION on X11.

    If no plugin can be loaded, the default one is returned.
 */
QGuiPlatformPlugin *qt_guiPlatformPlugin()
{
    static QGuiPlatformPlugin *plugin;
    if (!plugin)
    {
#ifndef QT_NO_LIBRARY

        QString key = QString::fromLocal8Bit(qgetenv("QT_PLATFORM_PLUGIN"));
        if (key.isEmpty()) {
            key = QString::fromLocal8Bit(qgetenv("DESKTOP_SESSION"));
        }

        if (!key.isEmpty() && QApplication::desktopSettingsAware()) {
            QFactoryLoader loader(QGuiPlatformPluginInterface_iid, QLatin1String("/gui_platform"));
            plugin = qobject_cast<QGuiPlatformPlugin *>(loader.instance(key));
        }
#endif // QT_NO_LIBRARY

        if(!plugin) {
            static QGuiPlatformPlugin def;
            plugin = &def;
        }
    }
    return plugin;
}


/* \class QPlatformPlugin
    QGuiPlatformPlugin can be used to integrate Qt applications in a platform built on top of Qt.
    The application developer should not know or use the plugin, it is only used by Qt internaly.

    But full platform that are built on top of Qt may provide a plugin so 3rd party Qt application
    running in the platform are integrated.
 */

/*
    The constructor can be used to install hooks in Qt
 */
QGuiPlatformPlugin::QGuiPlatformPlugin(QObject *parent) : QObject(parent) {}
QGuiPlatformPlugin::~QGuiPlatformPlugin() {}


/* return the string key to be used by default the application */
QString QGuiPlatformPlugin::styleName()
{
    return QLatin1String("cleanlooks");
}

/* return an additional default palette  (only work on X11) */
QPalette QGuiPlatformPlugin::palette()
{
    return QPalette();
}

/* the default icon theme name for QIcon::fromTheme. */
QString QGuiPlatformPlugin::systemIconThemeName()
{
    return QString::fromLatin1("hicolor");
}


QStringList QGuiPlatformPlugin::iconThemeSearchPaths()
{
    QStringList paths;
    QString xdgDirString = QFile::decodeName(getenv("XDG_DATA_DIRS"));
    if (xdgDirString.isEmpty())
        xdgDirString = QLatin1String("/usr/local/share/:/usr/share/");

    QStringList xdgDirs = xdgDirString.split(QLatin1Char(':'));

    for (int i = 0 ; i < xdgDirs.size() ; ++i) {
        QDir dir(xdgDirs[i]);
        if (dir.exists())
            paths.append(dir.path() + QLatin1String("/icons"));
    }

    // Add home directory first in search path
    QDir homeDir(QDir::homePath() + QLatin1String("/.icons"));
    if (homeDir.exists())
        paths.prepend(homeDir.path());

    return paths;
}

/* backend for QIcon::fromTheme,  null icon means default */
QIcon QGuiPlatformPlugin::systemIcon(const QString &)
{
    return QIcon();
}

/* backend for QFileIconProvider,  null icon means default */
QIcon QGuiPlatformPlugin::fileSystemIcon(const QFileInfo &)
{
    return QIcon();
}

/* Like QStyle::styleHint */
int QGuiPlatformPlugin::platformHint(PlatformHint hint)
{
    int ret = 0;
    switch(hint)
    {
        case PH_ToolButtonStyle:
            ret = Qt::ToolButtonIconOnly;
            break;
        case PH_ToolBarIconSize:
            //by default keep ret = 0 so QCommonStyle will use the style default
            break;
        default:
            break;
    }
    return ret;
}


QT_END_NAMESPACE


#include "moc_qguiplatformplugin.h"

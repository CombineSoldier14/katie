/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the plugins of the Katie Toolkit.
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
#include "qconnmanengine.h"

#include <QtNetwork/qbearerplugin_p.h>

#include <QtCore/qdebug.h>

#ifndef QT_NO_BEARERMANAGEMENT
#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE

class QConnmanEnginePlugin : public QBearerEnginePlugin
{
public:
    QConnmanEnginePlugin();
    ~QConnmanEnginePlugin();

    QStringList keys() const;
    QBearerEngine *create(const QString &key) const;
};

QConnmanEnginePlugin::QConnmanEnginePlugin()
{
}

QConnmanEnginePlugin::~QConnmanEnginePlugin()
{
}

QStringList QConnmanEnginePlugin::keys() const
{
    return QStringList() << QLatin1String("connman");
}

QBearerEngine *QConnmanEnginePlugin::create(const QString &key) const
{
    if (key == QLatin1String("connman")) {
        QConnmanEngine *engine = new QConnmanEngine;
        if (engine->connmanAvailable())
            return engine;
        else
            delete engine;
    }
    return 0;
}

Q_EXPORT_STATIC_PLUGIN(QConnmanEnginePlugin)
Q_EXPORT_PLUGIN2(qconnmanbearer, QConnmanEnginePlugin)

QT_END_NAMESPACE

#endif
#endif // QT_NO_BEARERMANAGEMENT

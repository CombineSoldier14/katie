/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QFONTDATABASE_H
#define QFONTDATABASE_H

#include <QtGui/qwindowdefs.h>
#include <QtCore/qstring.h>
#include <QtGui/qfont.h>


QT_BEGIN_NAMESPACE

class QFontInfo;
class QStringList;
template <class T> class QList;
class QFontDatabasePrivate;

class Q_GUI_EXPORT QFontDatabase
{
public:
    QFontDatabase();
    ~QFontDatabase();

    QStringList families() const;
    QStringList styles(const QString &family) const;
    QList<int> pointSizes(const QString &family, const QString &style = QString());
    QList<int> smoothSizes(const QString &family, const QString &style);
    QString styleString(const QFont &font);
    QString styleString(const QFontInfo &fontInfo);

    QFont font(const QString &family, const QString &style, int pointSize) const;

    bool isSmoothlyScalable(const QString &family, const QString &style = QString()) const;
    bool isScalable(const QString &family, const QString &style = QString()) const;
    bool isFixedPitch(const QString &family, const QString &style = QString()) const;

    bool italic(const QString &family, const QString &style) const;
    bool bold(const QString &family, const QString &style) const;
    int weight(const QString &family, const QString &style) const;

    bool hasFamily(const QString &family) const;

    static QList<int> standardSizes();

    static bool supportsThreadedFontRendering();

private:
    Q_DISABLE_COPY(QFontDatabase);
    QFontDatabasePrivate *d_ptr;
};

QT_END_NAMESPACE


#endif // QFONTDATABASE_H

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

#ifndef QICON_P_H
#define QICON_P_H

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

#include <QtCore/qsize.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qicon.h>
#include <QtGui/qiconengine.h>

#ifndef QT_NO_ICON

QT_BEGIN_NAMESPACE

class QIconPrivate
{
public:
    QIconPrivate();

    ~QIconPrivate() {
        delete engine;
    }

    QIconEngine *engine;

    QAtomicInt ref;
    int serialNum;
    int detach_no;
};


struct QPixmapIconEngineEntry
{
    QPixmapIconEngineEntry():mode(QIcon::Normal), state(QIcon::Off){}
    QPixmapIconEngineEntry(const QPixmap &pm, QIcon::Mode m = QIcon::Normal, QIcon::State s = QIcon::Off)
        :pixmap(pm), size(pm.size()), mode(m), state(s){}
    QPixmapIconEngineEntry(const QString &file, const QSize &sz = QSize(), QIcon::Mode m = QIcon::Normal, QIcon::State s = QIcon::Off)
        :fileName(file), size(sz), mode(m), state(s){}
    QPixmap pixmap;
    QString fileName;
    QSize size;
    QIcon::Mode mode;
    QIcon::State state;
    bool isNull() const {return (fileName.isEmpty() && pixmap.isNull()); }
};



class QPixmapIconEngine : public QIconEngine {
public:
    QPixmapIconEngine();
    QPixmapIconEngine(const QPixmapIconEngine &);
    ~QPixmapIconEngine();
    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state);
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state);
    QPixmapIconEngineEntry *bestMatch(const QSize &size, QIcon::Mode mode, QIcon::State state, bool sizeOnly);
    QSize actualSize(const QSize &size, QIcon::Mode mode, QIcon::State state);
    QList<QSize> availableSizes(QIcon::Mode mode = QIcon::Normal, QIcon::State state = QIcon::Off) const;
    void addPixmap(const QPixmap &pixmap, QIcon::Mode mode, QIcon::State state);
    void addFile(const QString &fileName, const QSize &size, QIcon::Mode mode, QIcon::State state);

    QString key() const;
    QIconEngine *clone() const;
    bool read(QDataStream &in);
    bool write(QDataStream &out) const;

private:
    QPixmapIconEngineEntry *tryMatch(const QSize &size, QIcon::Mode mode, QIcon::State state);
    QVector<QPixmapIconEngineEntry> pixmaps;

    friend QDataStream &operator<<(QDataStream &s, const QIcon &icon);
    friend class QIconThemeEngine;
};

QT_END_NAMESPACE
#endif //QT_NO_ICON
#endif // QICON_P_H

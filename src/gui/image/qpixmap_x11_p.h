/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#ifndef QPIXMAPDATA_X11_P_H
#define QPIXMAPDATA_X11_P_H

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

#include "qpixmapdata_p.h"
#include "qx11info_x11.h"

QT_BEGIN_NAMESPACE

class QX11PaintEngine;

struct QXImageWrapper;

class Q_GUI_EXPORT QX11PixmapData : public QPixmapData
{
public:
    QX11PixmapData(PixelType type);
//     QX11PixmapData(PixelType type, int width, int height);
//     QX11PixmapData(PixelType type, const QImage &image,
//                    Qt::ImageConversionFlags flags);
    ~QX11PixmapData();

    QPixmapData *createCompatiblePixmapData() const;

    void resize(int width, int height);
    void fromImage(const QImage &image, Qt::ImageConversionFlags flags);
    void copy(const QPixmapData *data, const QRect &rect);
    bool scroll(int dx, int dy, const QRect &rect);

    void fill(const QColor &color);
    QBitmap mask() const;
    void setMask(const QBitmap &mask);
    bool hasAlphaChannel() const;
    void setAlphaChannel(const QPixmap &alphaChannel);
    QPixmap alphaChannel() const;
    QPixmap transformed(const QTransform &transform,
                        Qt::TransformationMode mode) const;
    QImage toImage() const;
    QImage toImage(const QRect &rect) const;
    QPaintEngine* paintEngine() const;

    Qt::HANDLE handle() const { return hd; }
    Qt::HANDLE x11ConvertToDefaultDepth();

    static Qt::HANDLE createBitmapFromImage(const QImage &image);

#ifndef QT_NO_XRENDER
    void convertToARGB32(bool preserveContents = true);
#endif

protected:
    int metric(QPaintDevice::PaintDeviceMetric metric) const;

private:
    friend class QPixmap;
    friend class QBitmap;
    friend class QX11PaintEngine;
    friend class QX11WindowSurface;
    friend class QRasterWindowSurface;

    void release();

    QImage toImage(const QXImageWrapper &xi, const QRect &rect) const;

    QBitmap mask_to_bitmap(int screen) const;
    static Qt::HANDLE bitmap_to_mask(const QBitmap &, int screen);
    void bitmapFromImage(const QImage &image);

    bool canTakeQImageFromXImage(const QXImageWrapper &xi) const;
    QImage takeQImageFromXImage(const QXImageWrapper &xi) const;

    Qt::HANDLE hd;

    enum Flag {
         NoFlags = 0x0,
         Uninitialized = 0x1,
         Readonly = 0x2
    };
    uint flags;

    QX11Info xinfo;
    Qt::HANDLE x11_mask;
    Qt::HANDLE picture;
    Qt::HANDLE mask_picture;
    Qt::HANDLE hd2; // sorted in the default display depth
    QPixmap::ShareMode share_mode;

    QX11PaintEngine *pengine;
};

QT_END_NAMESPACE

#endif // QPIXMAPDATA_X11_P_H


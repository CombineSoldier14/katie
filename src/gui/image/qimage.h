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

#ifndef QIMAGE_H
#define QIMAGE_H

#include <QtGui/qtransform.h>
#include <QtGui/qpaintdevice.h>
#include <QtGui/qrgb.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qrect.h>
#include <QtCore/qstring.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QColor;
class QIODevice;
class QMatrix;
class QTransform;
class QVariant;
template <class T> class QVector;
struct QImageData;

class Q_GUI_EXPORT QImage : public QPaintDevice
{
public:
    enum InvertMode { InvertRgb, InvertRgba };
    enum Format {
        Format_Invalid,
        Format_Mono,
        Format_MonoLSB,
        Format_Indexed8,
        Format_RGB32,
        Format_ARGB32,
        Format_ARGB32_Premultiplied,
        Format_RGB16,
        Format_ARGB8565_Premultiplied,
        Format_RGB666,
        Format_ARGB6666_Premultiplied,
        Format_RGB555,
        Format_ARGB8555_Premultiplied,
        Format_RGB888,
        Format_RGB444,
        Format_ARGB4444_Premultiplied,
#if 0
        // reserved for future use
        Format_RGB15,
        Format_Grayscale16,
        Format_Grayscale8,
        Format_Grayscale4,
        Format_Grayscale4LSB,
        Format_Grayscale2,
        Format_Grayscale2LSB
#endif
        NImageFormats
    };

    QImage();
    QImage(const QSize &size, Format format);
    QImage(int width, int height, Format format);
    QImage(uchar *data, int width, int height, Format format);
    QImage(const uchar *data, int width, int height, Format format);
    QImage(uchar *data, int width, int height, int bytesPerLine, Format format);
    QImage(const uchar *data, int width, int height, int bytesPerLine, Format format);

#ifndef QT_NO_IMAGEFORMAT_XPM
    explicit QImage(const char * const xpm[]);
#endif
    explicit QImage(const QString &fileName, const char *format = Q_NULLPTR);
#ifndef QT_NO_CAST_FROM_ASCII
    explicit QImage(const char *fileName, const char *format = Q_NULLPTR);
#endif

    QImage(const QImage &);
    ~QImage();

    QImage &operator=(const QImage &);
#ifdef Q_COMPILER_RVALUE_REFS
    inline QImage &operator=(QImage &&other)
    { qSwap(d, other.d); return *this; }
#endif
    inline void swap(QImage &other) { qSwap(d, other.d); }

    bool isNull() const;

    int devType() const;

    bool operator==(const QImage &) const;
    bool operator!=(const QImage &) const;
    operator QVariant() const;
    void detach();
    bool isDetached() const;

    QImage copy(const QRect &rect = QRect()) const;
    inline QImage copy(int x, int y, int w, int h) const
        { return copy(QRect(x, y, w, h)); }

    Format format() const;
    static Format systemFormat();

    QImage convertToFormat(Format f, Qt::ImageConversionFlags flags = Qt::AutoColor) const Q_REQUIRED_RESULT;
    QImage convertToFormat(Format f, const QVector<QRgb> &colorTable, Qt::ImageConversionFlags flags = Qt::AutoColor) const Q_REQUIRED_RESULT;

    int width() const;
    int height() const;
    QSize size() const;
    QRect rect() const;

    int depth() const;
    int colorCount() const;
    int bitPlaneCount() const;

    QRgb color(int i) const;
    void setColor(int i, QRgb c);
    void setColorCount(int);

    bool allGray() const;
    bool isGrayscale() const;

    uchar *bits();
    const uchar *bits() const;
    const uchar *constBits() const;
    int byteCount() const;

    uchar *scanLine(int);
    const uchar *scanLine(int) const;
    const uchar *constScanLine(int) const;
    int bytesPerLine() const;

    bool valid(int x, int y) const;
    inline bool valid(const QPoint &pt) const
        { return valid(pt.x(), pt.y()); }

    int pixelIndex(int x, int y) const;
    inline int pixelIndex(const QPoint &pt) const
        { return pixelIndex(pt.x(), pt.y());}

    QRgb pixel(int x, int y) const;
    inline QRgb pixel(const QPoint &pt) const
        { return pixel(pt.x(), pt.y()); }

    void setPixel(int x, int y, uint index_or_rgb);
    inline void setPixel(const QPoint &pt, uint index_or_rgb)
        { setPixel(pt.x(), pt.y(), index_or_rgb); }

    QVector<QRgb> colorTable() const;
    void setColorTable(const QVector<QRgb> &colors);

    void fill(uint pixel);
    void fill(const QColor &color);
    void fill(Qt::GlobalColor color);


    bool hasAlphaChannel() const;
    void setAlphaChannel(const QImage &alphaChannel);
    QImage alphaChannel() const;
    QImage createAlphaMask(Qt::ImageConversionFlags flags = Qt::AutoColor) const;
#ifndef QT_NO_IMAGE_HEURISTIC_MASK
    QImage createHeuristicMask(bool clipTight = true) const;
#endif
    QImage createMaskFromColor(QRgb color, Qt::MaskMode mode = Qt::MaskInColor) const;

    inline QImage scaled(int w, int h, Qt::AspectRatioMode aspectMode = Qt::IgnoreAspectRatio,
                        Qt::TransformationMode mode = Qt::FastTransformation) const
        { return scaled(QSize(w, h), aspectMode, mode); }
    QImage scaled(const QSize &s, Qt::AspectRatioMode aspectMode = Qt::IgnoreAspectRatio,
                 Qt::TransformationMode mode = Qt::FastTransformation) const;
    QImage scaledToWidth(int w, Qt::TransformationMode mode = Qt::FastTransformation) const;
    QImage scaledToHeight(int h, Qt::TransformationMode mode = Qt::FastTransformation) const;
    QImage transformed(const QMatrix &matrix, Qt::TransformationMode mode = Qt::FastTransformation) const;
    static QMatrix trueMatrix(const QMatrix &, int w, int h);
    QImage transformed(const QTransform &matrix, Qt::TransformationMode mode = Qt::FastTransformation) const;
    static QTransform trueMatrix(const QTransform &, int w, int h);
    QImage mirrored(bool horizontally = false, bool vertically = true) const;
    QImage rgbSwapped() const;
    void invertPixels(InvertMode = InvertRgb);


    bool load(QIODevice *device, const char* format);
    bool load(const QString &fileName, const char* format = Q_NULLPTR);
    bool loadFromData(const char *buf, int len, const char *format = Q_NULLPTR);
    inline bool loadFromData(const QByteArray &data, const char* aformat = Q_NULLPTR)
        { return loadFromData(data.constData(), data.size(), aformat); }

    bool save(const QString &fileName, const char* format = Q_NULLPTR, int quality=-1) const;
    bool save(QIODevice *device, const char* format = Q_NULLPTR, int quality=-1) const;

    static QImage fromData(const char *data, int size, const char *format = Q_NULLPTR);
    inline static QImage fromData(const QByteArray &data, const char *format = Q_NULLPTR)
        { return fromData(data.constData(), data.size(), format); }

    qint64 cacheKey() const;

    QPaintEngine *paintEngine() const;

    // Auxiliary data
    int dotsPerMeterX() const;
    int dotsPerMeterY() const;
    void setDotsPerMeterX(int);
    void setDotsPerMeterY(int);
    QPoint offset() const;
    void setOffset(const QPoint&);

protected:
    virtual int metric(PaintDeviceMetric metric) const;

private:
    QImageData *d;

    friend class QRasterPixmapData;
    friend class QPixmapCacheEntry;

public:
    typedef QImageData * DataPtr;
    inline DataPtr &data_ptr() { return d; }
};

Q_DECLARE_SHARED(QImage)
Q_DECLARE_TYPEINFO(QImage, Q_MOVABLE_TYPE);

// QImage stream functions

#if !defined(QT_NO_DATASTREAM)
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QImage &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QImage &);
#endif

QT_END_NAMESPACE

QT_END_HEADER

#endif // QIMAGE_H

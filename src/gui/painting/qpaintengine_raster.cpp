/****************************************************************************
**
** Copyright (C) 2021 Ivailo Monev
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

#include "qpaintengine_raster_p.h"
#include "qpen.h"
#include "qfont.h"
#include "qbuffer.h"
#include "qdebug.h"
#include "qimage_p.h"
#include "qpixmapdata_p.h"
#include "qpixmap_raster_p.h"

QT_BEGIN_NAMESPACE

// #define QT_RASTER_DEBUG
// #define QT_RASTER_EXPERIMENTAL
#define QT_RASTER_STATUS

extern QImage qt_imageForBrush(int brushStyle); // in qbrush.cpp
extern QPainterPath qt_regionToPath(const QRegion &region); // in qregion.cpp

#define QT_DUMP_BRUSH_BITS(style) \
    { \
        QImage sourceimage(qt_imageForBrush(style)); \
        sourceimage = sourceimage.convertToFormat(QImage::Format_ARGB32_Premultiplied); \
        const uchar* sourcedata = sourceimage.constBits(); \
        qDebug() << "begin" << # style; \
        { \
            QDebug dbg(QtMsgType::QtDebugMsg); \
            for (int i = 0; i < sourceimage.byteCount(); i++) { \
                dbg.nospace() << sourcedata[i] << ", "; \
            } \
        } \
        qDebug() << "end" << # style; \
    }

#ifdef QT_RASTER_STATUS
#  define QT_CHECK_RASTER_STATUS(cairo) \
     { \
         const cairo_status_t cairostatus = cairo_status(cairo); \
         if (Q_UNLIKELY(cairostatus != CAIRO_STATUS_SUCCESS)) { \
             qWarning() << Q_FUNC_INFO << cairo_status_to_string(cairostatus); \
         } \
     }
#else
#  define QT_CHECK_RASTER_STATUS(cairo)
#endif

static uchar* qt_bitsForBrush(const Qt::BrushStyle style)
{
    static uchar dense1bits[256] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
    static uchar dense2bits[256] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
    static uchar dense3bits[256] = { 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
    static uchar dense4bits[256] = { 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255 };
    static uchar dense5bits[256] = { 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255 };
    static uchar dense6bits[256] = { 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255 };
    static uchar dense7bits[256] = { 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255 };
    static uchar horbits[256] = { 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255 };
    static uchar verbits[256] = { 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255 };
    static uchar crossbits[256] = { 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255 };
    static uchar bdiagbits[256] = { 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255 };
    static uchar fdiagbits[256] = { 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255 };
    static uchar diagcrossbits[256] = { 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255 };

    switch (style) {
        case Qt::Dense1Pattern: {
            return dense1bits;
        }
        case Qt::Dense2Pattern: {
            return dense2bits;
        }
        case Qt::Dense3Pattern: {
            return dense3bits;
        }
        case Qt::Dense4Pattern: {
            return dense4bits;
        }
        case Qt::Dense5Pattern: {
            return dense5bits;
        }
        case Qt::Dense6Pattern: {
            return dense6bits;
        }
        case Qt::Dense7Pattern: {
            return dense7bits;
        }
        case Qt::HorPattern: {
            return horbits;
        }
        case Qt::VerPattern: {
            return verbits;
        }
        case Qt::CrossPattern: {
            return crossbits;
        }
        case Qt::BDiagPattern: {
            return bdiagbits;
        }
        case Qt::FDiagPattern: {
            return fdiagbits;
        }
        case Qt::DiagCrossPattern: {
            return diagcrossbits;
        }
        default: {
            Q_ASSERT_X(false, "qt_bitsForBrush", "internal error");
            break;
        }
    }
    Q_UNREACHABLE();
}

static const QPaintEngine::PaintEngineFeatures qt_raster_features =
    QPaintEngine::PerspectiveTransform |
    QPaintEngine::PatternTransform |
    QPaintEngine::ConstantOpacity |
    QPaintEngine::PorterDuff |
    QPaintEngine::BlendModes |
    QPaintEngine::Antialiasing;

static cairo_status_t qt_cairo_read(void *closure, unsigned char *data, unsigned int length)
{
    QBuffer* buffer = static_cast<QBuffer*>(closure);

    if (Q_LIKELY(buffer->read(reinterpret_cast<char*>(data), length) == length)) {
        return CAIRO_STATUS_SUCCESS;
    }

    return CAIRO_STATUS_READ_ERROR;
}

static cairo_status_t qt_cairo_write(void *closure, const unsigned char *data, unsigned int length)
{
    QBuffer* buffer = static_cast<QBuffer*>(closure);

    if (Q_LIKELY(buffer->write(reinterpret_cast<const char*>(data), length) == length)) {
        return CAIRO_STATUS_SUCCESS;
    }

    return CAIRO_STATUS_WRITE_ERROR;
}

QRasterPaintEnginePrivate::QRasterPaintEnginePrivate()
    : m_cairo(nullptr),
    m_cairosurface(nullptr)
{
#if 0
    QT_DUMP_BRUSH_BITS(Qt::Dense1Pattern)
    QT_DUMP_BRUSH_BITS(Qt::Dense2Pattern)
    QT_DUMP_BRUSH_BITS(Qt::Dense3Pattern)
    QT_DUMP_BRUSH_BITS(Qt::Dense4Pattern)
    QT_DUMP_BRUSH_BITS(Qt::Dense5Pattern)
    QT_DUMP_BRUSH_BITS(Qt::Dense6Pattern)
    QT_DUMP_BRUSH_BITS(Qt::Dense7Pattern)
    QT_DUMP_BRUSH_BITS(Qt::HorPattern)
    QT_DUMP_BRUSH_BITS(Qt::VerPattern)
    QT_DUMP_BRUSH_BITS(Qt::CrossPattern)
    QT_DUMP_BRUSH_BITS(Qt::BDiagPattern)
    QT_DUMP_BRUSH_BITS(Qt::FDiagPattern)
    QT_DUMP_BRUSH_BITS(Qt::DiagCrossPattern)
#endif
}
#undef QT_DUMP_BRUSH_BITS

QRasterPaintEnginePrivate::~QRasterPaintEnginePrivate()
{
    if (m_cairo) {
        QT_CHECK_RASTER_STATUS(m_cairo)

        cairo_destroy(m_cairo);
    }

    if (m_cairosurface) {
        cairo_surface_destroy(m_cairosurface);
    }
}

QRasterPaintEngine::QRasterPaintEngine()
    : QPaintEngine(*(new QRasterPaintEnginePrivate), qt_raster_features)
{
}

QRasterPaintEngine::QRasterPaintEngine(QRasterPaintEnginePrivate &dptr)
    : QPaintEngine(dptr, qt_raster_features)
{
}

QRasterPaintEngine::~QRasterPaintEngine()
{
}

bool QRasterPaintEngine::begin(QPaintDevice *pdev)
{
    Q_D(QRasterPaintEngine);

    // qDebug() << Q_FUNC_INFO << pdev;

    if (d->m_cairosurface || d->m_cairo) {
        // should not happen but it does
        if (!end()) {
            return false;
        }
    }

    switch (pdev->devType()) {
        case QInternal::Image: {
            const QImage* image = reinterpret_cast<QImage*>(pdev);
            const QSize imagesize(image->size());
            cairo_format_t cairoformat = CAIRO_FORMAT_ARGB32;
            switch (image->format()) {
                case QImage::Format_RGB16: {
                    cairoformat = CAIRO_FORMAT_RGB16_565;
                    break;
                }
                case QImage::Format_RGB32: {
                    cairoformat = CAIRO_FORMAT_RGB24;
                    break;
                }
                default: {
                    cairoformat = CAIRO_FORMAT_ARGB32;
                    break;
                }
            }
            d->m_cairosurface = cairo_image_surface_create(
                cairoformat, imagesize.width(), imagesize.height()
            );
            break;
        }
        case QInternal::Pixmap: {
            const QPixmap* pixmap = reinterpret_cast<QPixmap*>(pdev);
            const QSize pixmapsize(pixmap->size());
            d->m_cairosurface = cairo_image_surface_create(
                CAIRO_FORMAT_ARGB32,
                pixmapsize.width(), pixmapsize.height()
            );
            break;
        }
        default: {
            Q_ASSERT_X(false, "QRasterPaintEngine::begin", "internal error");
            break;
        }
    }
    if (Q_LIKELY(d->m_cairosurface)) {
        d->m_cairo = cairo_create(d->m_cairosurface);
        QT_CHECK_RASTER_STATUS(d->m_cairo)
    }

    if (Q_UNLIKELY(!d->m_cairosurface || !d->m_cairo)) {
        qWarning("QRasterPaintEngine: Could not create surface or context");
        return false;
    }

    const cairo_status_t cairostatus = cairo_status(d->m_cairo);
    if (Q_UNLIKELY(cairostatus != CAIRO_STATUS_SUCCESS)) {
        // happens with big images
        qWarning("QRasterPaintEngine: Surface format is not the one it should be");
        return false;
    }

    return true;
}

bool QRasterPaintEngine::end()
{
    Q_D(QRasterPaintEngine);

    // qDebug() << Q_FUNC_INFO;

    if (!d->m_cairosurface || !d->m_cairo) {
        // nothing painted?
        return true;
    }

    cairo_paint_with_alpha(d->m_cairo, state->opacity());
    QT_CHECK_RASTER_STATUS(d->m_cairo)

    bool result = false;
    const QPaintDevice* paintdevice(paintDevice());
    switch (paintdevice->devType()) {
        case QInternal::Image: {
            QImage* image = (QImage*)paintdevice;
            switch (image->format()) {
                case QImage::Format_RGB16:
                case QImage::Format_RGB32:
                case QImage::Format_ARGB32_Premultiplied: {
                    Q_ASSERT_X(image->d->ref == 1, "QRasterPaintEngine::end", "internal error");
                    const uchar* cairodata = cairo_image_surface_get_data(d->m_cairosurface);
                    ::memcpy(image->d->data, cairodata, image->byteCount());
                    result = true;
                    break;
                }
                default: {
                    Q_ASSERT_X(image->d->ref == 1, "QRasterPaintEngine::end", "internal error");
                    QImage converted(image->size(), QImage::Format_ARGB32_Premultiplied);
                    const uchar* cairodata = cairo_image_surface_get_data(d->m_cairosurface);
                    ::memcpy(converted.d->data, cairodata, image->byteCount());
                    converted = converted.convertToFormat(image->format());
                    ::memcpy(image->d->data, converted.d->data, image->byteCount());
                    result = true;
                    break;
                }
            }
            break;
        }
        case QInternal::Pixmap: {
            QPixmap* pixmap = (QPixmap*)paintdevice;
            QPixmapData *pixmapdata = pixmap->pixmapData();
            if (pixmapdata->classId() == QPixmapData::RasterClass) {
                QImage &image = static_cast<QRasterPixmapData *>(pixmapdata)->image;
                switch (image.format()) {
                    case QImage::Format_RGB16:
                    case QImage::Format_RGB32:
                    case QImage::Format_ARGB32_Premultiplied: {
                        Q_ASSERT_X(image.d->ref == 1, "QRasterPaintEngine::end", "internal error");
                        const uchar* cairodata = cairo_image_surface_get_data(d->m_cairosurface);
                        ::memcpy(image.d->data, cairodata, image.byteCount());
                        result = true;
                        break;
                    }
                    default: {
                        Q_ASSERT_X(image.d->ref == 1, "QRasterPaintEngine::end", "internal error");
                        QImage converted(image.size(), QImage::Format_ARGB32_Premultiplied);
                        const uchar* cairodata = cairo_image_surface_get_data(d->m_cairosurface);
                        ::memcpy(converted.d->data, cairodata, image.byteCount());
                        converted = converted.convertToFormat(image.format());
                        ::memcpy(image.d->data, converted.d->data, image.byteCount());
                        result = true;
                        break;
                    }
                }
                break;
            }

            QBuffer buffer;
            buffer.open(QBuffer::ReadWrite);
            cairo_surface_write_to_png_stream(d->m_cairosurface, qt_cairo_write, &buffer);
            buffer.seek(0);

            QPaintEngine* pixmapengine = pixmap->paintEngine();
            bool restoreengine = false;
            if (pixmapengine == this) {
                pixmap->data->setPaintEngine(nullptr);
                restoreengine = true;
            }
            result = pixmap->loadFromData(buffer.data(), "PNG");
            if (restoreengine) {
                pixmap->data->setPaintEngine(this);
            }
            break;
        }
        default: {
            Q_ASSERT_X(false, "QRasterPaintEngine::end", "internal error");
            break;
        }
    }

    cairo_destroy(d->m_cairo);
    d->m_cairo = nullptr;

    cairo_surface_destroy(d->m_cairosurface);
    d->m_cairosurface = nullptr;

    return result;
}

void QRasterPaintEngine::updateState(const QPaintEngineState &state)
{
    Q_D(QRasterPaintEngine);

#if 0
    qDebug() << Q_FUNC_INFO << state.pen() << state.brush() << state.brushOrigin()
        << state.backgroundBrush() << state.backgroundMode() << state.font()
        << state.matrix() << state.transform() << state.clipOperation()
        << state.clipRegion() << state.clipPath() << state.isClipEnabled()
        << state.renderHints() << state.compositionMode() << state.opacity();
#endif

    const QPaintEngine::DirtyFlags stateflags = state.state();

    // priority
    if (stateflags & QPaintEngine::DirtyTransform) {
        const QTransform statetransform(state.transform());

        switch (statetransform.type()) {
            case QTransform::TxNone: {
                cairo_matrix_t cairomatrix;
                cairo_matrix_init_identity(&cairomatrix);
                cairo_set_matrix(d->m_cairo, &cairomatrix);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
                break;
            }
            case QTransform::TxTranslate:
            case QTransform::TxScale:
            case QTransform::TxRotate:
            case QTransform::TxShear:
            case QTransform::TxProject: {
                cairo_matrix_t cairomatrix;
                cairo_matrix_init(&cairomatrix,
                    statetransform.m11(), statetransform.m12(),
                    statetransform.m21(), statetransform.m22(),
                    statetransform.dx(), statetransform.dy()
                );
                cairo_set_matrix(d->m_cairo, &cairomatrix);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
                break;
            }
        }
    }

    if (stateflags & QPaintEngine::DirtyHints) {
        const QPainter::RenderHints statehints(state.renderHints());

        if (statehints & QPainter::Antialiasing) {
            cairo_set_antialias(d->m_cairo, CAIRO_ANTIALIAS_BEST);
        } else {
            cairo_set_antialias(d->m_cairo, CAIRO_ANTIALIAS_DEFAULT);
        }
        QT_CHECK_RASTER_STATUS(d->m_cairo)

        if (statehints & QPainter::SmoothPixmapTransform) {
            d->m_cairofilter = CAIRO_FILTER_BEST;
        } else {
            d->m_cairofilter = CAIRO_FILTER_FAST;
        }

        cairo_font_options_t* cairooptions = cairo_font_options_create();
        cairo_get_font_options(d->m_cairo, cairooptions);
        QT_CHECK_RASTER_STATUS(d->m_cairo)
        if (statehints & QPainter::TextAntialiasing) {
            cairo_font_options_set_antialias(cairooptions, CAIRO_ANTIALIAS_SUBPIXEL);
        } else {
            cairo_font_options_set_antialias(cairooptions, CAIRO_ANTIALIAS_DEFAULT);
        }
        cairo_set_font_options(d->m_cairo, cairooptions);
        QT_CHECK_RASTER_STATUS(d->m_cairo)
        cairo_font_options_destroy(cairooptions);

        // QPainter::NonCosmeticDefaultPen handled internally by QPainterPrivate::updateState()
    }

    if (stateflags & QPaintEngine::DirtyCompositionMode) {
        switch (state.compositionMode()) {
            case QPainter::CompositionMode_SourceOver: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_OVER);
                break;
            }
            case QPainter::CompositionMode_DestinationOver: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_DEST_OVER);
                break;
            }
            case QPainter::CompositionMode_Clear: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_CLEAR);
                break;
            }
            case QPainter::CompositionMode_Source: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_SOURCE);
                break;
            }
            case QPainter::CompositionMode_Destination: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_DEST);
                break;
            }
            case QPainter::CompositionMode_SourceIn: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_IN);
                break;
            }
            case QPainter::CompositionMode_DestinationIn: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_DEST_IN);
                break;
            }
            case QPainter::CompositionMode_SourceOut: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_OUT);
                break;
            }
            case QPainter::CompositionMode_DestinationOut: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_DEST_OUT);
                break;
            }
            case QPainter::CompositionMode_SourceAtop: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_ATOP);
                break;
            }
            case QPainter::CompositionMode_DestinationAtop: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_DEST_ATOP);
                break;
            }
            case QPainter::CompositionMode_Xor: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_XOR);
                break;
            }
            case QPainter::CompositionMode_Plus: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_ADD);
                break;
            }
            case QPainter::CompositionMode_Multiply: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_MULTIPLY);
                break;
            }
            case QPainter::CompositionMode_Screen: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_SCREEN);
                break;
            }
            case QPainter::CompositionMode_Overlay: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_OVERLAY);
                break;
            }
            case QPainter::CompositionMode_Darken: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_DARKEN);
                break;
            }
            case QPainter::CompositionMode_Lighten: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_LIGHTEN);
                break;
            }
            case QPainter::CompositionMode_ColorDodge: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_COLOR_DODGE);
                break;
            }
            case QPainter::CompositionMode_ColorBurn: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_COLOR_BURN);
                break;
            }
            case QPainter::CompositionMode_HardLight: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_HARD_LIGHT);
                break;
            }
            case QPainter::CompositionMode_SoftLight: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_SOFT_LIGHT);
                break;
            }
            case QPainter::CompositionMode_Difference: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_DIFFERENCE);
                break;
            }
            case QPainter::CompositionMode_Exclusion: {
                cairo_set_operator(d->m_cairo, CAIRO_OPERATOR_EXCLUSION);
                break;
            }
        }
        QT_CHECK_RASTER_STATUS(d->m_cairo)
    }

    if (stateflags & QPaintEngine::DirtyClipEnabled) {
        const bool stateclip(state.isClipEnabled());
        if (!stateclip) {
            cairo_reset_clip(d->m_cairo);
            QT_CHECK_RASTER_STATUS(d->m_cairo)
        }
    }

    if (stateflags & QPaintEngine::DirtyClipPath) {
        switch (state.clipOperation()) {
            case Qt::NoClip: {
                cairo_reset_clip(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
                break;
            }
            // the clip can grow only if reset
            default: {
                const QPainterPath statepath(state.clipPath());

                cairo_reset_clip(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
                const QRectF pathrect(statepath.boundingRect());
                cairo_rectangle(d->m_cairo,
                    pathrect.x(), pathrect.y(),
                    pathrect.width(), pathrect.height()
                );
                cairo_clip(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
                break;
            }
        }
    } else if (stateflags & QPaintEngine::DirtyClipRegion) {
        switch (state.clipOperation()) {
            case Qt::NoClip: {
                cairo_reset_clip(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
                break;
            }
            default: {
                const QRegion stateregion(state.clipRegion());

                cairo_reset_clip(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
                if (!stateregion.isEmpty()) {
                    foreach (const QRect &regionrect, stateregion.rects()) {
                        cairo_rectangle(d->m_cairo,
                            regionrect.x(), regionrect.y(),
                            regionrect.width(), regionrect.height()
                        );
                        cairo_clip(d->m_cairo);
                        QT_CHECK_RASTER_STATUS(d->m_cairo)
                    }
                }
                break;
            }
        }
    }

    if (stateflags & QPaintEngine::DirtyPen) {
        const QPen statepen(state.pen());

        cairo_set_line_width(d->m_cairo, statepen.widthF());
        QT_CHECK_RASTER_STATUS(d->m_cairo)

        switch (statepen.capStyle()) {
            case Qt::FlatCap: {
                cairo_set_line_cap(d->m_cairo, CAIRO_LINE_CAP_BUTT);
                break;
            }
            case Qt::SquareCap: {
                cairo_set_line_cap(d->m_cairo, CAIRO_LINE_CAP_SQUARE);
                break;
            }
            case Qt::RoundCap: {
                cairo_set_line_cap(d->m_cairo, CAIRO_LINE_CAP_ROUND);
                break;
            }
        }
        QT_CHECK_RASTER_STATUS(d->m_cairo)

        switch (statepen.joinStyle()) {
            case Qt::MiterJoin: {
                cairo_set_line_join(d->m_cairo, CAIRO_LINE_JOIN_MITER);
                break;
            }
            case Qt::BevelJoin: {
                cairo_set_line_join(d->m_cairo, CAIRO_LINE_JOIN_BEVEL);
                break;
            }
            case Qt::RoundJoin: {
                cairo_set_line_join(d->m_cairo, CAIRO_LINE_JOIN_ROUND);
                break;
            }
            case Qt::SvgMiterJoin: {
                cairo_set_line_join(d->m_cairo, CAIRO_LINE_JOIN_MITER);
                break;
            }
        }
        QT_CHECK_RASTER_STATUS(d->m_cairo)

        cairo_set_miter_limit(d->m_cairo, statepen.miterLimit());
        QT_CHECK_RASTER_STATUS(d->m_cairo)

        const QVector<qreal> pendashpattern(statepen.dashPattern());
        cairo_set_dash(d->m_cairo, pendashpattern.data(), pendashpattern.size(), statepen.dashOffset());
        QT_CHECK_RASTER_STATUS(d->m_cairo)
    }

    if (stateflags & QPaintEngine::DirtyFont) {
        updateFont(state.font());
    }
}

void QRasterPaintEngine::drawPolygon(const QPointF *points, int pointCount, QPaintEngine::PolygonDrawMode mode)
{
    Q_D(QRasterPaintEngine);

    // qDebug() << Q_FUNC_INFO << points << pointCount << mode;

    cairo_new_path(d->m_cairo);
    QT_CHECK_RASTER_STATUS(d->m_cairo)
    cairo_move_to(d->m_cairo, points[0].x(), points[0].y());
    QT_CHECK_RASTER_STATUS(d->m_cairo)
    for (int i = 1; i < pointCount; i++) {
        cairo_line_to(d->m_cairo, points[i].x(), points[i].y());
        QT_CHECK_RASTER_STATUS(d->m_cairo)
    }
    if (mode != QPaintEngine::PolylineMode) {
        cairo_close_path(d->m_cairo);
        QT_CHECK_RASTER_STATUS(d->m_cairo)
    }

    cairo_pattern_t* penpattern = nullptr;
    cairo_pattern_t* brushpattern = nullptr;

    cairo_push_group(d->m_cairo);
    QT_CHECK_RASTER_STATUS(d->m_cairo)
    switch (mode) {
        case QPaintEngine::WindingMode: {
            const QPen statepen(state->pen());
            const QBrush statebrush(state->brush());

            if (statepen.style() != Qt::NoPen && statepen.brush().style() != Qt::NoBrush) {
                penpattern = penPattern(statepen);
                cairo_set_source(d->m_cairo, penpattern);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
                cairo_stroke(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
            } else {
                cairo_stroke_preserve(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
            }

            cairo_set_fill_rule(d->m_cairo, CAIRO_FILL_RULE_WINDING);
            QT_CHECK_RASTER_STATUS(d->m_cairo)
            if (statebrush.style() != Qt::NoBrush) {
                brushpattern = brushPattern(statebrush);
                cairo_set_source(d->m_cairo, brushpattern);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
                cairo_fill(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
            } else {
                cairo_fill_preserve(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
            }
            break;
        }
        case QPaintEngine::PolylineMode: {
            const QPen statepen(state->pen());
            if (statepen.style() != Qt::NoPen) {
                penpattern = penPattern(statepen);
                cairo_set_source(d->m_cairo, penpattern);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
                cairo_stroke(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
            } else {
                cairo_stroke_preserve(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
            }
            break;
        }
        default: {
            const QPen statepen(state->pen());
            const QBrush statebrush(state->brush());

            if (statepen.style() != Qt::NoPen && statepen.brush().style() != Qt::NoBrush) {
                penpattern = penPattern(statepen);
                cairo_set_source(d->m_cairo, penpattern);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
                cairo_stroke(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
            } else {
                cairo_stroke_preserve(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
            }

            cairo_set_fill_rule(d->m_cairo, CAIRO_FILL_RULE_EVEN_ODD);
            QT_CHECK_RASTER_STATUS(d->m_cairo)
            if (statebrush.style() != Qt::NoBrush) {
                brushpattern = brushPattern(statebrush);
                cairo_set_source(d->m_cairo, brushpattern);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
                cairo_fill(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
            } else {
                cairo_fill_preserve(d->m_cairo);
                QT_CHECK_RASTER_STATUS(d->m_cairo)
            }
            break;
        }
    }
    cairo_pop_group_to_source(d->m_cairo);
    QT_CHECK_RASTER_STATUS(d->m_cairo)
    cairo_paint_with_alpha(d->m_cairo, state->opacity());
    QT_CHECK_RASTER_STATUS(d->m_cairo)

    if (penpattern) {
        cairo_pattern_destroy(penpattern);
    }
    if (brushpattern) {
        cairo_pattern_destroy(brushpattern);
    }
}

void QRasterPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pixmap, const QRectF &sr)
{
    // qDebug() << Q_FUNC_INFO << r << pixmap.cacheKey() << sr;

    drawImage(r, pixmap.toImage(), sr);
}

void QRasterPaintEngine::drawImage(const QRectF &r, const QImage &image, const QRectF &sr,
                                   Qt::ImageConversionFlags flags)
{
    Q_D(QRasterPaintEngine);

    // qDebug() << Q_FUNC_INFO << r << image.cacheKey() << sr << flags;

    QImage sourceimage(image.copy(sr.toRect()));
    cairo_pattern_t* cairopattern = imagePattern(sourceimage, flags);

    if (!r.isEmpty()) {
#ifdef QT_RASTER_EXPERIMENTAL
        const QSizeF imagesize(sourceimage.size());
        QSizeF scaled(imagesize);
        scaled.scale(r.size(), Qt::IgnoreAspectRatio);
        // qDebug() << Q_FUNC_INFO << image.cacheKey() << r.size() << scaled;

        cairo_matrix_t cairomatrix;
        cairo_pattern_get_matrix(cairopattern, &cairomatrix);
        cairo_matrix_scale(&cairomatrix,
            scaled.height() / imagesize.height(),
            scaled.width() / imagesize.width()
        );
        cairo_pattern_set_matrix(cairopattern, &cairomatrix);
#else
        sourceimage = sourceimage.scaled(r.size().toSize(), Qt::IgnoreAspectRatio);
#endif

        cairo_move_to(d->m_cairo, r.x(), r.y());
        QT_CHECK_RASTER_STATUS(d->m_cairo)
    }

    cairo_push_group(d->m_cairo);
    QT_CHECK_RASTER_STATUS(d->m_cairo)
    cairo_set_source(d->m_cairo, cairopattern);
    QT_CHECK_RASTER_STATUS(d->m_cairo)
#ifdef QT_RASTER_DEBUG
    const QByteArray surfaceout = QByteArray("/tmp/surface-") + QByteArray::number(sourceimage.cacheKey()) + ".png";
    cairo_surface_write_to_png(d->m_cairosurface, surfaceout.constData());
#endif
    QT_CHECK_RASTER_STATUS(d->m_cairo)
    cairo_paint_with_alpha(d->m_cairo, state->opacity());
    cairo_pop_group_to_source(d->m_cairo);
    QT_CHECK_RASTER_STATUS(d->m_cairo)
    cairo_paint_with_alpha(d->m_cairo, state->opacity());
    QT_CHECK_RASTER_STATUS(d->m_cairo)
    cairo_pattern_destroy(cairopattern);
    QT_CHECK_RASTER_STATUS(d->m_cairo)
}

void QRasterPaintEngine::drawTextItem(const QPointF &p, const QTextItem &textItem)
{
    Q_D(QRasterPaintEngine);

    // qDebug() << Q_FUNC_INFO << p << textItem.text();

    const QByteArray text(textItem.text().toUtf8());
    const QFont statefont(state->font());

    // TODO: implement text item flags

    updateFont(textItem.font());

    cairo_move_to(d->m_cairo, p.x(), p.y());
    QT_CHECK_RASTER_STATUS(d->m_cairo)

    cairo_push_group(d->m_cairo);
    QT_CHECK_RASTER_STATUS(d->m_cairo)

    cairo_pattern_t* cairopattern = penPattern(state->pen());
    cairo_set_source(d->m_cairo, cairopattern);
    QT_CHECK_RASTER_STATUS(d->m_cairo)

    cairo_show_text(d->m_cairo, text.constData());
    QT_CHECK_RASTER_STATUS(d->m_cairo)

    cairo_pop_group_to_source(d->m_cairo);
    QT_CHECK_RASTER_STATUS(d->m_cairo)

    cairo_paint_with_alpha(d->m_cairo, state->opacity());
    QT_CHECK_RASTER_STATUS(d->m_cairo)

    cairo_pattern_destroy(cairopattern);

    updateFont(statefont);
}

void QRasterPaintEngine::updateFont(const QFont &font)
{
    Q_D(QRasterPaintEngine);

    // qDebug() << Q_FUNC_INFO << font;

    const QByteArray fontfamily(font.family().toUtf8());

    cairo_font_weight_t cairofontweight = CAIRO_FONT_WEIGHT_NORMAL;
    if (font.bold()) {
        cairofontweight = CAIRO_FONT_WEIGHT_BOLD;
    }

    cairo_font_slant_t cairofontslant = CAIRO_FONT_SLANT_NORMAL;
    switch (font.style()) {
        case QFont::StyleNormal: {
            cairofontslant = CAIRO_FONT_SLANT_NORMAL;
            break;
        }
        case QFont::StyleItalic: {
            cairofontslant = CAIRO_FONT_SLANT_ITALIC;
            break;
        }
        case QFont::StyleOblique: {
            cairofontslant = CAIRO_FONT_SLANT_OBLIQUE;
            break;
        }
    }

    cairo_font_options_t* cairooptions = cairo_font_options_create();
    cairo_get_font_options(d->m_cairo, cairooptions);
    switch (font.hintingPreference()) {
        case QFont::PreferDefaultHinting: {
            cairo_font_options_set_hint_style(cairooptions, CAIRO_HINT_STYLE_DEFAULT);
            break;
        }
        case QFont::PreferNoHinting: {
            cairo_font_options_set_hint_style(cairooptions, CAIRO_HINT_STYLE_NONE);
            break;
        }
        case QFont::PreferVerticalHinting: {
            cairo_font_options_set_hint_style(cairooptions, CAIRO_HINT_STYLE_SLIGHT);
            break;
        }
        case QFont::PreferFullHinting: {
            cairo_font_options_set_hint_style(cairooptions, CAIRO_HINT_STYLE_FULL);
            break;
        }
    }
    cairo_set_font_options(d->m_cairo, cairooptions);
    QT_CHECK_RASTER_STATUS(d->m_cairo)
    cairo_font_options_destroy(cairooptions);

    cairo_set_font_size(d->m_cairo, font.pointSizeF());
    QT_CHECK_RASTER_STATUS(d->m_cairo)
    cairo_select_font_face(d->m_cairo, fontfamily.constData(), cairofontslant, cairofontweight);
    QT_CHECK_RASTER_STATUS(d->m_cairo)
}

cairo_pattern_t* QRasterPaintEngine::imagePattern(const QImage &image, Qt::ImageConversionFlags flags)
{
    Q_D(QRasterPaintEngine);

    // qDebug() << Q_FUNC_INFO << image.cacheKey() << flags;

    QImage sourceimage(image);
    cairo_format_t cairoformat = CAIRO_FORMAT_ARGB32;
    switch (sourceimage.format()) {
        case QImage::Format_RGB16: {
            cairoformat = CAIRO_FORMAT_RGB16_565;
            break;
        }
        case QImage::Format_RGB32: {
            cairoformat = CAIRO_FORMAT_RGB24;
            break;
        }
        case QImage::Format_ARGB32_Premultiplied: {
            cairoformat = CAIRO_FORMAT_ARGB32;
            break;
        }
        default: {
            sourceimage = sourceimage.convertToFormat(QImage::Format_ARGB32_Premultiplied, flags);
            break;
        }
    }

    cairo_surface_t* imagesurface = cairo_image_surface_create_for_data((uchar*)sourceimage.bits(),
        cairoformat, sourceimage.width(), sourceimage.height(), sourceimage.bytesPerLine());

#ifdef QT_RASTER_DEBUG
    const QByteArray sourceout = QByteArray("/tmp/source-") + QByteArray::number(image.cacheKey()) + ".png";
    sourceimage.save(sourceout, "PNG");

    const QByteArray imageout = QByteArray("/tmp/image-") + QByteArray::number(image.cacheKey()) + ".png";
    cairo_surface_write_to_png(imagesurface, imageout.constData());
#endif

    if (Q_UNLIKELY(!imagesurface)) {
        qWarning("QRasterPaintEngine: Could not create image surface");
        return nullptr;
    }
    const cairo_format_t surfaceformat = cairo_image_surface_get_format(imagesurface);
    if (Q_UNLIKELY(surfaceformat != cairoformat)) {
        qWarning("QRasterPaintEngine: Image surface format is not %d", cairoformat);
        return nullptr;
    }

    cairo_pattern_t* cairopattern = cairo_pattern_create_for_surface(imagesurface);
    cairo_pattern_set_filter(cairopattern, d->m_cairofilter);

    cairo_surface_destroy(imagesurface);

    return cairopattern;
}

cairo_pattern_t* QRasterPaintEngine::penPattern(const QPen &pen)
{
    Q_D(QRasterPaintEngine);

    // qDebug() << Q_FUNC_INFO << pen;

    const QColor pencolor(pen.color());

    cairo_pattern_t* cairopattern = cairo_pattern_create_rgba(
        pencolor.redF(), pencolor.greenF(), pencolor.blueF(),
        pencolor.alphaF()
    );
    cairo_pattern_set_filter(cairopattern, d->m_cairofilter);

    return cairopattern;
}

cairo_pattern_t* QRasterPaintEngine::brushPattern(const QBrush &brush)
{
    Q_D(QRasterPaintEngine);

    // qDebug() << Q_FUNC_INFO << brush;

    cairo_pattern_t* cairopattern = nullptr;

    switch (brush.style()) {
        case Qt::NoBrush: {
            break;
        }
        case Qt::SolidPattern: {
            const QColor brushcolor(brush.color());
            cairopattern = cairo_pattern_create_rgba(
                brushcolor.redF(), brushcolor.greenF(), brushcolor.blueF(),
                brushcolor.alphaF()
            );
            break;
        }
        case Qt::Dense1Pattern:
        case Qt::Dense2Pattern:
        case Qt::Dense3Pattern:
        case Qt::Dense4Pattern:
        case Qt::Dense5Pattern:
        case Qt::Dense6Pattern:
        case Qt::Dense7Pattern:
        case Qt::HorPattern:
        case Qt::VerPattern:
        case Qt::CrossPattern:
        case Qt::BDiagPattern:
        case Qt::FDiagPattern:
        case Qt::DiagCrossPattern: {
#ifdef QT_RASTER_EXPERIMENTAL
            cairo_surface_t* imagesurface = cairo_image_surface_create_for_data(
                qt_bitsForBrush(brush.style()),
                CAIRO_FORMAT_ARGB32, 8, 8, 64
            );
            cairopattern = cairo_pattern_create_for_surface(imagesurface);
            cairo_pattern_set_filter(cairopattern, d->m_cairofilter);

            cairo_surface_destroy(imagesurface);
            break;
#else
            const QImage sourceimage(qt_imageForBrush(brush.style()));
            cairopattern = imagePattern(sourceimage);
            break;
#endif
        }
        case Qt::LinearGradientPattern: {
            const QLinearGradient* brushgradient(static_cast<const QLinearGradient*>(brush.gradient()));
            cairopattern = cairo_pattern_create_linear(
                brushgradient->start().x(), brushgradient->start().y(),
                brushgradient->finalStop().x(), brushgradient->finalStop().y()
            );
            foreach (const QGradientStop &gradientstop, brushgradient->stops()) {
                const QColor gradientcolor(gradientstop.second);
                cairo_pattern_add_color_stop_rgba(cairopattern,
                    gradientstop.first,
                    gradientcolor.redF(), gradientcolor.greenF(), gradientcolor.blueF(),
                    gradientcolor.alphaF()
                );
            }

            switch (brushgradient->spread()) {
                case QGradient::PadSpread: {
                    cairo_pattern_set_extend(cairopattern, CAIRO_EXTEND_PAD);
                    break;
                }
                case QGradient::ReflectSpread: {
                    cairo_pattern_set_extend(cairopattern, CAIRO_EXTEND_REFLECT);
                    break;
                }
                case QGradient::RepeatSpread: {
                    cairo_pattern_set_extend(cairopattern, CAIRO_EXTEND_REPEAT);
                    break;
                }
            }

            break;
        }
        case Qt::RadialGradientPattern: {
            const QRadialGradient* brushgradient(static_cast<const QRadialGradient*>(brush.gradient()));
            cairopattern = cairo_pattern_create_radial(
                brushgradient->center().x(), brushgradient->center().y(),
                brushgradient->centerRadius(),
                brushgradient->focalPoint().x(), brushgradient->focalPoint().y(),
                brushgradient->focalRadius()
            );
            foreach (const QGradientStop &gradientstop, brushgradient->stops()) {
                const QColor gradientcolor(gradientstop.second);
                cairo_pattern_add_color_stop_rgba(cairopattern,
                    gradientstop.first,
                    gradientcolor.redF(), gradientcolor.greenF(), gradientcolor.blueF(),
                    gradientcolor.alphaF()
                );
            }

            switch (brushgradient->spread()) {
                case QGradient::PadSpread: {
                    cairo_pattern_set_extend(cairopattern, CAIRO_EXTEND_PAD);
                    break;
                }
                case QGradient::ReflectSpread: {
                    cairo_pattern_set_extend(cairopattern, CAIRO_EXTEND_REFLECT);
                    break;
                }
                case QGradient::RepeatSpread: {
                    cairo_pattern_set_extend(cairopattern, CAIRO_EXTEND_REPEAT);
                    break;
                }
            }

            break;
        }
        case Qt::ConicalGradientPattern: {
            // TODO: drop support for conical gradient entirely
            qWarning("QRasterPaintEngine: Conical gradient brush pattern is not supported");
            break;
        }
        case Qt::TexturePattern: {
            cairopattern = imagePattern(brush.textureImage());
            break;
        }
    }

    if (Q_LIKELY(cairopattern)) {
        const QTransform brushtransform(brush.transform());
        cairo_matrix_t cairomatrix;
        cairo_matrix_init(&cairomatrix,
            brushtransform.m11(), brushtransform.m12(),
            brushtransform.m21(), brushtransform.m22(),
            brushtransform.dx(), brushtransform.dy()
        );
        cairo_pattern_set_matrix(cairopattern, &cairomatrix);

        cairo_pattern_set_filter(cairopattern, d->m_cairofilter);
    }

    return cairopattern;
}

QT_END_NAMESPACE

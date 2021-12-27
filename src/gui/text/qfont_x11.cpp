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

#define QT_FATAL_ASSERT

#include "qplatformdefs.h"

#include "qfont.h"
#include "qapplication.h"
#include "qfontinfo.h"
#include "qfontdatabase.h"
#include "qfontmetrics.h"
#include "qpaintdevice.h"
#include "qtextcodec.h"
#include "qunicodetables_p.h"
#include "qfontengine_p.h"
#include "qfontengine_x11_p.h"
#include "qtextengine_p.h"
#include "qt_x11_p.h"
#include "qx11info_x11.h"

#include <time.h>
#include <stdlib.h>
#include <ctype.h>

QT_BEGIN_NAMESPACE

double qt_pixelSize(double pointSize, int dpi)
{
    if (pointSize < 0)
        return -1.;
    if (dpi == 75) // the stupid 75 dpi setting on X11
        dpi = 72;
    return (pointSize * dpi) /72.;
}

double qt_pointSize(double pixelSize, int dpi)
{
    if (pixelSize < 0)
        return -1.;
    if (dpi == 75) // the stupid 75 dpi setting on X11
        dpi = 72;
    return pixelSize * 72. / ((double) dpi);
}

int QFontPrivate::defaultEncodingID = -1;

void QFont::initialize()
{
    extern int qt_encoding_id_for_mib(int mib); // from qfontdatabase_x11.cpp
    // determine the default encoding id using the locale, otherwise
    int mib = QTextCodec::codecForLocale()->mibEnum();

    // for asian locales, use the mib for the font codec instead of the locale codec
    switch (mib) {
    case 38: // eucKR
        mib = 36;
        break;

    case 2025: // GB2312
        mib = 57;
        break;

    case 113: // GBK
        mib = -113;
        break;

    case 114: // GB18030
        mib = -114;
        break;

    case 2026: // Big5
        mib = -2026;
        break;

    case 2101: // Big5-HKSCS
        mib = -2101;
        break;

    case 16: // JIS7
        mib = 15;
        break;

    case 17: // SJIS
    case 18: // eucJP
        mib = 63;
        break;
    }

    // get the default encoding id for the locale encoding...
    QFontPrivate::defaultEncodingID = qt_encoding_id_for_mib(mib);
}

void QFont::cleanup()
{
    QFontCache::cleanup();
}

/*!
  \internal
  X11 Only: Returns the screen with which this font is associated.
*/
int QFont::x11Screen() const
{
    return d->screen;
}

/*! \internal
    X11 Only: Associate the font with the specified \a screen.
*/
void QFont::x11SetScreen(int screen)
{
    if (screen < 0) // assume default
        screen = QX11Info::appScreen();

    if (screen == d->screen)
        return; // nothing to do

    detach();
    d->screen = screen;
}

Qt::HANDLE QFont::handle() const
{
    return 0;
}


FT_Face QFont::freetypeFace() const
{
    QFontEngine *engine = d->engineForScript(QUnicodeTables::Common);
    if (engine->type() == QFontEngine::Freetype) {
        const QFontEngineFT *ft = static_cast<const QFontEngineFT *>(engine);
        return ft->non_locked_face();
    }
    return 0;
}

QString QFont::lastResortFamily() const
{
    return QString::fromLatin1("Helvetica");
}

QString QFont::defaultFamily() const
{
    switch (d->request.styleHint) {
    case QFont::Times:
        return QString::fromLatin1("Times");

    case QFont::Courier:
        return QString::fromLatin1("Courier");

    case QFont::Monospace:
        return QString::fromLatin1("Courier New");

    case QFont::Cursive:
        return QString::fromLatin1("Comic Sans MS");

    case QFont::Fantasy:
        return QString::fromLatin1("Impact");

    case QFont::Decorative:
        return QString::fromLatin1("Old English");

    case QFont::Helvetica:
    case QFont::System:
    default:
        return QString::fromLatin1("Helvetica");
    }
}

static const char* LastResortFontsTbl[] = {
    "-*-helvetica-medium-r-*-*-*-120-*-*-*-*-*-*",
    "-*-courier-medium-r-*-*-*-120-*-*-*-*-*-*",
    "-*-times-medium-r-*-*-*-120-*-*-*-*-*-*",
    "-*-lucida-medium-r-*-*-*-120-*-*-*-*-*-*",
    "-*-helvetica-*-*-*-*-*-120-*-*-*-*-*-*",
    "-*-courier-*-*-*-*-*-120-*-*-*-*-*-*",
    "-*-times-*-*-*-*-*-120-*-*-*-*-*-*",
    "-*-lucida-*-*-*-*-*-120-*-*-*-*-*-*",
    "-*-helvetica-*-*-*-*-*-*-*-*-*-*-*-*",
    "-*-courier-*-*-*-*-*-*-*-*-*-*-*-*",
    "-*-times-*-*-*-*-*-*-*-*-*-*-*-*",
    "-*-lucida-*-*-*-*-*-*-*-*-*-*-*-*",
    "-*-fixed-*-*-*-*-*-*-*-*-*-*-*-*",
    "6x13",
    "7x13",
    "8x13",
    "9x15",
    "fixed",
};
static const qint16 LastResortFontsTblSize = 18;

/*
  Returns a last resort raw font name for the font matching algorithm.
  This is used if even the last resort family is not available. It
  returns \e something, almost no matter what.  The current
  implementation tries a wide variety of common fonts, returning the
  first one it finds. The implementation may change at any time.
*/
QString QFont::lastResortFont() const
{
    static QString last;

    // already found
    if (!last.isNull())
        return last;

    for (qint16 i = 0; i < LastResortFontsTblSize; i++) {
        int count;
        char **fontNames = XListFonts(QX11Info::display(), LastResortFontsTbl[i], SHRT_MAX, &count);
        if (fontNames) {
            XFreeFontNames(fontNames);
        }

        if (count != 0) {
            last = QString::fromLatin1(LastResortFontsTbl[i]);
            return last;
        }
    }

#if defined(CHECK_NULL)
    qFatal("QFontPrivate::lastResortFont: Cannot find any reasonable font");
#endif
    return last;
}

QT_END_NAMESPACE



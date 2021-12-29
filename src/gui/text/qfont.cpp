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

#include "qfont.h"
#include "qdebug.h"
#include "qpaintdevice.h"
#include "qfontdatabase.h"
#include "qfontmetrics.h"
#include "qfontinfo.h"
#include "qpainter.h"
#include "qhash.h"
#include "qdatastream.h"
#include "qapplication.h"
#include "qstringlist.h"

#include "qthread.h"
#include "qunicodetables_p.h"
#include "qfont_p.h"
#include "qfontengine_p.h"
#include "qpainter_p.h"
#include "qtextengine_p.h"
#include "qmutex.h"

#include <limits.h>

#ifdef Q_WS_X11
#include "qx11info_x11.h"
#include "qt_x11_p.h"
#endif

// #define QFONTCACHE_DEBUG
#ifdef QFONTCACHE_DEBUG
#  define FC_DEBUG qDebug
#else
#  define FC_DEBUG if (false) qDebug
#endif

QT_BEGIN_NAMESPACE

#ifdef Q_WS_X11
extern const QX11Info *qt_x11Info(const QPaintDevice *pd);
#endif

bool QFontDef::exactMatch(const QFontDef &other) const
{
    /*
      QFontDef comparison is more complicated than just simple
      per-member comparisons.

      When comparing point/pixel sizes, either point or pixelsize
      could be -1.  in This case we have to compare the non negative
      size value.

      This test will fail if the point-sizes differ by 1/2 point or
      more or they do not round to the same value.  We have to do this
      since our API still uses 'int' point-sizes in the API, but store
      deci-point-sizes internally.

      To compare the family members, we need to parse the font names
      and compare the family/foundry strings separately.  This allows
      us to compare e.g. "Helvetica" and "Helvetica [Adobe]" with
      positive results.
    */
    if (pixelSize != -1 && other.pixelSize != -1) {
        if (pixelSize != other.pixelSize)
            return false;
    } else if (pointSize != -1 && other.pointSize != -1) {
        if (pointSize != other.pointSize)
            return false;
    } else {
        return false;
    }

    if (!ignorePitch && !other.ignorePitch && fixedPitch != other.fixedPitch)
        return false;

    if (stretch != 0 && other.stretch != 0 && stretch != other.stretch)
        return false;

    QString this_family, this_foundry, other_family, other_foundry;
    QFontDatabase::parseFontName(family, this_foundry, this_family);
    QFontDatabase::parseFontName(other.family, other_foundry, other_family);

    return (styleStrategy == other.styleStrategy
            && weight        == other.weight
            && style        == other.style
            && this_family   == other_family
            && (styleName.isEmpty() || other.styleName.isEmpty() || styleName == other.styleName)
            && (this_foundry.isEmpty()
                || other_foundry.isEmpty()
                || this_foundry == other_foundry)
       );
}

QFontPrivate::QFontPrivate()
    : engineData(0), dpi(QX11Info::appDpiY()), screen(QX11Info::appScreen()),
      underline(false), overline(false), strikeOut(false), kerning(true),
      letterSpacingIsAbsolute(false)
{
}

QFontPrivate::QFontPrivate(const QFontPrivate &other)
    : request(other.request), engineData(0), dpi(other.dpi), screen(other.screen),
      underline(other.underline), overline(other.overline),
      strikeOut(other.strikeOut), kerning(other.kerning),
      letterSpacingIsAbsolute(other.letterSpacingIsAbsolute),
      letterSpacing(other.letterSpacing), wordSpacing(other.wordSpacing)
{
}

QFontPrivate::~QFontPrivate()
{
    if (engineData && !engineData->ref.deref())
        delete engineData;
    engineData = 0;
}

extern std::recursive_mutex &qt_fontdatabase_mutex();

QFontEngine *QFontPrivate::engineForScript(QUnicodeTables::Script script) const
{
    std::lock_guard<std::recursive_mutex> locker(qt_fontdatabase_mutex());
    if (script > QUnicodeTables::ScriptCount)
        script = QUnicodeTables::Common;
    if (engineData && engineData->fontCache != QFontCache::instance()) {
        // throw out engineData that came from a different thread
        if (!engineData->ref.deref())
            delete engineData;
        engineData = 0;
    }
    if (!engineData || !engineData->engines[script])
        QFontDatabase::load(this, script);
    return engineData->engines[script];
}

void QFontPrivate::resolve(uint mask, const QFontPrivate *other)
{
    Q_ASSERT(other != 0);

    dpi = other->dpi;

    if ((mask & QFont::AllPropertiesResolved) == QFont::AllPropertiesResolved) return;

    // assign the unset-bits with the set-bits of the other font def
    if (! (mask & QFont::FamilyResolved))
        request.family = other->request.family;

    if (! (mask & QFont::StyleNameResolved))
        request.styleName = other->request.styleName;

    if (! (mask & QFont::SizeResolved)) {
        request.pointSize = other->request.pointSize;
        request.pixelSize = other->request.pixelSize;
    }

    if (! (mask & QFont::StyleStrategyResolved))
        request.styleStrategy = other->request.styleStrategy;

    if (! (mask & QFont::WeightResolved))
        request.weight = other->request.weight;

    if (! (mask & QFont::StyleResolved))
        request.style = other->request.style;

    if (! (mask & QFont::FixedPitchResolved))
        request.fixedPitch = other->request.fixedPitch;

    if (! (mask & QFont::StretchResolved))
        request.stretch = other->request.stretch;

    if (! (mask & QFont::HintingPreferenceResolved))
        request.hintingPreference = other->request.hintingPreference;

    if (! (mask & QFont::UnderlineResolved))
        underline = other->underline;

    if (! (mask & QFont::OverlineResolved))
        overline = other->overline;

    if (! (mask & QFont::StrikeOutResolved))
        strikeOut = other->strikeOut;

    if (! (mask & QFont::KerningResolved))
        kerning = other->kerning;

    if (! (mask & QFont::LetterSpacingResolved)) {
        letterSpacing = other->letterSpacing;
        letterSpacingIsAbsolute = other->letterSpacingIsAbsolute;
    }
    if (! (mask & QFont::WordSpacingResolved))
        wordSpacing = other->wordSpacing;
}




QFontEngineData::QFontEngineData()
    : ref(1), fontCache(QFontCache::instance())
{
    memset(engines, 0, QUnicodeTables::ScriptCount * sizeof(QFontEngine *));
}

QFontEngineData::~QFontEngineData()
{
    for (int i = 0; i < QUnicodeTables::ScriptCount; ++i) {
        if (engines[i] && !engines[i]->ref.deref())
            delete engines[i];
        engines[i] = 0;
    }
}




/*!
    \class QFont
    \reentrant

    \brief The QFont class specifies a font used for drawing text.

    \ingroup painting
    \ingroup appearance
    \ingroup shared
    \ingroup richtext-processing


    When you create a QFont object you specify various attributes that
    you want the font to have. Qt will use the font with the specified
    attributes, or if no matching font exists, Qt will use the closest
    matching installed font. The attributes of the font that is
    actually used are retrievable from a QFontInfo object. If the
    window system provides an exact match exactMatch() returns true.
    Use QFontMetrics to get measurements, e.g. the pixel length of a
    string using QFontMetrics::width().

    Note that a QApplication instance must exist before a QFont can be
    used. You can set the application's default font with
    QApplication::setFont().

    If a chosen font does not include all the characters that
    need to be displayed, QFont will try to find the characters in the
    nearest equivalent fonts. When a QPainter draws a character from a
    font the QFont will report whether or not it has the character; if
    it does not, QPainter will draw an unfilled square.

    Create QFonts like this:

    \snippet doc/src/snippets/code/src_gui_text_qfont.cpp 0

    The attributes set in the constructor can also be set later, e.g.
    setFamily(), setPointSize(), setPointSizeFloat(), setWeight() and
    setItalic(). The remaining attributes must be set after
    contstruction, e.g. setBold(), setUnderline(), setOverline(),
    setStrikeOut() and setFixedPitch(). QFontInfo objects should be
    created \e after the font's attributes have been set. A QFontInfo
    object will not change, even if you change the font's
    attributes. The corresponding "get" functions, e.g. family(),
    pointSize(), etc., return the values that were set, even though
    the values used may differ. The actual values are available from a
    QFontInfo object.

    If the requested font family is unavailable you can influence the
    \link #fontmatching font matching algorithm\endlink by choosing a
    particular \l{QFont::StyleStrategy} with setStyleStrategy(). The
    default family is returned by defaultFamily().

    The font-matching algorithm has a lastResortFamily() and
    lastResortFont() in cases where a suitable match cannot be found.

    Every QFont has a key() which you can use, for example, as the key
    in a cache or dictionary. If you want to store a user's font
    preferences you could use QSettings, writing the font information
    with toString() and reading it back with fromString(). The
    operator<<() and operator>>() functions are also available, but
    they work on a data stream.

    It is possible to set the height of characters shown on the screen
    to a specified number of pixels with setPixelSize(); however using
    setPointSize() has a similar effect and provides device
    independence.

    Loading fonts can be expensive, especially on X11. QFont contains
    extensive optimizations to make the copying of QFont objects fast,
    and to cache the results of the slow window system functions it
    depends upon.

    \target fontmatching
    The font matching algorithm works as follows:
    \list 1
    \o The specified font family is searched for.
    \o Each replacement font family is searched for.
    \o If none of these are found "helvetica" will be searched for.
    \o If "helvetica" isn't found Katie will try the lastResortFamily().
    \o If the lastResortFamily() isn't found Katie will try the
       lastResortFont() which will always return a name of some kind.
    \endlist

    Note that the actual font matching algorithm varies from platform to platform.

    In Windows a request for the "Courier" font is automatically changed to
    "Courier New", an improved version of Courier that allows for smooth scaling.
    The older "Courier" bitmap font can be selected by setting the PreferBitmap
    style strategy (see setStyleStrategy()).

    Once a font is found, the remaining attributes are matched in order of
    priority:
    \list 1
    \o fixedPitch()
    \o pointSize() (see below)
    \o weight()
    \o style()
    \endlist

    If you have a font which matches on family, even if none of the
    other attributes match, this font will be chosen in preference to
    a font which doesn't match on family but which does match on the
    other attributes. This is because font family is the dominant
    search criteria.

    The point size is defined to match if it is within 20% of the
    requested point size. When several fonts match and are only
    distinguished by point size, the font with the closest point size
    to the one requested will be chosen.

    The actual family, font size, weight and other font attributes
    used for drawing text will depend on what's available for the
    chosen family under the window system. A QFontInfo object can be
    used to determine the actual values used for drawing the text.

    Examples:

    \snippet doc/src/snippets/code/src_gui_text_qfont.cpp 1
    If you had both an Adobe and a Cronyx Helvetica, you might get
    either.

    \snippet doc/src/snippets/code/src_gui_text_qfont.cpp 2

    You can specify the foundry you want in the family name. The font f
    in the above example will be set to "Helvetica
    [Cronyx]".

    To determine the attributes of the font actually used in the window
    system, use a QFontInfo object, e.g.

    \snippet doc/src/snippets/code/src_gui_text_qfont.cpp 3

    To find out font metrics use a QFontMetrics object, e.g.

    \snippet doc/src/snippets/code/src_gui_text_qfont.cpp 4

    For more general information on fonts, see the
    \link http://nwalsh.com/comp.fonts/FAQ/ comp.fonts FAQ.\endlink
    Information on encodings can be found from
    \link http://czyborra.com/ Roman Czyborra's\endlink page.

    \sa QFontComboBox, QFontMetrics, QFontInfo, QFontDatabase, {Character Map Example}
*/

/*!
    \internal
    \enum QFont::ResolveProperties

    This enum describes the properties of a QFont that can be set on a font
    individually and then considered resolved.

    \value FamilyResolved
    \value SizeResolved
    \value StyleStrategyResolved
    \value WeightResolved
    \value StyleResolved
    \value UnderlineResolved
    \value OverlineResolved
    \value StrikeOutResolved
    \value FixedPitchResolved
    \value StretchResolved
    \value KerningResolved
    \value LetterSpacingResolved
    \value WordSpacingResolved
    \value CompletelyResolved
*/

/*!
    \enum QFont::Style

    This enum describes the different styles of glyphs that are used to
    display text.

    \value StyleNormal  Normal glyphs used in unstyled text.
    \value StyleItalic  Italic glyphs that are specifically designed for
                        the purpose of representing italicized text.
    \value StyleOblique Glyphs with an italic appearance that are typically
                        based on the unstyled glyphs, but are not fine-tuned
                        for the purpose of representing italicized text.

    \sa Weight
*/

/*!
    \fn Qt::HANDLE QFont::handle() const

    Returns the window system handle to the font, for low-level
    access. Using this function is \e not portable.
*/

/*!
    \fn FT_Face QFont::freetypeFace() const

    Returns the handle to the primary FreeType face of the font. If font merging is not disabled a
    QFont can contain several physical fonts.

    Returns 0 if the font does not contain a FreeType face.

    \note This function is only available on platforms that provide the FreeType library;
    i.e., X11 and some Embedded Linux platforms.
*/

/*!
    \fn QString QFont::lastResortFamily() const

    Returns the "last resort" font family name.

    The current implementation tries a wide variety of common fonts,
    returning the first one it finds. Is is possible that no family is
    found in which case an empty string is returned.

    \sa lastResortFont()
*/

/*!
    \fn QString QFont::defaultFamily() const

    Returns the family name that corresponds to the current style.
*/

/*!
    \fn QString QFont::lastResortFont() const

    Returns a "last resort" font name for the font matching algorithm.
    This is used if the last resort family is not available. It will
    always return a name, if necessary returning something like
    "fixed" or "system".

    The current implementation tries a wide variety of common fonts,
    returning the first one it finds. The implementation may change
    at any time, but this function will always return a string
    containing something.

    It is theoretically possible that there really isn't a
    lastResortFont() in which case Qt will abort with an error
    message. We have not been able to identify a case where this
    happens. Please \link bughowto.html report it as a bug\endlink if
    it does, preferably with a list of the fonts you have installed.

    \sa lastResortFamily()
*/

/*!
  Constructs a font from \a font for use on the paint device \a pd.
*/
QFont::QFont(const QFont &font, QPaintDevice *pd)
    : resolve_mask(font.resolve_mask)
{
    Q_ASSERT(pd != 0);
    int dpi = pd->logicalDpiY();
#ifdef Q_WS_X11
    const QX11Info *info = qt_x11Info(pd);
    int screen = info ? info->screen() : 0;
#else
    const int screen = 0;
#endif
    if (font.d->dpi != dpi || font.d->screen != screen ) {
        d = new QFontPrivate(*font.d);
        d->dpi = dpi;
        d->screen = screen;
    } else {
        d = font.d.data();
    }
}

/*!
  \internal
*/
QFont::QFont(QFontPrivate *data)
    : d(data), resolve_mask(QFont::AllPropertiesResolved)
{
}

/*! \internal
    Detaches the font object from common font data.
*/
void QFont::detach()
{
    if (d->ref == 1) {
        if (d->engineData && !d->engineData->ref.deref())
            delete d->engineData;
        d->engineData = 0;
        return;
    }

    d.detach();
}

/*!
    Constructs a font object that uses the application's default font.

    \sa QApplication::setFont(), QApplication::font()
*/
QFont::QFont()
    : d(QApplication::font().d.data()), resolve_mask(0)
{
}

/*!
    Constructs a font object with the specified \a family, \a
    pointSize, \a weight and \a italic settings.

    If \a pointSize is zero or negative, the point size of the font
    is set to a system-dependent default value. Generally, this is
    12 points, except on Symbian where it is 7 points.

    The \a family name may optionally also include a foundry name,
    e.g. "Helvetica [Cronyx]". If the \a family is
    available from more than one foundry and the foundry isn't
    specified, an arbitrary foundry is chosen. If the family isn't
    available a family will be set using the \l{QFont}{font matching}
    algorithm.

    \sa Weight, setFamily(), setPointSize(), setWeight(), setItalic(),
    QApplication::font()
*/
QFont::QFont(const QString &family, int pointSize, int weight, bool italic)
    : d(new QFontPrivate()), resolve_mask(QFont::FamilyResolved)
{
    if (pointSize <= 0) {
        pointSize = 12;
    } else {
        resolve_mask |= QFont::SizeResolved;
    }

    if (weight < 0) {
        weight = Normal;
    } else {
        resolve_mask |= QFont::WeightResolved | QFont::StyleResolved;
    }

    if (italic)
        resolve_mask |= QFont::StyleResolved;

    d->request.family = family;
    d->request.pointSize = qreal(pointSize);
    d->request.pixelSize = -1;
    d->request.weight = weight;
    d->request.style = italic ? QFont::StyleItalic : QFont::StyleNormal;
}

/*!
    Constructs a font that is a copy of \a font.
*/
QFont::QFont(const QFont &font)
    : d(font.d.data()), resolve_mask(font.resolve_mask)
{
}

/*!
    Destroys the font object and frees all allocated resources.
*/
QFont::~QFont()
{
}

/*!
    Assigns \a font to this font and returns a reference to it.
*/
QFont &QFont::operator=(const QFont &font)
{
    d = font.d.data();
    resolve_mask = font.resolve_mask;
    return *this;
}

/*!
    Returns the requested font family name, i.e. the name set in the
    constructor or the last setFont() call.

    \sa setFamily()
*/
QString QFont::family() const
{
    return d->request.family;
}

/*!
    Sets the family name of the font. The name is case insensitive and
    may include a foundry name.

    The \a family name may optionally also include a foundry name,
    e.g. "Helvetica [Cronyx]". If the \a family is
    available from more than one foundry and the foundry isn't
    specified, an arbitrary foundry is chosen. If the family isn't
    available a family will be set using the \l{QFont}{font matching}
    algorithm.

    \sa family(), QFontInfo
*/
void QFont::setFamily(const QString &family)
{
    detach();

    d->request.family = family;

    resolve_mask |= QFont::FamilyResolved;
}

/*!
    \since 4.8

    Returns the requested font style name, it will be used to match the
    font with irregular styles (that can't be normalized in other style
    properties). It depends on system font support, thus only works for
    Mac OS X and X11 so far. On Windows irregular styles will be added
    as separate font families so there is no need for this.

    \sa setFamily() setStyle()
*/
QString QFont::styleName() const
{
    return d->request.styleName;
}

/*!
    \since 4.8

    Sets the style name of the font to the given \a styleName.
    When set, other style properties like style() and weight() will be ignored
    for font matching.

    \sa styleName()
*/
void QFont::setStyleName(const QString &styleName)
{
    detach();

    d->request.styleName = styleName;
    resolve_mask |= QFont::StyleNameResolved;
}

/*!
    Returns the point size of the font. Returns -1 if the font size
    was specified in pixels.

    \sa setPointSize() pointSizeF()
*/
int QFont::pointSize() const
{
    return qRound(d->request.pointSize);
}

/*!
    \since 4.8

    \enum QFont::HintingPreference

    This enum describes the different levels of hinting that can be applied
    to glyphs to improve legibility on displays where it might be warranted
    by the density of pixels.

    \value PreferDefaultHinting Use the default hinting level for the target platform.
    \value PreferNoHinting If possible, render text without hinting the outlines
           of the glyphs. The text layout will be typographically accurate and
           scalable, using the same metrics as are used e.g. when printing.
    \value PreferVerticalHinting If possible, render text with no horizontal hinting,
           but align glyphs to the pixel grid in the vertical direction. The text will appear
           crisper on displays where the density is too low to give an accurate rendering
           of the glyphs. But since the horizontal metrics of the glyphs are unhinted, the text's
           layout will be scalable to higher density devices (such as printers) without impacting
           details such as line breaks.
    \value PreferFullHinting If possible, render text with hinting in both horizontal and
           vertical directions. The text will be altered to optimize legibility on the target
           device, but since the metrics will depend on the target size of the text, the positions
           of glyphs, line breaks, and other typographical detail will not scale, meaning that a
           text layout may look different on devices with different pixel densities.

    Please note that this enum only describes a preference, as the full range of hinting levels
    are not supported on all of Qt's supported platforms. The following table details the effect
    of a given hinting preference on a selected set of target platforms.

    \table
    \header
    \o
    \o PreferDefaultHinting
    \o PreferNoHinting
    \o PreferVerticalHinting
    \o PreferFullHinting
    \row
    \o FreeType
    \o Operating System setting
    \o No hinting
    \o Vertical hinting (light)
    \o Full hinting
    \endtable

*/

/*!
    \since 4.8

    Set the preference for the hinting level of the glyphs to \a hintingPreference. This is a hint
    to the underlying font rendering system to use a certain level of hinting, and has varying
    support across platforms. See the table in the documentation for QFont::HintingPreference for
    more details.

    The default hinting preference is QFont::PreferDefaultHinting.
*/
void QFont::setHintingPreference(HintingPreference hintingPreference)
{
    detach();

    d->request.hintingPreference = hintingPreference;

    resolve_mask |= QFont::HintingPreferenceResolved;
}

/*!
    \since 4.8

    Returns the currently preferred hinting level for glyphs rendered with this font.
*/
QFont::HintingPreference QFont::hintingPreference() const
{
    return d->request.hintingPreference;
}

/*!
    Sets the point size to \a pointSize. The point size must be
    greater than zero.

    \sa pointSize() setPointSizeF()
*/
void QFont::setPointSize(int pointSize)
{
    if (pointSize <= 0) {
        qWarning("QFont::setPointSize: Point size <= 0 (%d), must be greater than 0", pointSize);
        return;
    }

    detach();

    d->request.pointSize = qreal(pointSize);
    d->request.pixelSize = -1;

    resolve_mask |= QFont::SizeResolved;
}

/*!
    Sets the point size to \a pointSize. The point size must be
    greater than zero. The requested precision may not be achieved on
    all platforms.

    \sa pointSizeF() setPointSize() setPixelSize()
*/
void QFont::setPointSizeF(qreal pointSize)
{
    if (pointSize <= 0) {
        qWarning("QFont::setPointSizeF: Point size <= 0 (%f), must be greater than 0", pointSize);
        return;
    }

    detach();

    d->request.pointSize = pointSize;
    d->request.pixelSize = -1;

    resolve_mask |= QFont::SizeResolved;
}

/*!
    Returns the point size of the font. Returns -1 if the font size was
    specified in pixels.

    \sa pointSize() setPointSizeF() pixelSize() QFontInfo::pointSize() QFontInfo::pixelSize()
*/
qreal QFont::pointSizeF() const
{
    return d->request.pointSize;
}

/*!
    Sets the font size to \a pixelSize pixels.

    Using this function makes the font device dependent. Use
    setPointSize() or setPointSizeF() to set the size of the font
    in a device independent manner.

    \sa pixelSize()
*/
void QFont::setPixelSize(int pixelSize)
{
    if (pixelSize <= 0) {
        qWarning("QFont::setPixelSize: Pixel size <= 0 (%d)", pixelSize);
        return;
    }

    detach();

    d->request.pixelSize = pixelSize;
    d->request.pointSize = -1;

    resolve_mask |= QFont::SizeResolved;
}

/*!
    Returns the pixel size of the font if it was set with
    setPixelSize(). Returns -1 if the size was set with setPointSize()
    or setPointSizeF().

    \sa setPixelSize() pointSize() QFontInfo::pointSize() QFontInfo::pixelSize()
*/
int QFont::pixelSize() const
{
    return d->request.pixelSize;
}


/*!
  \fn bool QFont::italic() const

    Returns true if the style() of the font is not QFont::StyleNormal

    \sa setItalic() style()
*/

/*!
  \fn void QFont::setItalic(bool enable)

  Sets the style() of the font to QFont::StyleItalic if \a enable is true;
  otherwise the style is set to QFont::StyleNormal.

  \sa italic() QFontInfo
*/

/*!
    Returns the style of the font.

    \sa setStyle()
*/
QFont::Style QFont::style() const
{
    return d->request.style;
}


/*!
  Sets the style of the font to \a style.

  \sa italic(), QFontInfo
*/
void QFont::setStyle(Style style)
{
    detach();

    d->request.style = style;
    resolve_mask |= QFont::StyleResolved;
}

/*!
    Returns the weight of the font which is one of the enumerated
    values from \l{QFont::Weight}.

    \sa setWeight(), Weight, QFontInfo
*/
int QFont::weight() const
{
    return d->request.weight;
}

/*!
    \enum QFont::Weight

    Qt uses a weighting scale from 0 to 99 similar to, but not the
    same as, the scales used in Windows or CSS. A weight of 0 is
    ultralight, whilst 99 will be an extremely black.

    This enum contains the predefined font weights:

    \value Light 25
    \value Normal 50
    \value DemiBold 63
    \value Bold 75
    \value Black 87
*/

/*!
    Sets the weight the font to \a weight, which should be a value
    from the \l QFont::Weight enumeration.

    \sa weight(), QFontInfo
*/
void QFont::setWeight(int weight)
{
    Q_ASSERT_X(weight >= 0 && weight <= 99, "QFont::setWeight", "Weight must be between 0 and 99");

    detach();

    d->request.weight = weight;
    resolve_mask |= QFont::WeightResolved;
}

/*!
    \fn bool QFont::bold() const

    Returns true if weight() is a value greater than \link Weight
    QFont::Normal \endlink; otherwise returns false.

    \sa weight(), setBold(), QFontInfo::bold()
*/

/*!
    \fn void QFont::setBold(bool enable)

    If \a enable is true sets the font's weight to \link Weight
    QFont::Bold \endlink; otherwise sets the weight to \link Weight
    QFont::Normal\endlink.

    For finer boldness control use setWeight().

    \sa bold(), setWeight()
*/

/*!
    Returns true if underline has been set; otherwise returns false.

    \sa setUnderline()
*/
bool QFont::underline() const
{
    return d->underline;
}

/*!
    If \a enable is true, sets underline on; otherwise sets underline
    off.

    \sa underline(), QFontInfo
*/
void QFont::setUnderline(bool enable)
{
    detach();

    d->underline = enable;
    resolve_mask |= QFont::UnderlineResolved;
}

/*!
    Returns true if overline has been set; otherwise returns false.

    \sa setOverline()
*/
bool QFont::overline() const
{
    return d->overline;
}

/*!
  If \a enable is true, sets overline on; otherwise sets overline off.

  \sa overline(), QFontInfo
*/
void QFont::setOverline(bool enable)
{
    detach();

    d->overline = enable;
    resolve_mask |= QFont::OverlineResolved;
}

/*!
    Returns true if strikeout has been set; otherwise returns false.

    \sa setStrikeOut()
*/
bool QFont::strikeOut() const
{
    return d->strikeOut;
}

/*!
    If \a enable is true, sets strikeout on; otherwise sets strikeout
    off.

    \sa strikeOut(), QFontInfo
*/
void QFont::setStrikeOut(bool enable)
{
    detach();

    d->strikeOut = enable;
    resolve_mask |= QFont::StrikeOutResolved;
}

/*!
    Returns true if fixed pitch has been set; otherwise returns false.

    \sa setFixedPitch(), QFontInfo::fixedPitch()
*/
bool QFont::fixedPitch() const
{
    return d->request.fixedPitch;
}

/*!
    If \a enable is true, sets fixed pitch on; otherwise sets fixed
    pitch off.

    \sa fixedPitch(), QFontInfo
*/
void QFont::setFixedPitch(bool enable)
{
    detach();

    d->request.fixedPitch = enable;
    d->request.ignorePitch = false;
    resolve_mask |= QFont::FixedPitchResolved;
}

/*!
  Returns true if kerning should be used when drawing text with this font.

  \sa setKerning()
*/
bool QFont::kerning() const
{
    return d->kerning;
}

/*!
    Enables kerning for this font if \a enable is true; otherwise
    disables it. By default, kerning is enabled.

    When kerning is enabled, glyph metrics do not add up anymore,
    even for Latin text. In other words, the assumption that
    width('a') + width('b') is equal to width("ab") is not
    neccesairly true.

    \sa kerning(), QFontMetrics
*/
void QFont::setKerning(bool enable)
{
    detach();
    d->kerning = enable;
    resolve_mask |= QFont::KerningResolved;
}

/*!
    Returns the StyleStrategy.

    The style strategy affects the \l{QFont}{font matching} algorithm.
    See \l QFont::StyleStrategy for the list of available strategies.
*/
QFont::StyleStrategy QFont::styleStrategy() const
{
    return d->request.styleStrategy;
}

/*!
    \enum QFont::StyleStrategy

    The style strategy tells the \l{QFont}{font matching} algorithm
    what type of fonts should be used to find an appropriate default
    family.

    The following strategies are available:

    \value PreferDefault the default style strategy. It does not prefer
           any type of font.
    \value PreferBitmap prefers bitmap fonts (as opposed to outline
           fonts).
    \value PreferOutline prefers outline fonts (as opposed to bitmap fonts).
    \value ForceOutline forces the use of outline fonts.
    \value NoAntialias don't antialias the fonts.
    \value PreferAntialias antialias if possible.

    Any of these may be OR-ed with one of these flags:

    \value ForceIntegerMetrics forces the use of integer values in font engines that support fractional
           font metrics.
*/

/*!
    Sets the style strategy for the font to \a s.

    \sa QFont::StyleStrategy
*/
void QFont::setStyleStrategy(StyleStrategy s)
{
    detach();

    if ((resolve_mask & QFont::StyleStrategyResolved) && s == d->request.styleStrategy)
        return;

    d->request.styleStrategy = s;
    resolve_mask |= QFont::StyleStrategyResolved;
}


/*!
    \enum QFont::Stretch

    Predefined stretch values that follow the CSS naming convention. The higher
    the value, the more stretched the text is.

    \value UltraCondensed 50
    \value ExtraCondensed 62
    \value Condensed 75
    \value SemiCondensed 87
    \value Unstretched 100
    \value SemiExpanded 112
    \value Expanded 125
    \value ExtraExpanded 150
    \value UltraExpanded 200

    \sa setStretch() stretch()
*/

/*!
    Returns the stretch factor for the font.

    \sa setStretch()
 */
int QFont::stretch() const
{
    return d->request.stretch;
}

/*!
    Sets the stretch factor for the font.

    The stretch factor changes the width of all characters in the font
    by \a factor percent.  For example, setting \a factor to 150
    results in all characters in the font being 1.5 times (ie. 150%)
    wider.  The default stretch factor is 100.  The minimum stretch
    factor is 1, and the maximum stretch factor is 4000.

    The stretch factor is only applied to outline fonts.  The stretch
    factor is ignored for bitmap fonts.

    \sa stretch() QFont::Stretch
*/
void QFont::setStretch(int factor)
{
    if (factor < 1 || factor > 4000) {
        qWarning("QFont::setStretch: Parameter '%d' out of range", factor);
        return;
    }

    if ((resolve_mask & QFont::StretchResolved) && d->request.stretch == factor)
        return;

    detach();

    d->request.stretch = factor;
    resolve_mask |= QFont::StretchResolved;
}

/*!
    \enum QFont::SpacingType
    \since 4.4

    \value PercentageSpacing  A value of 100 will keep the spacing unchanged; a value of 200 will enlarge the
                                                   spacing after a character by the width of the character itself.
    \value AbsoluteSpacing      A positive value increases the letter spacing by the corresponding pixels; a negative
                                                   value decreases the spacing.
*/

/*!
    \since 4.4
    Returns the letter spacing for the font.

    \sa setLetterSpacing(), letterSpacingType(), setWordSpacing()
 */
qreal QFont::letterSpacing() const
{
    return d->letterSpacing.toReal();
}

/*!
    \since 4.4
    Sets the letter spacing for the font to \a spacing and the type
    of spacing to \a type.

    Letter spacing changes the default spacing between individual
    letters in the font.  The spacing between the letters can be
    made smaller as well as larger.

    \sa letterSpacing(), letterSpacingType(), setWordSpacing()
*/
void QFont::setLetterSpacing(SpacingType type, qreal spacing)
{
    const QFixed newSpacing = QFixed::fromReal(spacing);
    const bool absoluteSpacing = type == AbsoluteSpacing;
    if ((resolve_mask & QFont::LetterSpacingResolved) &&
        d->letterSpacingIsAbsolute == absoluteSpacing &&
        d->letterSpacing == newSpacing)
        return;

    detach();

    d->letterSpacing = newSpacing;
    d->letterSpacingIsAbsolute = absoluteSpacing;
    resolve_mask |= QFont::LetterSpacingResolved;
}

/*!
    \since 4.4
    Returns the spacing type used for letter spacing.

    \sa letterSpacing(), setLetterSpacing(), setWordSpacing()
*/
QFont::SpacingType QFont::letterSpacingType() const
{
    return d->letterSpacingIsAbsolute ? AbsoluteSpacing : PercentageSpacing;
}

/*!
    \since 4.4
    Returns the word spacing for the font.

    \sa setWordSpacing(), setLetterSpacing()
 */
qreal QFont::wordSpacing() const
{
    return d->wordSpacing.toReal();
}

/*!
    \since 4.4
    Sets the word spacing for the font to \a spacing.

    Word spacing changes the default spacing between individual
    words. A positive value increases the word spacing
    by a corresponding amount of pixels, while a negative value
    decreases the inter-word spacing accordingly.

    Word spacing will not apply to writing systems, where indiviaul
    words are not separated by white space.

    \sa wordSpacing(), setLetterSpacing()
*/
void QFont::setWordSpacing(qreal spacing)
{
    const QFixed newSpacing = QFixed::fromReal(spacing);
    if ((resolve_mask & QFont::WordSpacingResolved) &&
        d->wordSpacing == newSpacing)
        return;

    detach();

    d->wordSpacing = newSpacing;
    resolve_mask |= QFont::WordSpacingResolved;
}

/*!
    Returns true if a window system font exactly matching the settings
    of this font is available.

    \sa QFontInfo
*/
bool QFont::exactMatch() const
{
    QFontEngine *engine = d->engineForScript(QUnicodeTables::Common);
    Q_ASSERT(engine != 0);
    return d->request.exactMatch(engine->fontDef);
}

/*!
    Returns true if this font is equal to \a f; otherwise returns
    false.

    Two QFonts are considered equal if their font attributes are
    equal.

    \sa operator!=() isCopyOf()
*/
bool QFont::operator==(const QFont &f) const
{
    return (f.d == d
            || (f.d->request   == d->request
                && f.d->request.pointSize == d->request.pointSize
                && f.d->underline == d->underline
                && f.d->overline  == d->overline
                && f.d->strikeOut == d->strikeOut
                && f.d->kerning == d->kerning
                && f.d->letterSpacingIsAbsolute == d->letterSpacingIsAbsolute
                && f.d->letterSpacing == d->letterSpacing
                && f.d->wordSpacing == d->wordSpacing
            ));
}


/*!
    Provides an arbitrary comparison of this font and font \a f.
    All that is guaranteed is that the operator returns false if both
    fonts are equal and that (f1 \< f2) == !(f2 \< f1) if the fonts
    are not equal.

    This function is useful in some circumstances, for example if you
    want to use QFont objects as keys in a QMap.

    \sa operator==() operator!=() isCopyOf()
*/
bool QFont::operator<(const QFont &f) const
{
    if (f.d == d) return false;
    // the < operator for fontdefs ignores point sizes.
    QFontDef &r1 = f.d->request;
    QFontDef &r2 = d->request;
    if (r1.pointSize != r2.pointSize) return r1.pointSize < r2.pointSize;
    if (r1.pixelSize != r2.pixelSize) return r1.pixelSize < r2.pixelSize;
    if (r1.weight != r2.weight) return r1.weight < r2.weight;
    if (r1.style != r2.style) return r1.style < r2.style;
    if (r1.stretch != r2.stretch) return r1.stretch < r2.stretch;
    if (r1.styleStrategy != r2.styleStrategy) return r1.styleStrategy < r2.styleStrategy;
    if (r1.family != r2.family) return r1.family < r2.family;

    if (f.d->letterSpacingIsAbsolute != d->letterSpacingIsAbsolute) return f.d->letterSpacingIsAbsolute < d->letterSpacingIsAbsolute;
    if (f.d->letterSpacing != d->letterSpacing) return f.d->letterSpacing < d->letterSpacing;
    if (f.d->wordSpacing != d->wordSpacing) return f.d->wordSpacing < d->wordSpacing;

    int f1attrs = (f.d->underline << 3) + (f.d->overline << 2) + (f.d->strikeOut<<1) + f.d->kerning;
    int f2attrs = (d->underline << 3) + (d->overline << 2) + (d->strikeOut<<1) + d->kerning;
    return f1attrs < f2attrs;
}


/*!
    Returns true if this font is different from \a f; otherwise
    returns false.

    Two QFonts are considered to be different if their font attributes
    are different.

    \sa operator==()
*/
bool QFont::operator!=(const QFont &f) const
{
    return !(operator==(f));
}

/*!
   Returns the font as a QVariant
*/
QFont::operator QVariant() const
{
    return QVariant(QVariant::Font, this);
}

/*!
    Returns true if this font and \a f are copies of each other, i.e.
    one of them was created as a copy of the other and neither has
    been modified since. This is much stricter than equality.

    \sa operator=() operator==()
*/
bool QFont::isCopyOf(const QFont & f) const
{
    return d == f.d;
}

/*!
    Returns a new QFont that has attributes copied from \a other that
    have not been previously set on this font.
*/
QFont QFont::resolve(const QFont &other) const
{
    if (*this == other
        && (resolve_mask == other.resolve_mask || resolve_mask == 0)
        && d->dpi == other.d->dpi) {
        QFont o = other;
        o.resolve_mask = resolve_mask;
        return o;
    }

    QFont font(*this);
    font.detach();
    font.d->resolve(resolve_mask, other.d.data());

    return font;
}

/*!
    \fn uint QFont::resolve() const
    \internal
*/

/*!
    \fn void QFont::resolve(uint mask)
    \internal
*/

/*! \fn void QFont::initialize()
  \internal

  Internal function that initializes the font system.  The font cache
  and font dict do not alloc the keys. The key is a QString which is
  shared between QFontPrivate and QXFontName.
*/

/*! \fn void QFont::cleanup()
  \internal

  Internal function that cleans up the font system.
*/


/*  \internal
    Internal function. Converts boolean font settings to an unsigned
    8-bit number. Used for serialization etc.
*/
static inline quint8 get_font_bits(int version, const QFontPrivate *f)
{
    Q_ASSERT(f != 0);
    quint8 bits = 0;
    if (f->request.style)
        bits |= 0x01;
    if (f->underline)
        bits |= 0x02;
    if (f->overline)
        bits |= 0x40;
    if (f->strikeOut)
        bits |= 0x04;
    if (f->request.fixedPitch)
        bits |= 0x08;
    if (f->kerning)
        bits |= 0x10;
    if (f->request.style == QFont::StyleOblique)
        bits |= 0x80;
    return bits;
}

static inline quint8 get_extended_font_bits(const QFontPrivate *f)
{
    Q_ASSERT(f != 0);
    quint8 bits = 0;
    if (f->request.ignorePitch)
        bits |= 0x01;
    if (f->letterSpacingIsAbsolute)
        bits |= 0x02;
    return bits;
}

#ifndef QT_NO_DATASTREAM

/*  \internal
    Internal function. Sets boolean font settings from an unsigned
    8-bit number. Used for serialization etc.
*/
static void set_font_bits(quint8 bits, QFontPrivate *f)
{
    Q_ASSERT(f != 0);
    f->request.style         = (bits & 0x01) != 0 ? QFont::StyleItalic : QFont::StyleNormal;
    f->underline             = (bits & 0x02) != 0;
    f->overline              = (bits & 0x40) != 0;
    f->strikeOut             = (bits & 0x04) != 0;
    f->request.fixedPitch    = (bits & 0x08) != 0;
    f->kerning               = (bits & 0x10) != 0;
    if ((bits & 0x80) != 0)
        f->request.style         = QFont::StyleOblique;
}

static void set_extended_font_bits(quint8 bits, QFontPrivate *f)
{
    Q_ASSERT(f != 0);
    f->request.ignorePitch = (bits & 0x01) != 0;
    f->letterSpacingIsAbsolute = (bits & 0x02) != 0;
}
#endif


/*!
    Returns the font's key, a textual representation of a font. It is
    typically used as the key for a cache or dictionary of fonts.

    \sa QMap
*/
QString QFont::key() const
{
    return toString();
}

/*!
    Returns a description of the font. The description is a
    comma-separated list of the attributes, perfectly suited for use
    in QSettings.

    \sa fromString()
 */
QString QFont::toString() const
{
    const QLatin1Char comma(',');
    return family() + comma +
        QString::number(     pointSizeF()) + comma +
        QString::number(      pixelSize()) + comma +
        QString::number(         weight()) + comma +
        QString::number((int)     style()) + comma +
        QString::number((int) underline()) + comma +
        QString::number((int) strikeOut()) + comma +
        QString::number((int)fixedPitch());
}


/*!
    Sets this font to match the description \a descrip. The description
    is a comma-separated list of the font attributes, as returned by
    toString().

    \sa toString()
 */
bool QFont::fromString(const QString &descrip)
{
    QStringList l(descrip.split(QLatin1Char(',')));

    int count = l.count();
    if (!count || count > 8) {
        qWarning("QFont::fromString: Invalid description '%s'",
                 descrip.isEmpty() ? "(empty)" : descrip.toLatin1().data());
        return false;
    }

    setFamily(l[0]);
    if (count > 1 && l[1].toDouble() > 0.0)
        setPointSizeF(l[1].toDouble());
    if (count == 8) {
        if (l[2].toInt() > 0)
            setPixelSize(l[2].toInt());
        setWeight(qMax(qMin(99, l[3].toInt()), 0));
        setItalic(l[4].toInt());
        setUnderline(l[5].toInt());
        setStrikeOut(l[6].toInt());
        setFixedPitch(l[7].toInt());

        if (!d->request.fixedPitch) // assume 'false' fixedPitch equals default
            d->request.ignorePitch = true;
    }

    return true;
}


/*****************************************************************************
  QFont stream functions
 *****************************************************************************/
#ifndef QT_NO_DATASTREAM

/*!
    \relates QFont

    Writes the font \a font to the data stream \a s. (toString()
    writes to a text stream.)

    \sa \link datastreamformat.html Format of the QDataStream operators \endlink
*/
QDataStream &operator<<(QDataStream &s, const QFont &font)
{
    s << font.d->request.family;

    double pointSize = font.d->request.pointSize;
    double pixelSize = font.d->request.pixelSize;
    s << pointSize;
    s << pixelSize;

    s << (qint8) font.d->request.styleStrategy;
    s << (qint8) font.d->request.weight
      << get_font_bits(s.version(), font.d.data());
    s << (qint16)font.d->request.stretch;
    s << get_extended_font_bits(font.d.data());
    s << font.d->letterSpacing.value();
    s << font.d->wordSpacing.value();
    return s;
}


/*!
    \relates QFont

    Reads the font \a font from the data stream \a s. (fromString()
    reads from a text stream.)

    \sa \link datastreamformat.html Format of the QDataStream operators \endlink
*/
QDataStream &operator>>(QDataStream &s, QFont &font)
{
    font.d = new QFontPrivate;
    font.resolve_mask = QFont::AllPropertiesResolved;

    qint8 styleStrategy = QFont::PreferDefault, bits;
    qint8 weight;

    s >> font.d->request.family;

    double pointSize;
    double pixelSize;
    s >> pointSize;
    s >> pixelSize;
    font.d->request.pointSize = qreal(pointSize);
    font.d->request.pixelSize = qreal(pixelSize);
    s >> styleStrategy;

    s >> weight;
    s >> bits;

    font.d->request.styleStrategy = QFont::StyleStrategy(styleStrategy);
    font.d->request.weight = weight;

    set_font_bits(bits, font.d.data());

    qint16 stretch;
    s >> stretch;
    font.d->request.stretch = stretch;

    quint8 extendedBits;
    s >> extendedBits;
    set_extended_font_bits(extendedBits, font.d.data());

    int value;
    s >> value;
    font.d->letterSpacing.setValue(value);
    s >> value;
    font.d->wordSpacing.setValue(value);

    return s;
}

#endif // QT_NO_DATASTREAM


/*****************************************************************************
  QFontInfo member functions
 *****************************************************************************/

/*!
    \class QFontInfo
    \reentrant

    \brief The QFontInfo class provides general information about fonts.

    \ingroup appearance
    \ingroup shared

    The QFontInfo class provides the same access functions as QFont,
    e.g. family(), pointSize(), italic(), weight(), fixedPitch(),
    etc. But whilst the QFont access functions return the values that
    were set, a QFontInfo object returns the values that apply to
    the font that will actually be used to draw the text.

    For example, when the program asks for a 25pt Courier font on a
    machine that has a non-scalable 24pt Courier font, QFont will
    (normally) use the 24pt Courier for rendering. In this case,
    QFont::pointSize() returns 25 and QFontInfo::pointSize() returns
    24.

    There are three ways to create a QFontInfo object.
    \list 1
    \o Calling the QFontInfo constructor with a QFont creates a font
    info object for a screen-compatible font, i.e. the font cannot be
    a printer font. If the font is changed later, the font
    info object is \e not updated.

    (Note: If you use a printer font the values returned may be
    inaccurate. Printer fonts are not always accessible so the nearest
    screen font is used if a printer font is supplied.)

    \o QWidget::fontInfo() returns the font info for a widget's font.
    This is equivalent to calling QFontInfo(widget->font()). If the
    widget's font is changed later, the font info object is \e not
    updated.

    \o QPainter::fontInfo() returns the font info for a painter's
    current font. If the painter's font is changed later, the font
    info object is \e not updated.
    \endlist

    \sa QFont QFontMetrics QFontDatabase
*/

/*!
    Constructs a font info object for \a font.

    The font must be screen-compatible, i.e. a font you use when
    drawing text in \link QWidget widgets\endlink or \link QPixmap
    pixmaps\endlink, not QPrinter.

    The font info object holds the information for the font that is
    passed in the constructor at the time it is created, and is not
    updated if the font's attributes are changed later.

    Use QPainter::fontInfo() to get the font info when painting.
    This will give correct results also when painting on paint device
    that is not screen-compatible.
*/
QFontInfo::QFontInfo(const QFont &font)
    : d(font.d.data())
{
}

/*!
    Constructs a copy of \a fi.
*/
QFontInfo::QFontInfo(const QFontInfo &fi)
    : d(fi.d.data())
{
}

/*!
    Destroys the font info object.
*/
QFontInfo::~QFontInfo()
{
}

/*!
    Assigns the font info in \a fi.
*/
QFontInfo &QFontInfo::operator=(const QFontInfo &fi)
{
    d = fi.d.data();
    return *this;
}

/*!
    Returns the family name of the matched window system font.

    \sa QFont::family()
*/
QString QFontInfo::family() const
{
    QFontEngine *engine = d->engineForScript(QUnicodeTables::Common);
    Q_ASSERT(engine != 0);
    return engine->fontDef.family;
}

/*!
    \since 4.8

    Returns the style name of the matched window system font on
    system that supports it.

    \sa QFont::styleName()
*/
QString QFontInfo::styleName() const
{
    QFontEngine *engine = d->engineForScript(QUnicodeTables::Common);
    Q_ASSERT(engine != 0);
    return engine->fontDef.styleName;
}

/*!
    Returns the point size of the matched window system font.

    \sa pointSizeF() QFont::pointSize()
*/
int QFontInfo::pointSize() const
{
    QFontEngine *engine = d->engineForScript(QUnicodeTables::Common);
    Q_ASSERT(engine != 0);
    return qRound(engine->fontDef.pointSize);
}

/*!
    Returns the point size of the matched window system font.

    \sa QFont::pointSizeF()
*/
qreal QFontInfo::pointSizeF() const
{
    QFontEngine *engine = d->engineForScript(QUnicodeTables::Common);
    Q_ASSERT(engine != 0);
    return engine->fontDef.pointSize;
}

/*!
    Returns the pixel size of the matched window system font.

    \sa QFont::pointSize()
*/
int QFontInfo::pixelSize() const
{
    QFontEngine *engine = d->engineForScript(QUnicodeTables::Common);
    Q_ASSERT(engine != 0);
    return engine->fontDef.pixelSize;
}

/*!
    Returns the italic value of the matched window system font.

    \sa QFont::italic()
*/
bool QFontInfo::italic() const
{
    QFontEngine *engine = d->engineForScript(QUnicodeTables::Common);
    Q_ASSERT(engine != 0);
    return engine->fontDef.style != QFont::StyleNormal;
}

/*!
    Returns the style value of the matched window system font.

    \sa QFont::style()
*/
QFont::Style QFontInfo::style() const
{
    QFontEngine *engine = d->engineForScript(QUnicodeTables::Common);
    Q_ASSERT(engine != 0);
    return engine->fontDef.style;
}

/*!
    Returns the weight of the matched window system font.

    \sa QFont::weight(), bold()
*/
int QFontInfo::weight() const
{
    QFontEngine *engine = d->engineForScript(QUnicodeTables::Common);
    Q_ASSERT(engine != 0);
    return engine->fontDef.weight;

}

/*!
    \fn bool QFontInfo::bold() const

    Returns true if weight() would return a value greater than
    QFont::Normal; otherwise returns false.

    \sa weight(), QFont::bold()
*/

/*!
    Returns the underline value of the matched window system font.

  \sa QFont::underline()

  \internal

  Here we read the underline flag directly from the QFont.
  This is OK for X11 and for Windows because we always get what we want.
*/
bool QFontInfo::underline() const
{
    return d->underline;
}

/*!
    Returns the overline value of the matched window system font.

    \sa QFont::overline()

    \internal

    Here we read the overline flag directly from the QFont.
    This is OK for X11 and for Windows because we always get what we want.
*/
bool QFontInfo::overline() const
{
    return d->overline;
}

/*!
    Returns the strikeout value of the matched window system font.

  \sa QFont::strikeOut()

  \internal Here we read the strikeOut flag directly from the QFont.
  This is OK for X11 and for Windows because we always get what we want.
*/
bool QFontInfo::strikeOut() const
{
    return d->strikeOut;
}

/*!
    Returns the fixed pitch value of the matched window system font.

    \sa QFont::fixedPitch()
*/
bool QFontInfo::fixedPitch() const
{
    QFontEngine *engine = d->engineForScript(QUnicodeTables::Common);
    Q_ASSERT(engine != 0);
    return engine->fontDef.fixedPitch;
}

/*!
    Returns true if the matched window system font is exactly the same
    as the one specified by the font; otherwise returns false.

    \sa QFont::exactMatch()
*/
bool QFontInfo::exactMatch() const
{
    QFontEngine *engine = d->engineForScript(QUnicodeTables::Common);
    Q_ASSERT(engine != 0);
    return d->request.exactMatch(engine->fontDef);
}




// **********************************************************************
// QFontCache
thread_local QFontCache* theFontCache = nullptr;

QFontCache *QFontCache::instance()
{
    if (!theFontCache)
        theFontCache = new QFontCache;
    return theFontCache;
}

void QFontCache::cleanup()
{
    if (theFontCache) {
        delete theFontCache;
        theFontCache = 0;
    }
}

QFontCache::QFontCache()
{
}

QFontCache::~QFontCache()
{
    clear();
    {
        EngineDataCache::ConstIterator it = engineDataCache.constBegin(),
                                 end = engineDataCache.constEnd();
        while (it != end) {
            if (!it.value()->ref.deref())
                delete it.value();
            else
                FC_DEBUG("QFontCache::~QFontCache: engineData %p still has refcount %d",
                         it.value(), int(it.value()->ref));
            ++it;
        }
    }
}

void QFontCache::clear()
{
    {
        EngineDataCache::Iterator it = engineDataCache.begin(),
                                 end = engineDataCache.end();
        while (it != end) {
            QFontEngineData *data = it.value();
            for (int i = 0; i < QUnicodeTables::ScriptCount; ++i) {
                if (data->engines[i] && !data->engines[i]->ref.deref())
                    delete data->engines[i];
                data->engines[i] = 0;
            }
            ++it;
        }
    }

    for (EngineCache::Iterator it = engineCache.begin(), end = engineCache.end();
         it != end; ++it) {
        QFontEngine *engine = it.value();
        if (!engine->ref.deref()) {
            delete engine;
        }
    }

    engineCache.clear();
}


QFontEngineData *QFontCache::findEngineData(const Key &key) const
{
    EngineDataCache::ConstIterator it = engineDataCache.find(key),
                                  end = engineDataCache.end();
    if (it == end) {
        return nullptr;
    }

    // found
    return it.value();
}

void QFontCache::insertEngineData(const Key &key, QFontEngineData *engineData)
{
    FC_DEBUG("QFontCache: inserting new engine data %p", engineData);

    Q_ASSERT(!engineDataCache.contains(key));
    engineData->ref.ref(); // the cache has a reference
    engineDataCache.insert(key, engineData);
}

QFontEngine *QFontCache::findEngine(const Key &key)
{
    EngineCache::Iterator it = engineCache.find(key),
                         end = engineCache.end();
    if (it == end) {
        return nullptr;
    }

    FC_DEBUG("QFontCache: found font engine\n"
             "  %p: ref %2d, type '%s'",
             it.value(), int(it.value()->ref), it.value()->name());

    return it.value();
}

void QFontCache::insertEngine(const Key &key, QFontEngine *engine)
{
    FC_DEBUG("QFontCache: inserting new engine %p", engine);

    QFontEngine *oldEngine = engineCache.value(key);
    engine->ref.ref(); // the cache has a reference
    if (oldEngine && !oldEngine->ref.deref())
        delete oldEngine;

    engineCache.insert(key, engine);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug stream, const QFont &font)
{
    return stream << "QFont(" << font.toString() << ')';
}
#endif

QT_END_NAMESPACE

#include "moc_qfont.h"

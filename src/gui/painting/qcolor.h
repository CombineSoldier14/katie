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

#ifndef QCOLOR_H
#define QCOLOR_H

#include <QtGui/qrgb.h>
#include <QtCore/qnamespace.h>
#include <QtCore/qstringlist.h>


QT_BEGIN_NAMESPACE


class QColormap;
class QVariant;

class Q_GUI_EXPORT QColor
{
public:
    enum Spec { Invalid, Rgb, Hsv };

    QColor();
    QColor(Qt::GlobalColor color);
    QColor(int r, int g, int b, int a = 255);
    QColor(QRgb rgb);
    QColor(const QString& name);
    QColor(const char *name);
    QColor(const QColor &color);
    QColor(Spec spec);

    bool isValid() const;

    QString name() const;
    bool setNamedColor(const QString& name);

    static QStringList colorNames();

    inline Spec spec() const
    { return cspec; }

    int alpha() const;
    void setAlpha(int alpha);

    qreal alphaF() const;
    void setAlphaF(qreal alpha);

    int red() const;
    int green() const;
    int blue() const;
    void setRed(int red);
    void setGreen(int green);
    void setBlue(int blue);

    qreal redF() const;
    qreal greenF() const;
    qreal blueF() const;
    void setRedF(qreal red);
    void setGreenF(qreal green);
    void setBlueF(qreal blue);

    void getRgb(int *r, int *g, int *b, int *a = nullptr) const;
    void setRgb(int r, int g, int b, int a = 255);

    void getRgbF(qreal *r, qreal *g, qreal *b, qreal *a = nullptr) const;
    void setRgbF(qreal r, qreal g, qreal b, qreal a = 1.0);

    QRgb rgba() const;
    void setRgba(QRgb rgba);

    QRgb rgb() const;
    void setRgb(QRgb rgb);

    int hue() const; // 0 <= hue < 360
    int saturation() const;
    int hsvHue() const; // 0 <= hue < 360
    int hsvSaturation() const;
    int value() const;

    qreal hueF() const; // 0.0 <= hueF < 360.0
    qreal saturationF() const;
    qreal hsvHueF() const; // 0.0 <= hueF < 360.0
    qreal hsvSaturationF() const;
    qreal valueF() const;

    void getHsv(int *h, int *s, int *v, int *a = nullptr) const;
    void setHsv(int h, int s, int v, int a = 255);

    void getHsvF(qreal *h, qreal *s, qreal *v, qreal *a = nullptr) const;
    void setHsvF(qreal h, qreal s, qreal v, qreal a = 1.0);

    QColor toRgb() const;
    QColor toHsv() const;

    static QColor fromRgb(QRgb rgb);
    static QColor fromRgba(QRgb rgba);

    static QColor fromRgb(int r, int g, int b, int a = 255);
    static QColor fromRgbF(qreal r, qreal g, qreal b, qreal a = 1.0);

    static QColor fromHsv(int h, int s, int v, int a = 255);
    static QColor fromHsvF(qreal h, qreal s, qreal v, qreal a = 1.0);

    QColor light(int f = 150) const;
    QColor lighter(int f = 150) const;
    QColor dark(int f = 200) const;
    QColor darker(int f = 200) const;

    QColor &operator=(const QColor &);
    QColor &operator=(Qt::GlobalColor color);

    bool operator==(const QColor &c) const;
    bool operator!=(const QColor &c) const;

    operator QVariant() const;

#ifdef Q_WS_X11
    static bool allowX11ColorNames();
    static void setAllowX11ColorNames(bool enabled);
#endif

    static bool isValidColor(const QString &name);

private:
    // do not allow a spec to be used as an alpha value
    QColor(int, int, int, Spec);

    void invalidate();

    Spec cspec;
    union {
        struct {
            ushort alpha;
            ushort red;
            ushort green;
            ushort blue;
        } argb;
        struct {
            ushort alpha;
            ushort hue;
            ushort saturation;
            ushort value;
        } ahsv;
    } ct;

    friend class QColormap;
#ifndef QT_NO_DATASTREAM
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QColor &);
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QColor &);
#endif
};

inline QColor::QColor()
{ invalidate(); }

inline QColor::QColor(int r, int g, int b, int a)
{ setRgb(r, g, b, a); }

inline QColor::QColor(const char *aname)
{ setNamedColor(QLatin1String(aname)); }

inline QColor::QColor(const QString& aname)
{ setNamedColor(aname); }

inline QColor::QColor(const QColor &acolor)
    : cspec(acolor.cspec)
{ ct.argb = acolor.ct.argb; }

inline bool QColor::isValid() const
{ return cspec != Invalid; }

inline QColor QColor::lighter(int f) const 
{ return light(f); }

inline QColor QColor::darker(int f) const 
{ return dark(f); }

#ifndef QT_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const QColor &);
#endif
#ifndef QT_NO_DATASTREAM
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QColor &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QColor &);
#endif

QT_END_NAMESPACE


#endif // QCOLOR_H

/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QTEXTSTREAM_H
#define QTEXTSTREAM_H

#include <QtCore/qlocale.h>

#include <stdio.h>


QT_BEGIN_NAMESPACE

class QTextCodec;
class QTextDecoder;

class QTextStreamPrivate;
class Q_CORE_EXPORT QTextStream                                // text stream class
{
    Q_DECLARE_PRIVATE(QTextStream)

public:
    enum RealNumberNotation {
        SmartNotation,
        FixedNotation,
        ScientificNotation
    };
    enum FieldAlignment {
        AlignLeft,
        AlignRight,
        AlignCenter,
        AlignAccountingStyle
    };
    enum TextStatus {
        Ok,
        ReadPastEnd,
        ReadCorruptData,
        WriteFailed
    };
    enum NumberFlag {
        ShowBase = 0x1,
        ForcePoint = 0x2,
        ForceSign = 0x4,
        UppercaseBase = 0x8,
        UppercaseDigits = 0x10
    };
    Q_DECLARE_FLAGS(NumberFlags, NumberFlag)

    QTextStream();
    explicit QTextStream(QIODevice *device);
    explicit QTextStream(FILE *fileHandle, QIODevice::OpenMode openMode = QIODevice::ReadWrite);
    explicit QTextStream(QString *string, QIODevice::OpenMode openMode = QIODevice::ReadWrite);
    explicit QTextStream(QByteArray *array, QIODevice::OpenMode openMode = QIODevice::ReadWrite);
    explicit QTextStream(const QByteArray &array, QIODevice::OpenMode openMode = QIODevice::ReadOnly);
    virtual ~QTextStream();

#ifndef QT_NO_TEXTCODEC
    void setCodec(QTextCodec *codec);
    void setCodec(const char *codecName);
    QTextCodec *codec() const;
    void setAutoDetectUnicode(bool enabled);
    bool autoDetectUnicode() const;
    void setGenerateByteOrderMark(bool generate);
    bool generateByteOrderMark() const;
#endif

    void setLocale(const QLocale &locale);
    QLocale locale() const;

    void setDevice(QIODevice *device);
    QIODevice *device() const;

    void setString(QString *string, QIODevice::OpenMode openMode = QIODevice::ReadWrite);
    QString *string() const;

    TextStatus status() const;
    void setStatus(TextStatus status);
    void resetStatus();

    bool atEnd() const;
    void reset();
    void flush();
    bool seek(qint64 pos);
    qint64 pos() const;

    void skipWhiteSpace();

    QString readLine(qint64 maxlen = 0);
    QString readAll();
    QString read(qint64 maxlen);

    void setFieldAlignment(FieldAlignment alignment);
    FieldAlignment fieldAlignment() const;

    void setPadChar(QChar ch);
    QChar padChar() const;

    void setFieldWidth(int width);
    int fieldWidth() const;

    void setNumberFlags(NumberFlags flags);
    NumberFlags numberFlags() const;

    void setIntegerBase(int base);
    int integerBase() const;

    void setRealNumberNotation(RealNumberNotation notation);
    RealNumberNotation realNumberNotation() const;

    void setRealNumberPrecision(int precision);
    int realNumberPrecision() const;

    QTextStream &operator>>(QChar &ch);
    QTextStream &operator>>(char &ch);
    QTextStream &operator>>(signed short &i);
    QTextStream &operator>>(unsigned short &i);
    QTextStream &operator>>(signed int &i);
    QTextStream &operator>>(unsigned int &i);
    QTextStream &operator>>(signed long &i);
    QTextStream &operator>>(unsigned long &i);
    QTextStream &operator>>(qlonglong &i);
    QTextStream &operator>>(qulonglong &i);
    QTextStream &operator>>(float &f);
    QTextStream &operator>>(double &f);
    QTextStream &operator>>(QString &s);
    QTextStream &operator>>(QByteArray &array);
    QTextStream &operator>>(char *c);

    QTextStream &operator<<(QChar ch);
    QTextStream &operator<<(char ch);
    QTextStream &operator<<(signed short i);
    QTextStream &operator<<(unsigned short i);
    QTextStream &operator<<(signed int i);
    QTextStream &operator<<(unsigned int i);
    QTextStream &operator<<(signed long i);
    QTextStream &operator<<(unsigned long i);
    QTextStream &operator<<(qlonglong i);
    QTextStream &operator<<(qulonglong i);
    QTextStream &operator<<(float f);
    QTextStream &operator<<(double f);
    QTextStream &operator<<(const QString &s);
    QTextStream &operator<<(const QByteArray &array);
    QTextStream &operator<<(const char *c);
    QTextStream &operator<<(const void *ptr);


private:

    Q_DISABLE_COPY(QTextStream)

    QTextStreamPrivate* d_ptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QTextStream::NumberFlags)

/*****************************************************************************
  QTextStream manipulators
 *****************************************************************************/

typedef QTextStream & (*QTextStreamFunction)(QTextStream &);// manipulator function
typedef void (QTextStream::*QTSMFI)(int); // manipulator w/int argument
typedef void (QTextStream::*QTSMFC)(QChar); // manipulator w/QChar argument

class Q_CORE_EXPORT QTextStreamManipulator
{
public:
    QTextStreamManipulator(QTSMFI m, int a) { mf = m; mc = Q_NULLPTR; arg = a; }
    QTextStreamManipulator(QTSMFC m, QChar c) { mf = Q_NULLPTR; mc = m; ch = c; arg = -1; }
    void exec(QTextStream &s) { if (mf) { (s.*mf)(arg); } else { (s.*mc)(ch); } }

private:
    QTSMFI mf;                                        // QTextStream member function
    QTSMFC mc;                                        // QTextStream member function
    int arg;                                          // member function argument
    QChar ch;
};

inline QTextStream &operator>>(QTextStream &s, QTextStreamFunction f)
{ return (*f)(s); }

inline QTextStream &operator<<(QTextStream &s, QTextStreamFunction f)
{ return (*f)(s); }

inline QTextStream &operator<<(QTextStream &s, QTextStreamManipulator m)
{ m.exec(s); return s; }

Q_CORE_EXPORT QTextStream &bin(QTextStream &s);
Q_CORE_EXPORT QTextStream &oct(QTextStream &s);
Q_CORE_EXPORT QTextStream &dec(QTextStream &s);
Q_CORE_EXPORT QTextStream &hex(QTextStream &s);

Q_CORE_EXPORT QTextStream &showbase(QTextStream &s);
Q_CORE_EXPORT QTextStream &forcesign(QTextStream &s);
Q_CORE_EXPORT QTextStream &forcepoint(QTextStream &s);
Q_CORE_EXPORT QTextStream &noshowbase(QTextStream &s);
Q_CORE_EXPORT QTextStream &noforcesign(QTextStream &s);
Q_CORE_EXPORT QTextStream &noforcepoint(QTextStream &s);

Q_CORE_EXPORT QTextStream &uppercasebase(QTextStream &s);
Q_CORE_EXPORT QTextStream &uppercasedigits(QTextStream &s);
Q_CORE_EXPORT QTextStream &lowercasebase(QTextStream &s);
Q_CORE_EXPORT QTextStream &lowercasedigits(QTextStream &s);

Q_CORE_EXPORT QTextStream &fixed(QTextStream &s);
Q_CORE_EXPORT QTextStream &scientific(QTextStream &s);

Q_CORE_EXPORT QTextStream &left(QTextStream &s);
Q_CORE_EXPORT QTextStream &right(QTextStream &s);
Q_CORE_EXPORT QTextStream &center(QTextStream &s);

Q_CORE_EXPORT QTextStream &endl(QTextStream &s);
Q_CORE_EXPORT QTextStream &flush(QTextStream &s);
Q_CORE_EXPORT QTextStream &reset(QTextStream &s);

Q_CORE_EXPORT QTextStream &bom(QTextStream &s);

Q_CORE_EXPORT QTextStream &ws(QTextStream &s);

inline QTextStreamManipulator qSetFieldWidth(int width)
{
    QTSMFI func = &QTextStream::setFieldWidth;
    return QTextStreamManipulator(func,width);
}

inline QTextStreamManipulator qSetPadChar(QChar ch)
{
    QTSMFC func = &QTextStream::setPadChar;
    return QTextStreamManipulator(func, ch);
}

inline QTextStreamManipulator qSetRealNumberPrecision(int precision)
{
    QTSMFI func = &QTextStream::setRealNumberPrecision;
    return QTextStreamManipulator(func, precision);
}


QT_END_NAMESPACE


#endif // QTEXTSTREAM_H

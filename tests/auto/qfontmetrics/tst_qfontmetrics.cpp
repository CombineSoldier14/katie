/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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


#include <QtTest/QtTest>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qfontdatabase.h>
#include <qstringlist.h>
#include <qlist.h>



//TESTED_CLASS=
//TESTED_FILES=

class tst_QFontMetrics : public QObject
{
Q_OBJECT

public:
    tst_QFontMetrics();
    virtual ~tst_QFontMetrics();

public slots:
    void init();
    void cleanup();
private slots:
    void same();
    void metrics();
    void boundingRect();
    void elidedText_data();
    void elidedText();
    void veryNarrowElidedText();
    void averageCharWidth();
    void inFontUcs4();
    void lineWidth();
};

tst_QFontMetrics::tst_QFontMetrics()

{
}

tst_QFontMetrics::~tst_QFontMetrics()
{

}

void tst_QFontMetrics::init()
{
}

void tst_QFontMetrics::cleanup()
{
}

void tst_QFontMetrics::same()
{
    QFont font;
    font.setBold(true);
    QFontMetrics fm(font);
    const QString text = QLatin1String("Some stupid STRING");
    QCOMPARE(fm.size(0, text), fm.size(0, text)) ;

    {
        QImage image;
        QFontMetrics fm2(font, &image);
        QString text2 =  QLatin1String("Foo Foo");
        QCOMPARE(fm2.size(0, text2), fm2.size(0, text2));  //used to crash
    }

    {
        QImage image;
        QFontMetricsF fm3(font, &image);
        QString text2 =  QLatin1String("Foo Foo");
        QCOMPARE(fm3.size(0, text2), fm3.size(0, text2));  //used to crash
    }
}


void tst_QFontMetrics::metrics()
{
    QFont font;
    QFontDatabase fdb;

    // Query the QFontDatabase for a specific font, store the
    // result in family, style and size.
    QStringList families = fdb.families();
    if (families.isEmpty())
        return;

    QStringList::ConstIterator f_it, f_end = families.end();
    for (f_it = families.begin(); f_it != f_end; ++f_it) {
        const QString &family = *f_it;

        QStringList styles = fdb.styles(family);
        QStringList::ConstIterator s_it, s_end = styles.end();
        for (s_it = styles.begin(); s_it != s_end; ++s_it) {
            const QString &style = *s_it;

            if (fdb.isSmoothlyScalable(family, style)) {
                // smoothly scalable font... don't need to load every pointsize
                font = fdb.font(family, style, 12);

                QFontMetrics fontmetrics(font);
                QCOMPARE(fontmetrics.ascent() + fontmetrics.descent() + 1,
                        fontmetrics.height());

                QCOMPARE(fontmetrics.height() + fontmetrics.leading(),
                        fontmetrics.lineSpacing());
            } else {
                QList<int> sizes = fdb.pointSizes(family, style);
                QVERIFY(!sizes.isEmpty());
                QList<int>::ConstIterator z_it, z_end = sizes.end();
                for (z_it = sizes.begin(); z_it != z_end; ++z_it) {
                    const int size = *z_it;

                    // Initialize the font, and check if it is an exact match
                    font = fdb.font(family, style, size);

                    QFontMetrics fontmetrics(font);
                    QCOMPARE(fontmetrics.ascent() + fontmetrics.descent() + 1,
                            fontmetrics.height());
                    QCOMPARE(fontmetrics.height() + fontmetrics.leading(),
                            fontmetrics.lineSpacing());
                }
            }
	}
    }
}

void tst_QFontMetrics::boundingRect()
{
    QFont f;
    f.setPointSize(24);
    QFontMetrics fm(f);
    QRect r = fm.boundingRect(QChar('Y'));
    QVERIFY(r.top() < 0);
    r = fm.boundingRect(QString("Y"));
    QVERIFY(r.top() < 0);
}

void tst_QFontMetrics::elidedText_data()
{
    QTest::addColumn<QFont>("font");
    QTest::addColumn<QString>("text");

    foreach (const int psize, QFontDatabase::standardSizes()) {
        QTest::newRow(QString::fromLatin1("freesans hello (%1)").arg(psize).toLatin1()) << QFont("freesans", psize) << QString("hello");
        QTest::newRow(QString::fromLatin1("freesans hello &Bye (%1)").arg(psize).toLatin1()) << QFont("freesans", psize) << QString("hello&Bye");
    }
}


void tst_QFontMetrics::elidedText()
{
    QFETCH(QFont, font);
    QFETCH(QString, text);
    QFontMetrics fm(font);
    int w = fm.width(text);
    QString newtext = fm.elidedText(text,Qt::ElideRight,w+1, 0);
    QCOMPARE(text,newtext); // should not elide
    newtext = fm.elidedText(text,Qt::ElideRight,w-1, 0);
    QVERIFY(text != newtext); // should elide
}

void tst_QFontMetrics::veryNarrowElidedText()
{
    QFont f;
    QFontMetrics fm(f);
    QString text("hello");
    QCOMPARE(fm.elidedText(text, Qt::ElideRight, 0), QString());

    // Not even wide enough for "small" - should use ellipsis
    QString text1 = QString::fromLatin1("small");
    QChar ellipsisChar(0x2026);
    QString text1_el = QString::fromLatin1("s") + ellipsisChar;
    int text1_el_width = fm.width(text1_el);
    QCOMPARE(fm.elidedText(text1,Qt::ElideRight, text1_el_width + 1), text1_el);
}

void tst_QFontMetrics::averageCharWidth()
{
    QFont f;
    QFontMetrics fm(f);
    QVERIFY(fm.averageCharWidth() != 0);
    QFontMetricsF fmf(f);
    QVERIFY(fmf.averageCharWidth() != 0);
}

void tst_QFontMetrics::inFontUcs4()
{
    QFont font(":/fonts/ucs4font.ttf");
    {
        QFontMetrics fm(font);

        QVERIFY(fm.inFontUcs4(0x1D7FF));
    }

    {
        QFontMetricsF fm(font);

        QVERIFY(fm.inFontUcs4(0x1D7FF));
    }
}

void tst_QFontMetrics::lineWidth()
{
    // QTBUG-13009, QTBUG-13011
    QFont smallFont;
    smallFont.setPointSize(8);
    smallFont.setWeight(QFont::Light);
    const QFontMetrics smallFontMetrics(smallFont);

    QFont bigFont;
    bigFont.setPointSize(40);
    bigFont.setWeight(QFont::Black);
    const QFontMetrics bigFontMetrics(bigFont);

    QVERIFY(smallFontMetrics.lineWidth() >= 1);
    QVERIFY(smallFontMetrics.lineWidth() < bigFontMetrics.lineWidth());
}

QTEST_MAIN(tst_QFontMetrics)

#include "moc_tst_qfontmetrics.cpp"
#include "qrc_testfont.cpp"

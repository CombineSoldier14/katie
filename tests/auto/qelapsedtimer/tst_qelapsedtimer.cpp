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

#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtCore/QElapsedTimer>
#include <QtCore/QDebug>
#include <QtTest/QtTest>

static const int minResolution = 50; // the minimum resolution for the tests

QDebug operator<<(QDebug s, const QElapsedTimer &t)
{
    s.nospace() << "(" <<  t.msecsSinceReference() << ")";
    return s.space();
}

class tst_QElapsedTimer : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void statics();
    void validity();
    void basics();
    void elapsed();
};

void tst_QElapsedTimer::statics()
{
    qDebug() << "Clock type is" << QElapsedTimer::clockType();
    qDebug() << "Said clock is" << (QElapsedTimer::isMonotonic() ? "monotonic" : "not monotonic");
    QElapsedTimer t;
    t.start();
    qDebug() << "Current time is" << t.msecsSinceReference();
}

void tst_QElapsedTimer::validity()
{
    QElapsedTimer t;

    QVERIFY(!t.isValid());

    t.start();
    QVERIFY(t.isValid());

    t.invalidate();
    QVERIFY(!t.isValid());
}

void tst_QElapsedTimer::basics()
{
    QElapsedTimer t1;
    t1.start();

    QVERIFY(t1.msecsSinceReference() != 0);

    QCOMPARE(t1, t1);
    QVERIFY(!(t1 != t1));
    QVERIFY(!(t1 < t1));
    QCOMPARE(t1.msecsTo(t1), qint64(0));
    QCOMPARE(t1.secsTo(t1), qint64(0));
//    QCOMPARE(t1 + 0, t1);
//    QCOMPARE(t1 - 0, t1);

#if 0
    QElapsedTimer t2 = t1;
    t2 += 1000;   // so we can use secsTo

    QVERIFY(t1 != t2);
    QVERIFY(!(t1 == t2));
    QVERIFY(t1 < t2);
    QVERIFY(!(t2 < t1));
    QCOMPARE(t1.msecsTo(t2), qint64(1000));
    QCOMPARE(t1.secsTo(t2), qint64(1));
//    QCOMPARE(t2 - t1, qint64(1000));
//    QCOMPARE(t1 - t2, qint64(-1000));
#endif

    quint64 value1 = t1.msecsSinceReference();
    qDebug() << value1 << t1;
    qint64 nsecs = t1.nsecsElapsed();
    qint64 elapsed = t1.restart();
    QVERIFY(elapsed < minResolution);
    QVERIFY(nsecs / 1000000 < minResolution);

    quint64 value2 = t1.msecsSinceReference();
    qDebug() << value2 << t1 << elapsed << nsecs;
    // in theory, elapsed == value2 - value1

    // However, since QElapsedTimer keeps internally the full resolution,
    // we have here a rounding error due to integer division
    QVERIFY(qAbs(elapsed - qint64(value2 - value1)) <= 1);
}

void tst_QElapsedTimer::elapsed()
{
    QElapsedTimer t1;
    t1.start();

    QTest::qSleep(4*minResolution);
    QElapsedTimer t2;
    t2.start();

    QVERIFY(t1 != t2);
    QVERIFY(!(t1 == t2));
    QVERIFY(t1 < t2);
    QVERIFY(t1.msecsTo(t2) > 0);
    // don't check: t1.secsTo(t2)
//    QVERIFY(t1 - t2 < 0);

    QVERIFY(t1.nsecsElapsed() > 0);
    QVERIFY(t1.elapsed() > 0);
    // the number of elapsed nanoseconds and milliseconds should match
    QVERIFY(t1.nsecsElapsed() - t1.elapsed() * 1000000 < 1000000);
    QVERIFY(t1.hasExpired(minResolution));
    QVERIFY(!t1.hasExpired(8*minResolution));
    QVERIFY(!t2.hasExpired(minResolution));

    QVERIFY(!t1.hasExpired(-1));
    QVERIFY(!t2.hasExpired(-1));

    qint64 elapsed = t1.restart();
    QVERIFY(elapsed > 3*minResolution);
    QVERIFY(elapsed < 5*minResolution);
    qint64 diff = t2.msecsTo(t1);
    QVERIFY(diff < minResolution);
}

QTEST_MAIN(tst_QElapsedTimer);

#include "moc_tst_qelapsedtimer.cpp"

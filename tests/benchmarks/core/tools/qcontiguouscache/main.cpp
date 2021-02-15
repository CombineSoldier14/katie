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

#include <QObject>
#include <QTest>
#include <QCache>
#include <QContiguousCache>
#include <QDebug>

#include <stdio.h>

QT_USE_NAMESPACE

class tst_QContiguousCache : public QObject
{
    Q_OBJECT
private slots:
    void asScrollingList();
    void cacheBenchmark();
    void contiguousCacheBenchmark();
};

QTEST_MAIN(tst_QContiguousCache)

void tst_QContiguousCache::asScrollingList()
{
    int i;
    QContiguousCache<int> c(10);

    // Once allocated QContiguousCache should not
    // allocate any additional memory for non
    // complex data types.
    QBENCHMARK {
        // simulate scrolling in a list of items;
        for (i = 0; i < 10; ++i) {
            QCOMPARE(c.available(), 10-i);
            c.append(i);
        }

        QCOMPARE(c.firstIndex(), 0);
        QCOMPARE(c.lastIndex(), 9);
        QCOMPARE(c.first(), 0);
        QCOMPARE(c.last(), 9);
        QVERIFY(!c.containsIndex(-1));
        QVERIFY(!c.containsIndex(10));
        QCOMPARE(c.available(), 0);

        for (i = 0; i < 10; ++i) {
            QVERIFY(c.containsIndex(i));
            QCOMPARE(c.at(i), i);
            QCOMPARE(c[i], i);
            QCOMPARE(((const QContiguousCache<int>)c)[i], i);
        }

        for (i = 10; i < 30; ++i)
            c.append(i);

        QCOMPARE(c.firstIndex(), 20);
        QCOMPARE(c.lastIndex(), 29);
        QCOMPARE(c.first(), 20);
        QCOMPARE(c.last(), 29);
        QVERIFY(!c.containsIndex(19));
        QVERIFY(!c.containsIndex(30));
        QCOMPARE(c.available(), 0);

        for (i = 20; i < 30; ++i) {
            QVERIFY(c.containsIndex(i));
            QCOMPARE(c.at(i), i);
            QCOMPARE(c[i], i);
            QCOMPARE(((const QContiguousCache<int> )c)[i], i);
        }

        for (i = 19; i >= 10; --i)
            c.prepend(i);

        QCOMPARE(c.firstIndex(), 10);
        QCOMPARE(c.lastIndex(), 19);
        QCOMPARE(c.first(), 10);
        QCOMPARE(c.last(), 19);
        QVERIFY(!c.containsIndex(9));
        QVERIFY(!c.containsIndex(20));
        QCOMPARE(c.available(), 0);

        for (i = 10; i < 20; ++i) {
            QVERIFY(c.containsIndex(i));
            QCOMPARE(c.at(i), i);
            QCOMPARE(c[i], i);
            QCOMPARE(((const QContiguousCache<int> )c)[i], i);
        }

        for (i = 200; i < 220; ++i)
            c.insert(i, i);

        QCOMPARE(c.firstIndex(), 210);
        QCOMPARE(c.lastIndex(), 219);
        QCOMPARE(c.first(), 210);
        QCOMPARE(c.last(), 219);
        QVERIFY(!c.containsIndex(209));
        QVERIFY(!c.containsIndex(300));
        QCOMPARE(c.available(), 0);

        for (i = 210; i < 220; ++i) {
            QVERIFY(c.containsIndex(i));
            QCOMPARE(c.at(i), i);
            QCOMPARE(c[i], i);
            QCOMPARE(((const QContiguousCache<int> )c)[i], i);
        }
        c.clear(); // needed to reset benchmark
    }

    // from a specific bug that was encountered.  100 to 299 cached, attempted to cache 250 - 205 via insert, failed.
    // bug was that item at 150 would instead be item that should have been inserted at 250
    c.setCapacity(200);
    for (i = 100; i < 300; ++i)
        c.insert(i, i);
    for (i = 250; i <= 306; ++i)
        c.insert(i, 1000+i);
    for (i = 107; i <= 306; ++i) {
        QVERIFY(c.containsIndex(i));
        QCOMPARE(c.at(i), i < 250 ? i : 1000+i);
    }
}

/*
    Benchmarks must be near identical in tasks to be fair.
    QCache uses pointers to ints as its a requirement of QCache,
    whereas QContiguousCache doesn't support pointers (won't free them).
    Given the ability to use simple data types is a benefit, its
    fair.  Although this obviously must take into account we are
    testing QContiguousCache use cases here, QCache has its own
    areas where it is the more sensible class to use.
*/
void tst_QContiguousCache::cacheBenchmark()
{
    QBENCHMARK {
        QCache<int, int> cache;
        cache.setMaxCost(100);

        for (int i = 0; i < 1000; i++)
            cache.insert(i, new int(i));
    }
}

void tst_QContiguousCache::contiguousCacheBenchmark()
{
    QBENCHMARK {
        QContiguousCache<int> contiguousCache(100);
        for (int i = 0; i < 1000; i++)
            contiguousCache.insert(i, i);
    }
}

#include "moc_main.cpp"

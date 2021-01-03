/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
#include <QString>

#include <qtest.h>

QT_USE_NAMESPACE

class tst_associative_containers : public QObject
{
    Q_OBJECT
private slots:
    void insert_data();
    void insert();
    void lookup_data();
    void lookup();
};

template <typename T> 
void testInsert(int size)
{
    T container;

    QBENCHMARK {
        for (int i = 0; i < size; ++i)
            container.insert(i, i);
    }
}

void tst_associative_containers::insert_data()
{
    QTest::addColumn<bool>("useHash");
    QTest::addColumn<int>("size");

    for (int size = 10; size < 20000; size += 100) {
    
        const QByteArray sizeString = QByteArray::number(size);

        QTest::newRow(QByteArray("hash--" + sizeString).constData()) << true << size;
        QTest::newRow(QByteArray("map--" + sizeString).constData()) << false << size;
    }
}

void tst_associative_containers::insert()
{
    QFETCH(bool, useHash);
    QFETCH(int, size);

    QHash<int, int> testHash;
    QMap<int, int> testMap;

    if (useHash) {
        testInsert<QHash<int, int> >(size);        
    } else {
        testInsert<QMap<int, int> >(size);        
    }
}

void tst_associative_containers::lookup_data()
{
//    setReportType(LineChartReport);
//    setChartTitle("Time to call value(), with an increasing number of items in the container");

    QTest::addColumn<bool>("useHash");
    QTest::addColumn<int>("size");

    for (int size = 10; size < 20000; size += 100) {
    
        const QByteArray sizeString = QByteArray::number(size);

        QTest::newRow(QByteArray("hash--" + sizeString).constData()) << true << size;
        QTest::newRow(QByteArray("map--" + sizeString).constData()) << false << size;
    }
}

template <typename T> 
void testLookup(int size)
{
    T container;
    
    for (int i = 0; i < size; ++i)
        container.insert(i, i);

    int val;

    QBENCHMARK {
        for (int i = 0; i < size; ++i)
            val = container.value(i);
    }
    Q_UNUSED(val);
}

void tst_associative_containers::lookup()
{
    QFETCH(bool, useHash);
    QFETCH(int, size);

    if (useHash) {
        testLookup<QHash<int, int> >(size);
    } else {
        testLookup<QMap<int, int> >(size);
    }
}

QTEST_MAIN(tst_associative_containers)

#include "moc_main.cpp"

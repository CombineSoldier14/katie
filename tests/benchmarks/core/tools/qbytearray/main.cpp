/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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
#include <QDebug>
#include <QIODevice>
#include <QFile>
#include <QString>
#include <qtest.h>


QT_USE_NAMESPACE

static const QByteArray lorem = QByteArray("Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.");

class tst_qbytearray : public QObject
{
    Q_OBJECT
private slots:
    void append();
    void append_data();
    void compress_uncompress_data();
    void compress_uncompress();
    void fast_compress_uncompress_data();
    void fast_compress_uncompress();
    void compress_uncompress_diff_data();
    void compress_uncompress_diff();
};


void tst_qbytearray::append_data()
{
    QTest::addColumn<int>("size");
    QTest::newRow("1")         << int(1);
    QTest::newRow("10")        << int(10);
    QTest::newRow("100")       << int(100);
    QTest::newRow("1000")      << int(1000);
    QTest::newRow("10000")     << int(10000);
    QTest::newRow("100000")    << int(100000);
    QTest::newRow("1000000")   << int(1000000);
    QTest::newRow("10000000")  << int(10000000);
    QTest::newRow("100000000") << int(100000000);
}

void tst_qbytearray::append()
{
    QFETCH(int, size);

    QByteArray ba;
    QBENCHMARK {
        QByteArray ba2(size, 'x');
        ba.append(ba2);
        ba.clear();
    }
}

void tst_qbytearray::compress_uncompress_data()
{
    QTest::addColumn<int>("level");
    QTest::newRow("-1") << int(-1);
    QTest::newRow("0")  << int(0);
    QTest::newRow("1")  << int(1);
    QTest::newRow("2")  << int(2);
    QTest::newRow("3")  << int(3);
    QTest::newRow("4")  << int(4);
    QTest::newRow("5")  << int(5);
    QTest::newRow("6")  << int(6);
    QTest::newRow("7")  << int(7);
    QTest::newRow("8")  << int(8);
    QTest::newRow("9")  << int(9);
}

void tst_qbytearray::compress_uncompress()
{
    QFETCH(int, level);

    QBENCHMARK {
        QByteArray compressed = qCompress(lorem, level);
        QVERIFY(!compressed.isEmpty());
        QByteArray decompressed = qUncompress(compressed);
        QVERIFY(decompressed == lorem);
    }
}

void tst_qbytearray::fast_compress_uncompress_data() {
    compress_uncompress_data();
}

void tst_qbytearray::fast_compress_uncompress() {
    QFETCH(int, level);

    QBENCHMARK {
        QByteArray compressed = qFastCompress(lorem, level);
        QVERIFY(!compressed.isEmpty());
        QByteArray decompressed = qFastUncompress(compressed);
        QVERIFY(decompressed == lorem);
    }
}

void tst_qbytearray::compress_uncompress_diff_data() {
    compress_uncompress_data();
}

void tst_qbytearray::compress_uncompress_diff() {
    QFETCH(int, level);

    QByteArray compressed = qCompress(lorem, level);
    QVERIFY(!compressed.isEmpty());
    QByteArray fastcompressed = qFastCompress(lorem, level);
    QVERIFY(!fastcompressed.isEmpty());
    qDebug() << "compressed" << compressed.size() << "fastcompressed" << fastcompressed.size();
}

QTEST_MAIN(tst_qbytearray)

#include "moc_main.cpp"

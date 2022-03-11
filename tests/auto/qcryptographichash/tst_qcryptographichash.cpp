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
#include <QCryptographicHash>
#include <QDebug>

class tst_QCryptographicHash : public QObject
{
    Q_OBJECT
private slots:
    void repeated_result_data();
    void repeated_result();
    void intermediary_result_data();
    void intermediary_result();
};

void tst_QCryptographicHash::repeated_result_data()
{
    intermediary_result_data();
}

void tst_QCryptographicHash::repeated_result()
{
    QFETCH(int, algo);
    QCryptographicHash::Algorithm _algo = QCryptographicHash::Algorithm(algo);
    QCryptographicHash hash(_algo);

    QFETCH(QByteArray, first);
    hash.addData(first);

    QFETCH(QByteArray, hash_first);
    QByteArray result = hash.result();
    QCOMPARE(result, hash_first);
    QCOMPARE(result, hash.result());

    hash.reset();
    hash.addData(first);
    result = hash.result();
    QCOMPARE(result, hash_first);
    QCOMPARE(result, hash.result());
}

void tst_QCryptographicHash::intermediary_result_data()
{
    QTest::addColumn<int>("algo");
    QTest::addColumn<QByteArray>("first");
    QTest::addColumn<QByteArray>("second");
    QTest::addColumn<QByteArray>("hash_first");
    QTest::addColumn<QByteArray>("hash_firstsecond");

    QTest::newRow("md5") << int(QCryptographicHash::Md5)
                         << QByteArray("abc") << QByteArray("abc")
                         << QByteArray::fromHex("900150983CD24FB0D6963F7D28E17F72")
                         << QByteArray::fromHex("440AC85892CA43AD26D44C7AD9D47D3E");
    QTest::newRow("sha1") << int(QCryptographicHash::Sha1)
                          << QByteArray("abc") << QByteArray("abc")
                          << QByteArray::fromHex("A9993E364706816ABA3E25717850C26C9CD0D89D")
                          << QByteArray::fromHex("F8C1D87006FBF7E5CC4B026C3138BC046883DC71");

    QTest::newRow("sha256") << int(QCryptographicHash::Sha256)
                          << QByteArray("abc") << QByteArray("abc")
                          << QByteArray::fromHex("BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD")
                          << QByteArray::fromHex("BBB59DA3AF939F7AF5F360F2CEB80A496E3BAE1CD87DDE426DB0AE40677E1C2C");

    QTest::newRow("sha512") << int(QCryptographicHash::Sha512)
                          << QByteArray("abc") << QByteArray("abc")
                          << QByteArray::fromHex("DDAF35A193617ABACC417349AE20413112E6FA4E89A97EA20A9EEEE64B55D39A2192992A274FC1A836BA3C23A3FEEBBD454D4423643CE80E2A9AC94FA54CA49F")
                          << QByteArray::fromHex("F3C41E7B63EE869596FC28BAD64120612C520F65928AB4D126C72C6998B551B8FF1CEDDFED4373E6717554DC89D1EEE6F0AB22FD3675E561ABA9AE26A3EEC53B");
    QTest::newRow("BLAKE2b") << int(QCryptographicHash::BLAKE2b)
                          << QByteArray("abc") << QByteArray("abc")
                          << QByteArray::fromHex("ba80a53f981c4d0d6a2797b69f12f6e94c212f14685ac4b74b12bb6fdbffa2d17d87c5392aab792dc252d5de4533cc9518d38aa8dbf1925ab92386edd4009923")
                          << QByteArray::fromHex("60b5c037082a21e1b92bc3484d8bbe015a56574bdf3517796dd9a32095bec69af06104d0391076e9329d150b0ec925af7c482faf4d66127f38b6a65bec4d695b");
    QTest::newRow("BLAKE2s") << int(QCryptographicHash::BLAKE2s)
                          << QByteArray("abc") << QByteArray("abc")
                          << QByteArray::fromHex("508c5e8c327c14e2e1a72ba34eeb452f37458b209ed63a294d999b4c86675982")
                          << QByteArray::fromHex("3605affd00f95791174cf1f50f90c40aa094dfc5e80898f014729b23eabe6415");
}

void tst_QCryptographicHash::intermediary_result()
{
    QFETCH(int, algo);
    QCryptographicHash::Algorithm _algo = QCryptographicHash::Algorithm(algo);
    QCryptographicHash hash(_algo);

    QFETCH(QByteArray, first);
    hash.addData(first);

    QFETCH(QByteArray, hash_first);
    QByteArray result = hash.result();
    // qDebug() result.toHex();
    QCOMPARE(result, hash_first);

    // don't reset
    QFETCH(QByteArray, second);
    QFETCH(QByteArray, hash_firstsecond);
    hash.addData(second);

    result = hash.result();
    // qDebug() << result.toHex();
    QCOMPARE(result, hash_firstsecond);

    hash.reset();
}

QTEST_MAIN(tst_QCryptographicHash)

#include "moc_tst_qcryptographichash.cpp"

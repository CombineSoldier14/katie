/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
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
#include <QHostInfo>
#include <QStringList>
#include <QString>

#include <qtest.h>
#include <qtesteventloop.h>

#include "qhostinfo_p.h"

class tst_qhostinfo : public QObject
{
    Q_OBJECT
public slots:
    void init();
private slots:
    void lookupSpeed_data();
    void lookupSpeed();
};

class SignalReceiver : public QObject
{
    Q_OBJECT
public:
    SignalReceiver(int nrc) : receiveCount(0), neededReceiveCount(nrc) {};
    int receiveCount;
    int neededReceiveCount;
public slots:
    void resultsReady(const QHostInfo) {
        receiveCount++;
        if (receiveCount == neededReceiveCount)
            QTestEventLoop::instance().exitLoop();
    }
};

void tst_qhostinfo::init()
{
    // delete the cache so inidividual testcase results are independant from each other
    qt_qhostinfo_clear_cache();
}

void tst_qhostinfo::lookupSpeed_data()
{
    QTest::addColumn<bool>("cache");
    QTest::newRow("WithCache") << true;
    QTest::newRow("WithoutCache") << false;
}

void tst_qhostinfo::lookupSpeed()
{
    QFETCH(bool, cache);
    qt_qhostinfo_enable_cache(cache);

    QStringList hostnameList;
    hostnameList << QLatin1String("www.ovi.com")
                 << QLatin1String("www.nokia.com")
                 << QLatin1String("qt.nokia.com")
                 << QLatin1String("www.trolltech.com")
                 << QLatin1String("troll.no")
                 << QLatin1String("www.qtcentre.org")
                 << QLatin1String("forum.nokia.com")
                 << QLatin1String("www.forum.nokia.com")
                 << QLatin1String("wiki.forum.nokia.com")
                 << QLatin1String("www.nokia.no")
                 << QLatin1String("nokia.de")
                 << QLatin1String("127.0.0.1")
                 << QLatin1String("----");
    // also add some duplicates:
    hostnameList << QLatin1String("www.nokia.com")
                 << QLatin1String("127.0.0.1")
                 << QLatin1String("www.trolltech.com");
    // and some more
    hostnameList << hostnameList;

    const int COUNT = hostnameList.size();

    SignalReceiver receiver(COUNT);

    QBENCHMARK {
        for (int i = 0; i < hostnameList.size(); i++)
            QHostInfo::lookupHost(hostnameList.at(i), &receiver, SLOT(resultsReady(const QHostInfo)));
        QTestEventLoop::instance().enterLoop(20);
        QVERIFY(!QTestEventLoop::instance().timeout());
    }
}


QTEST_MAIN(tst_qhostinfo)

#include "moc_main.cpp"

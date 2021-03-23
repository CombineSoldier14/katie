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


#include <QtTest/QtTest>


#include <qabstracteventdispatcher.h>
#include <qcoreapplication.h>
#include <qcoreevent.h>
#include <qeventloop.h>
#include <qmutex.h>
#include <qthread.h>
#include <qtimer.h>
#include <qwaitcondition.h>
#include <QTcpServer>
#include <QTcpSocket>

#include "../../shared/util.h"

//TESTED_CLASS=
//TESTED_FILES=

class EventLoopExiter : public QObject
{
    Q_OBJECT
    QEventLoop *eventLoop;
public:
    inline EventLoopExiter(QEventLoop *el)
        : eventLoop(el)
    { }
public slots:
    void exit();
    void exit1();
    void exit2();
};

void EventLoopExiter::exit()
{ eventLoop->exit(); }

void EventLoopExiter::exit1()
{ eventLoop->exit(1); }

void EventLoopExiter::exit2()
{ eventLoop->exit(2); }

class EventLoopThread : public QThread
{
    Q_OBJECT
signals:
    void checkPoint();
public:
    QEventLoop *eventLoop;
    void run();
};

void EventLoopThread::run()
{
    eventLoop = new QEventLoop;
    emit checkPoint();
    (void) eventLoop->exec();
    delete eventLoop;
    eventLoop = 0;
}

class MultipleExecThread : public QThread
{
    Q_OBJECT
signals:
    void checkPoint();
public:
    QMutex mutex;
    QWaitCondition cond;
    volatile int result1;
    volatile int result2;
    MultipleExecThread() : result1(0xdead), result2(0xbeef) {}

    void run()
    {
        QMutexLocker locker(&mutex);
        // this exec should work

        cond.wakeOne();
        cond.wait(&mutex);

        QTimer timer;
        connect(&timer, SIGNAL(timeout()), SLOT(quit()), Qt::DirectConnection);
        timer.setInterval(1000);
        timer.start();
        result1 = exec();

        // this should return immediately, since exit() has been called
        cond.wakeOne();
        cond.wait(&mutex);
        QEventLoop eventLoop;
        result2 = eventLoop.exec();
    }
};

class StartStopEvent: public QEvent
{
public:
    StartStopEvent(int type, QEventLoop *loop = 0)
        : QEvent(Type(type)), el(loop)
    { }

    QEventLoop *el;
};

class EventLoopExecutor : public QObject
{
    Q_OBJECT
    QEventLoop *eventLoop;
public:
    int returnCode;
    EventLoopExecutor(QEventLoop *eventLoop)
        : QObject(), eventLoop(eventLoop), returnCode(-42)
    {
    }
public slots:
    void exec()
    {
        QTimer::singleShot(100, eventLoop, SLOT(quit()));
        // this should return immediately, and the timer event should be delivered to
        // tst_QEventLoop::exec() test, letting the test complete
        returnCode = eventLoop->exec();
    }
};

#if 0 // ndef QT_NO_EXCEPTIONS
class QEventLoopTestException { };

class ExceptionThrower : public QObject
{
    Q_OBJECT
public:
    ExceptionThrower() : QObject() { }
public slots:
    void throwException()
    {
        QEventLoopTestException e;
        throw e;
    }
};
#endif

class tst_QEventLoop : public QObject
{
    Q_OBJECT
public:
    tst_QEventLoop();
    ~tst_QEventLoop();
public slots:
    void init();
    void cleanup();
private slots:
    void processEvents();
    void exec();
    void reexec();
    void exit();
    void execAfterExit();
    void wakeUp();
    void quit();
    void processEventsExcludeSocket();
    void processEventsExcludeTimers();
    void deliverInDefinedOrder_QTBUG19637();

    // keep this test last:
    void nestedLoops();

protected:
    void customEvent(QEvent *e);
};

tst_QEventLoop::tst_QEventLoop()
{ }

tst_QEventLoop::~tst_QEventLoop()
{ }

void tst_QEventLoop::init()
{ }

void tst_QEventLoop::cleanup()
{ }

void tst_QEventLoop::processEvents()
{
    QSignalSpy spy1(QAbstractEventDispatcher::instance(), SIGNAL(aboutToBlock()));
    QSignalSpy spy2(QAbstractEventDispatcher::instance(), SIGNAL(awake()));

    QEventLoop eventLoop;

    QCoreApplication::postEvent(&eventLoop, new QEvent(QEvent::User));

    // process posted events, QEventLoop::processEvents() should return
    // true
    QVERIFY(eventLoop.processEvents());
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 1);

    // allow any session manager to complete its handshake, so that
    // there are no pending events left.
    while (eventLoop.processEvents())
        ;

    // On mac we get application started events at this point,
    // so process events one more time just to be sure.
    eventLoop.processEvents();

    // no events to process, QEventLoop::processEvents() should return
    // false
    spy1.clear();
    spy2.clear();
    QVERIFY(!eventLoop.processEvents());
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 1);

    // make sure the test doesn't block forever
    int timerId = startTimer(100);

    // wait for more events to process, QEventLoop::processEvents()
    // should return true
    spy1.clear();
    spy2.clear();
    QVERIFY(eventLoop.processEvents(QEventLoop::WaitForMoreEvents));

    // Verify that the eventloop has blocked and woken up. Some eventloops
    // may block and wake up multiple times.
    QVERIFY(spy1.count() > 0);
    QVERIFY(spy2.count() > 0);
    // We should get one awake for each aboutToBlock, plus one awake when
    // processEvents is entered.
    QVERIFY(spy2.count() >= spy1.count());

    killTimer(timerId);
}

#define EXEC_TIMEOUT 100

void tst_QEventLoop::exec()
{
    {
        QEventLoop eventLoop;
        EventLoopExiter exiter(&eventLoop);
        int returnCode;

        QTimer::singleShot(EXEC_TIMEOUT, &exiter, SLOT(exit()));
        returnCode = eventLoop.exec();
        QCOMPARE(returnCode, 0);

        QTimer::singleShot(EXEC_TIMEOUT, &exiter, SLOT(exit1()));
        returnCode = eventLoop.exec();
        QCOMPARE(returnCode, 1);

        QTimer::singleShot(EXEC_TIMEOUT, &exiter, SLOT(exit2()));
        returnCode = eventLoop.exec();
        QCOMPARE(returnCode, 2);
    }

    {
        // calling QEventLoop::exec() after a thread loop has exit()ed should return immediately
        // Note: this behaviour differs from QCoreApplication and QEventLoop
        // see tst_QCoreApplication::eventLoopExecAfterExit, tst_QEventLoop::reexec
        MultipleExecThread thread;

        // start thread and wait for checkpoint
        thread.mutex.lock();
        thread.start();
        thread.cond.wait(&thread.mutex);

        // make sure the eventloop runs
        QSignalSpy spy(QAbstractEventDispatcher::instance(&thread), SIGNAL(awake()));
        thread.cond.wakeOne();
        thread.cond.wait(&thread.mutex);
        QVERIFY(spy.count() > 0);
        int v = thread.result1;
        QCOMPARE(v, 0);

        // exec should return immediately
        spy.clear();
        thread.cond.wakeOne();
        thread.mutex.unlock();
        thread.wait();
        QCOMPARE(spy.count(), 0);
        v = thread.result2;
        QCOMPARE(v, -1);
    }

    {
        // a single instance of QEventLoop should not be allowed to recurse into exec()
        QEventLoop eventLoop;
        EventLoopExecutor executor(&eventLoop);

        QTimer::singleShot(EXEC_TIMEOUT, &executor, SLOT(exec()));
        int returnCode = eventLoop.exec();
        QCOMPARE(returnCode, 0);
        QCOMPARE(executor.returnCode, -1);
    }

#if 0 // !defined(QT_NO_EXCEPTIONS)
    {
        // QEventLoop::exec() is exception safe
        QEventLoop eventLoop;
        int caughtExceptions = 0;

        try {
            ExceptionThrower exceptionThrower;
            QTimer::singleShot(EXEC_TIMEOUT, &exceptionThrower, SLOT(throwException()));
            (void) eventLoop.exec();
        } catch (...) {
            ++caughtExceptions;
        }
        try {
            ExceptionThrower exceptionThrower;
            QTimer::singleShot(EXEC_TIMEOUT, &exceptionThrower, SLOT(throwException()));
            (void) eventLoop.exec();
        } catch (...) {
            ++caughtExceptions;
        }
        QCOMPARE(caughtExceptions, 2);
    }
#endif
}

void tst_QEventLoop::reexec()
{
    QEventLoop loop;

    // exec once
    QMetaObject::invokeMethod(&loop, "quit", Qt::QueuedConnection);
    QCOMPARE(loop.exec(), 0);

    // and again
    QMetaObject::invokeMethod(&loop, "quit", Qt::QueuedConnection);
    QCOMPARE(loop.exec(), 0);
}

void tst_QEventLoop::exit()
{ DEPENDS_ON(exec()); }

void tst_QEventLoop::execAfterExit()
{
    QEventLoop loop;
    EventLoopExiter obj(&loop);

    QMetaObject::invokeMethod(&obj, "exit", Qt::QueuedConnection);
    loop.exit(1);
    QCOMPARE(loop.exec(), 0);
}

void tst_QEventLoop::wakeUp()
{
    EventLoopThread thread;
    QEventLoop eventLoop;
    connect(&thread, SIGNAL(checkPoint()), &eventLoop, SLOT(quit()));
    connect(&thread, SIGNAL(finished()), &eventLoop, SLOT(quit()));

    thread.start();
    (void) eventLoop.exec();

    QSignalSpy spy(QAbstractEventDispatcher::instance(&thread), SIGNAL(awake()));
    thread.eventLoop->wakeUp();

    // give the thread time to wake up
    QTimer::singleShot(1000, &eventLoop, SLOT(quit()));
    (void) eventLoop.exec();

    QVERIFY(spy.count() > 0);

    thread.quit();
    (void) eventLoop.exec();
}

void tst_QEventLoop::quit()
{
    QEventLoop eventLoop;
    int returnCode;

    QTimer::singleShot(100, &eventLoop, SLOT(quit()));
    returnCode = eventLoop.exec();
    QCOMPARE(returnCode, 0);
}


void tst_QEventLoop::nestedLoops()
{
    QCoreApplication::postEvent(this, new StartStopEvent(QEvent::User));
    QCoreApplication::postEvent(this, new StartStopEvent(QEvent::User));
    QCoreApplication::postEvent(this, new StartStopEvent(QEvent::User));

    // without the fix, this will *wedge* and never return
    QTest::qWait(1000);
}

void tst_QEventLoop::customEvent(QEvent *e)
{
    if (e->type() == QEvent::User) {
        QEventLoop loop;
        QCoreApplication::postEvent(this, new StartStopEvent(int(QEvent::User) + 1, &loop));
        loop.exec();
    } else {
        static_cast<StartStopEvent *>(e)->el->exit();
    }
}

class SocketEventsTester: public QObject
{
    Q_OBJECT
public:
    SocketEventsTester()
    {
        socket = 0;
        server = 0;
        dataArrived = false;
        testResult = false;
    }
    ~SocketEventsTester()
    {
        delete socket;
        delete server;
    }
    bool init()
    {
        bool ret = false;
        server = new QTcpServer();
        socket = new QTcpSocket();
        connect(server, SIGNAL(newConnection()), this, SLOT(sendHello()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(sendAck()), Qt::DirectConnection);
        if((ret = server->listen(QHostAddress::LocalHost, 0))) {
            socket->connectToHost(server->serverAddress(), server->serverPort());
            socket->waitForConnected();
        }
        return ret;
    }

    QTcpSocket *socket;
    QTcpServer *server;
    bool dataArrived;
    bool testResult;
public slots:
    void sendAck()
    {
        dataArrived = true;
    }
    void sendHello()
    {
        char data[10] ="HELLO";
        qint64 size = sizeof(data);

        QTcpSocket *serverSocket = server->nextPendingConnection();
        serverSocket->write(data, size);
        serverSocket->flush();
        QEventLoop loop;
        QTimer::singleShot(200, &loop, SLOT(quit())); //allow the TCP/IP stack time to loopback the data, so our socket is ready to read
        loop.exec(QEventLoop::ExcludeSocketNotifiers);
        testResult = dataArrived;
        QTimer::singleShot(200, &loop, SLOT(quit()));
        loop.exec(); //check the deferred event is processed
        serverSocket->close();
        QThread::currentThread()->exit(0);
    }
};

class SocketTestThread : public QThread
{
    Q_OBJECT
public:
    SocketTestThread():QThread(0),testResult(false){};
    void run()
    {
        SocketEventsTester *tester = new SocketEventsTester();
        if (tester->init())
            exec();
        testResult = tester->testResult;
        dataArrived = tester->dataArrived;
        delete tester;
    }
     bool testResult;
     bool dataArrived;
};

void tst_QEventLoop::processEventsExcludeSocket()
{
    SocketTestThread thread;
    thread.start();
    QVERIFY(thread.wait());
    QVERIFY(!thread.testResult);
    QVERIFY(thread.dataArrived);
}

class TimerReceiver : public QObject
{
public:
    int gotTimerEvent;

    TimerReceiver()
        : QObject(), gotTimerEvent(-1)
    { }

    void timerEvent(QTimerEvent *event)
    {
        gotTimerEvent = event->timerId();
    }
};

void tst_QEventLoop::processEventsExcludeTimers()
{
    TimerReceiver timerReceiver;
    int timerId = timerReceiver.startTimer(0);

    QEventLoop eventLoop;

    // normal process events will send timers
    eventLoop.processEvents();
    QCOMPARE(timerReceiver.gotTimerEvent, timerId);
    timerReceiver.gotTimerEvent = -1;
}

Q_DECLARE_METATYPE(QThread*)

namespace DeliverInDefinedOrder_QTBUG19637 {
    enum { NbThread = 3,  NbObject = 500, NbEventQueue = 5, NbEvent = 50 };

    struct CustomEvent : public QEvent {
        CustomEvent(int q, int v) : QEvent(Type(User + q)), value(v) {}
        int value;
    };

    struct Object : public QObject {
        Q_OBJECT
    public:
        Object() : count(0) {
            for (int i = 0; i < NbEventQueue;  i++)
                lastReceived[i] = -1;
        }
        int lastReceived[NbEventQueue];
        int count;
        virtual void customEvent(QEvent* e) {
            QVERIFY(e->type() >= QEvent::User);
            QVERIFY(e->type() < QEvent::User + 5);
            uint idx = e->type() - QEvent::User;
            int value = static_cast<CustomEvent *>(e)->value;
            QVERIFY(lastReceived[idx] < value);
            lastReceived[idx] = value;
            count++;
        }

    public slots:
        void moveToThread(QThread *t) {
            QObject::moveToThread(t);
        }
        void processEvents() {
            // Process all events for this thread
            QCoreApplication::processEvents(QEventLoop::AllEvents, 30000);
        }
    };

}

void tst_QEventLoop::deliverInDefinedOrder_QTBUG19637()
{
    using namespace DeliverInDefinedOrder_QTBUG19637;
    qMetaTypeId<QThread*>();
    QThread threads[NbThread];
    Object objects[NbObject];
    for (int t = 0; t < NbThread; t++) {
        threads[t].start();
    }

    int event = 0;

    for (int o = 0; o < NbObject; o++) {
        objects[o].moveToThread(&threads[o % NbThread]);
        for (int e = 0; e < NbEvent; e++) {
            int q = e % NbEventQueue;
            QCoreApplication::postEvent(&objects[o], new CustomEvent(q, ++event) , q);
            if (e % 7)
                QMetaObject::invokeMethod(&objects[o], "moveToThread", Qt::QueuedConnection, Q_ARG(QThread*, &threads[(e+o)%NbThread]));
        }
    }

    for (int o = 0; o < NbObject; o++) {
        // Wait until all events processed
        QMetaObject::invokeMethod(&objects[o], "processEvents", Qt::BlockingQueuedConnection);
        // Test event count
        QTRY_COMPARE(objects[o].count, int(NbEvent));
    }

    for (int t = 0; t < NbThread; t++) {
        threads[t].quit();
        threads[t].wait();
    }

}

QTEST_MAIN(tst_QEventLoop)

#include "moc_tst_qeventloop.cpp"

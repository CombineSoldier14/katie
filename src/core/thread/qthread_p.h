/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#ifndef QTHREAD_P_H
#define QTHREAD_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//
//

#include "qplatformdefs.h"
#include "QtCore/qthread.h"
#include "QtCore/qmutex.h"
#include "QtCore/qstack.h"
#include "QtCore/qwaitcondition.h"
#include "QtCore/qmap.h"
#include "qobject_p.h"

QT_BEGIN_NAMESPACE

class QAbstractEventDispatcher;
class QEventLoop;

class QPostEvent
{
public:
    QObject *receiver;
    QEvent *event;
    int priority;
    inline QPostEvent()
        : receiver(Q_NULLPTR), event(Q_NULLPTR), priority(0)
    { }
    inline QPostEvent(QObject *r, QEvent *e, int p)
        : receiver(r), event(e), priority(p)
    { }
};
inline bool operator<(int priority, const QPostEvent &pe)
{
    return pe.priority < priority;
}
inline bool operator<(const QPostEvent &pe, int priority)
{
    return priority < pe.priority;
}

// This class holds the list of posted events.
//  The list has to be kept sorted by priority
class QPostEventList : public QList<QPostEvent>
{
public:
    // recursion == recursion count for sendPostedEvents()
    int recursion;

    // sendOffset == the current event to start sending
    int startOffset;
    // insertionOffset == set by sendPostedEvents to tell postEvent() where to start insertions
    int insertionOffset;

    QMutex mutex;

    inline QPostEventList()
        : QList<QPostEvent>(), recursion(0), startOffset(0), insertionOffset(0)
    { }

    void addEvent(const QPostEvent &ev) {
        int priority = ev.priority;
        if (isEmpty() || last().priority >= priority) {
            // optimization: we can simply append if the last event in
            // the queue has higher or equal priority
            append(ev);
        } else {
            // insert event in descending priority order, using upper
            // bound for a given priority (to ensure proper ordering
            // of events with the same priority)
            QPostEventList::iterator at = qUpperBound(begin() + insertionOffset, end(), priority);
            insert(at, ev);
        }
    }
private:
    //hides because they do not keep that list sorted. addEvent must be used
    using QList<QPostEvent>::append;
    using QList<QPostEvent>::insert;
};

#ifndef QT_NO_THREAD
class QThreadPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QThread)

public:
    QThreadPrivate(QThreadData *d = Q_NULLPTR);
    ~QThreadPrivate();

    QMutex mutex;

    bool running;
    bool finished;
    bool terminated;
    bool isInFinish; //when in QThreadPrivate::finish

    bool exited;
    int returnCode;

    uint stackSize;
    QThread::Priority priority;

    pthread_t thread_id;
    QWaitCondition thread_done;

    static void *start(void *arg);
    static void finish(void *);

    QThreadData *data;

    static void createEventDispatcher(QThreadData *data);
};

#else // QT_NO_THREAD

class QThreadPrivate : public QObjectPrivate
{
public:
    QThreadPrivate(QThreadData *d = Q_NULLPTR);
    ~QThreadPrivate();

    QThreadData *data;

    static void setCurrentThread(QThread*) {}
    static void createEventDispatcher(QThreadData *data);

    Q_DECLARE_PUBLIC(QThread)
};

#endif // QT_NO_THREAD

class QThreadData
{
public:
    QThreadData(int initialRefCount = 1);
    ~QThreadData();

    static QThreadData *current();
    static void clearCurrentThreadData();
    static QThreadData *get2(QThread *thread)
    { Q_ASSERT_X(thread != 0, "QThread", "internal error"); return thread->d_func()->data; }


    void ref();
    void deref();

    bool canWaitLocked()
    {
        QMutexLocker locker(&postEventList.mutex);
        return canWait;
    }

    // This class provides per-thread (by way of being a QThreadData
    // member) storage for qFlagLocation()
    class FlaggedDebugSignatures
    {
        static const uint Count = 2;

        uint idx;
        const char* locations[Count];

    public:
        FlaggedDebugSignatures() : idx(0)
        {
            for (uint i = 0; i < Count; ++i)
                locations[i] = Q_NULLPTR;
        }

        void store(const char* method)
        { locations[idx++ % Count] = method; }

        bool contains(const char *method) const
        {
            for (uint i = 0; i < Count; ++i)
                if (locations[i] == method)
                    return true;
            return false;
        }
    };

    bool quitNow;
    bool canWait;
    bool isAdopted;
    int loopLevel;

    Qt::HANDLE threadId;
    QStack<QEventLoop *> eventLoops;
    QThread *thread;
    QAbstractEventDispatcher *eventDispatcher;
    QPostEventList postEventList;
    FlaggedDebugSignatures flaggedSignatures;

private:
    QAtomicInt _ref;
};

class QScopedLoopLevelCounter
{
    QThreadData *threadData;
public:
    inline QScopedLoopLevelCounter(QThreadData *threaddata)
        : threadData(threaddata)
    { ++threadData->loopLevel; }
    inline ~QScopedLoopLevelCounter()
    { --threadData->loopLevel; }
};

// thread wrapper for the main() thread
class QAdoptedThread : public QThread
{
    Q_DECLARE_PRIVATE(QThread)

public:
    QAdoptedThread(QThreadData *data = Q_NULLPTR);
    ~QAdoptedThread();
    void init();

private:
    void run();
};

QT_END_NAMESPACE

#endif // QTHREAD_P_H

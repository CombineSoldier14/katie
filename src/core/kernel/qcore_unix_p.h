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

#ifndef QCORE_UNIX_P_H
#define QCORE_UNIX_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of Qt code on Unix. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qplatformdefs.h"
#include "qatomic.h"

#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#define EINTR_LOOP(var, cmd)                    \
    do {                                        \
        var = cmd;                              \
    } while (var == -1 && errno == EINTR)

QT_BEGIN_NAMESPACE

// Internal operator functions for timevals
inline timeval &normalizedTimeval(timeval &t)
{
    while (t.tv_usec > 1000000l) {
        ++t.tv_sec;
        t.tv_usec -= 1000000l;
    }
    while (t.tv_usec < 0l) {
        --t.tv_sec;
        t.tv_usec += 1000000l;
    }
    return t;
}
inline bool operator<(const timeval &t1, const timeval &t2)
{ return t1.tv_sec < t2.tv_sec || (t1.tv_sec == t2.tv_sec && t1.tv_usec < t2.tv_usec); }
inline bool operator==(const timeval &t1, const timeval &t2)
{ return t1.tv_sec == t2.tv_sec && t1.tv_usec == t2.tv_usec; }
inline timeval &operator+=(timeval &t1, const timeval &t2)
{
    t1.tv_sec += t2.tv_sec;
    t1.tv_usec += t2.tv_usec;
    return normalizedTimeval(t1);
}
inline timeval operator+(const timeval &t1, const timeval &t2)
{
    timeval tmp;
    tmp.tv_sec = t1.tv_sec + t2.tv_sec;
    tmp.tv_usec = t1.tv_usec + t2.tv_usec;
    return normalizedTimeval(tmp);
}
inline timeval operator-(const timeval &t1, const timeval &t2)
{
    timeval tmp;
    tmp.tv_sec = t1.tv_sec - (t2.tv_sec - 1);
    tmp.tv_usec = t1.tv_usec - (t2.tv_usec + 1000000);
    return normalizedTimeval(tmp);
}
inline timeval operator*(const timeval &t1, int mul)
{
    timeval tmp;
    tmp.tv_sec = t1.tv_sec * mul;
    tmp.tv_usec = t1.tv_usec * mul;
    return normalizedTimeval(tmp);
}

static inline void qt_ignore_sigpipe()
{
    // Set to ignore SIGPIPE once only.
    static QAtomicInt atom = QAtomicInt(0);
    if (!atom) {
        // More than one thread could turn off SIGPIPE at the same time
        // But that's acceptable because they all would be doing the same
        // action
        struct sigaction noaction;
        memset(&noaction, 0, sizeof(noaction));
        noaction.sa_handler = SIG_IGN;
        ::sigaction(SIGPIPE, &noaction, Q_NULLPTR);
        atom = 1;
    }
}

// don't call QT_OPEN or ::open
// call qt_safe_open
static inline int qt_safe_open(const char *pathname, int flags, mode_t mode = 0777)
{
#ifdef O_CLOEXEC
    flags |= O_CLOEXEC;
#endif
    int fd;
    EINTR_LOOP(fd, QT_OPEN(pathname, flags, mode));

    // unknown flags are ignored, so we have no way of verifying if
    // O_CLOEXEC was accepted
    if (fd != -1)
        ::fcntl(fd, F_SETFD, FD_CLOEXEC);
    return fd;
}
#undef QT_OPEN
#define QT_OPEN         qt_safe_open

// don't call ::pipe
// call qt_safe_pipe
static inline int qt_safe_pipe(int pipefd[2], int flags = 0)
{
#ifdef O_CLOEXEC
    Q_ASSERT((flags & ~(O_CLOEXEC | O_NONBLOCK)) == 0);
#else
    Q_ASSERT((flags & ~O_NONBLOCK) == 0);
#endif

#if defined(Q_OS_LINUX) && defined(O_CLOEXEC)
    // since Linux 2.6.24 and glibc 2.9
    flags |= O_CLOEXEC;
    return ::pipe2(pipefd, flags);
#else
    int ret = ::pipe(pipefd);
    if (ret == -1)
        return -1;

    ::fcntl(pipefd[0], F_SETFD, FD_CLOEXEC);
    ::fcntl(pipefd[1], F_SETFD, FD_CLOEXEC);

    // set non-block too?
    if (flags & O_NONBLOCK) {
        ::fcntl(pipefd[0], F_SETFL, ::fcntl(pipefd[0], F_GETFL) | O_NONBLOCK);
        ::fcntl(pipefd[1], F_SETFL, ::fcntl(pipefd[1], F_GETFL) | O_NONBLOCK);
    }

    return 0;
#endif
}

// don't call dup or fcntl(F_DUPFD)
static inline int qt_safe_dup(int oldfd)
{
#ifdef F_DUPFD_CLOEXEC
    // since Linux 2.6.24
    return ::fcntl(oldfd, F_DUPFD_CLOEXEC, 0);
#else
    int ret = ::fcntl(oldfd, F_DUPFD, 0);
    if (ret != -1)
        ::fcntl(ret, F_SETFD, FD_CLOEXEC);
    return ret;
#endif
}

// don't call dup2
// call qt_safe_dup2
static inline int qt_safe_dup2(int oldfd, int newfd)
{
    int ret;
    EINTR_LOOP(ret, ::dup2(oldfd, newfd));
    return ret;
}

static inline qint64 qt_safe_read(int fd, void *data, qint64 maxlen)
{
    qint64 ret = 0;
    EINTR_LOOP(ret, QT_READ(fd, data, maxlen));
    return ret;
}
#undef QT_READ
#define QT_READ qt_safe_read

static inline qint64 qt_safe_write(int fd, const void *data, qint64 len)
{
    qint64 ret = 0;
    EINTR_LOOP(ret, QT_WRITE(fd, data, len));
    return ret;
}
#undef QT_WRITE
#define QT_WRITE qt_safe_write

static inline qint64 qt_safe_write_nosignal(int fd, const void *data, qint64 len)
{
    qt_ignore_sigpipe();
    return qt_safe_write(fd, data, len);
}

static inline int qt_safe_close(int fd)
{
    int ret;
    EINTR_LOOP(ret, QT_CLOSE(fd));
    return ret;
}
#undef QT_CLOSE
#define QT_CLOSE qt_safe_close

static inline int qt_safe_execve(const char *filename, char *const argv[],
                                 char *const envp[])
{
    int ret;
    EINTR_LOOP(ret, ::execve(filename, argv, envp));
    return ret;
}

static inline int qt_safe_execv(const char *path, char *const argv[])
{
    int ret;
    EINTR_LOOP(ret, ::execv(path, argv));
    return ret;
}

static inline int qt_safe_execvp(const char *file, char *const argv[])
{
    int ret;
    EINTR_LOOP(ret, ::execvp(file, argv));
    return ret;
}

static inline pid_t qt_safe_waitpid(pid_t pid, int *status, int options)
{
    int ret;
    EINTR_LOOP(ret, ::waitpid(pid, status, options));
    return ret;
}

timeval qt_gettime(); // in qelapsedtimer_unix.cpp

Q_CORE_EXPORT int qt_safe_select(int nfds, fd_set *fdread, fd_set *fdwrite, fd_set *fdexcept,
                                 const struct timeval *tv);

// according to X/OPEN we have to define semun ourselves
// we use prefix as on some systems sem.h will have it
struct semid_ds;
union qt_semun {
    int val;                    /* value for SETVAL */
    struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array;      /* array for GETALL, SETALL */
};


/*
   Returns the difference between msecs and elapsed. If msecs is -1,
   however, -1 is returned.
*/
inline static qint64 qt_timeout_value(qint64 msecs, qint64 elapsed)
{
    if (msecs == -1)
        return -1;

    qint64 timeout = msecs - elapsed;
    return timeout < 0 ? 0 : timeout;
}


QT_END_NAMESPACE

#endif

/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QSCOPEDPOINTER_H
#define QSCOPEDPOINTER_H

#include <QtCore/qglobal.h>

#include <cstdlib>


QT_BEGIN_NAMESPACE

template <typename T>
class QScopedPointerDeleter
{
public:
    static inline void cleanup(T *pointer)
    {
        // Enforce a complete type.
        // If you get a compile error here, read the section on forward declared
        // classes in the QScopedPointer documentation.
        typedef char IsIncompleteType[ sizeof(T) ? 1 : -1 ];
        (void) sizeof(IsIncompleteType);

        delete pointer;
    }
};

struct QScopedPointerPodDeleter
{
    static inline void cleanup(void *pointer) { if (pointer) free(pointer); }
};

template <typename T, typename Cleanup = QScopedPointerDeleter<T> >
class QScopedPointer
{
    typedef T *QScopedPointer:: *RestrictedBool;
public:
    explicit inline QScopedPointer(T *p = 0) : d(p)
    {
    }

    inline ~QScopedPointer()
    {
        T *oldD = this->d;
        Cleanup::cleanup(oldD);
        this->d = nullptr;
    }

    inline T &operator*() const
    {
        Q_ASSERT(d);
        return *d;
    }

    inline T *operator->() const
    {
        Q_ASSERT(d);
        return d;
    }

    inline bool operator!() const
    {
        return !d;
    }

    inline operator RestrictedBool() const
    {
        return isNull() ? nullptr : &QScopedPointer::d;
    }

    inline T *data() const
    {
        return d;
    }

    inline bool isNull() const
    {
        return !d;
    }

    inline void reset(T *other = nullptr)
    {
        if (d == other)
            return;
        T *oldD = d;
        d = other;
        Cleanup::cleanup(oldD);
    }

    inline T *take()
    {
        T *oldD = d;
        d = nullptr;
        return oldD;
    }

    inline void swap(QScopedPointer<T, Cleanup> &other)
    {
        qSwap(d, other.d);
    }

    typedef T *pointer;

protected:
    T *d;

private:
    Q_DISABLE_COPY(QScopedPointer)
};

template <class T, class Cleanup>
inline bool operator==(const QScopedPointer<T, Cleanup> &lhs, const QScopedPointer<T, Cleanup> &rhs)
{
    return lhs.data() == rhs.data();
}

template <class T, class Cleanup>
inline bool operator!=(const QScopedPointer<T, Cleanup> &lhs, const QScopedPointer<T, Cleanup> &rhs)
{
    return lhs.data() != rhs.data();
}

template <class T, class Cleanup>
Q_INLINE_TEMPLATE void qSwap(QScopedPointer<T, Cleanup> &p1, QScopedPointer<T, Cleanup> &p2)
{ p1.swap(p2); }

QT_END_NAMESPACE
namespace std {
    template <class T, class Cleanup>
    Q_INLINE_TEMPLATE void swap(QT_PREPEND_NAMESPACE(QScopedPointer)<T, Cleanup> &p1, QT_PREPEND_NAMESPACE(QScopedPointer)<T, Cleanup> &p2)
    { p1.swap(p2); }
}
QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

#endif // QSCOPEDPOINTER_H

/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of internal files.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

#ifndef QSCOPEDPOINTER_P_H
#define QSCOPEDPOINTER_P_H

#include "QtCore/qscopedpointer.h"

QT_BEGIN_NAMESPACE


/* Internal helper class - exposes the data through data_ptr (legacy from QShared).
   Required for some internal Qt classes, do not use otherwise. */
template <typename T, typename Cleanup = QScopedPointerDeleter<T> >
class QCustomScopedPointer : public QScopedPointer<T, Cleanup>
{
public:
    explicit inline QCustomScopedPointer(T *p = 0)
        : QScopedPointer<T, Cleanup>(p)
    {
    }

    inline T *&data_ptr()
    {
        return this->d;
    }

    inline bool operator==(const QCustomScopedPointer<T, Cleanup> &other) const
    {
        return this->d == other.d;
    }

    inline bool operator!=(const QCustomScopedPointer<T, Cleanup> &other) const
    {
        return this->d != other.d;
    }

private:
    Q_DISABLE_COPY(QCustomScopedPointer)
};

/* Internal helper class - a handler for QShared* classes, to be used in QCustomScopedPointer */
template <typename T>
class QScopedPointerSharedDeleter
{
public:
    static inline void cleanup(T *d)
    {
        if (d && !d->ref.deref())
            delete d;
    }
};

/* Internal.
   This class is basically a scoped pointer pointing to a ref-counted object
 */
template <typename T>
class QScopedSharedPointer : public QCustomScopedPointer<T, QScopedPointerSharedDeleter<T> >
{
public:
    explicit inline QScopedSharedPointer(T *p = 0)
        : QCustomScopedPointer<T, QScopedPointerSharedDeleter<T> >(p)
    {
    }

    inline void detach()
    {
        qAtomicDetach(this->d);
    }

    inline void assign(T *other)
    {
        if (this->d == other)
            return;
        if (other)
            other->ref.ref();
        T *oldD = this->d;
        this->d = other;
        QScopedPointerSharedDeleter<T>::cleanup(oldD);
    }

    inline bool operator==(const QScopedSharedPointer<T> &other) const
    {
        return this->d == other.d;
    }

    inline bool operator!=(const QScopedSharedPointer<T> &other) const
    {
        return this->d != other.d;
    }

private:
    Q_DISABLE_COPY(QScopedSharedPointer)
};


QT_END_NAMESPACE

#endif

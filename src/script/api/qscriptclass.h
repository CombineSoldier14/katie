/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the QtScript module of the Katie Toolkit.
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

#ifndef QSCRIPTCLASS_H
#define QSCRIPTCLASS_H

#include <QtCore/qstring.h>

#include <QtCore/qvariant.h>
#include <QtScript/qscriptvalue.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QScriptString;
class QScriptClassPropertyIterator;

class QScriptClassPrivate;
class Q_SCRIPT_EXPORT QScriptClass
{
public:
    enum QueryFlag {
        HandlesReadAccess = 0x01,
        HandlesWriteAccess = 0x02
    };
    Q_DECLARE_FLAGS(QueryFlags, QueryFlag)

    enum Extension {
        Callable,
        HasInstance
    };

    QScriptClass(QScriptEngine *engine);
    virtual ~QScriptClass();

    QScriptEngine *engine() const;

    virtual QueryFlags queryProperty(const QScriptValue &object,
                                     const QScriptString &name,
                                     QueryFlags flags, uint *id);

    virtual QScriptValue property(const QScriptValue &object,
                                  const QScriptString &name, uint id);

    virtual void setProperty(QScriptValue &object, const QScriptString &name,
                             uint id, const QScriptValue &value);

    virtual QScriptValue::PropertyFlags propertyFlags(
        const QScriptValue &object, const QScriptString &name, uint id);

    virtual QScriptClassPropertyIterator *newIterator(const QScriptValue &object);

    virtual QScriptValue prototype() const;

    virtual QString name() const;

    virtual bool supportsExtension(Extension extension) const;
    virtual QVariant extension(Extension extension,
                               const QVariant &argument = QVariant());

protected:
    QScriptClass(QScriptEngine *engine, QScriptClassPrivate &dd);
    QScriptClassPrivate* d_ptr;

private:
    Q_DECLARE_PRIVATE(QScriptClass)
    Q_DISABLE_COPY(QScriptClass)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QScriptClass::QueryFlags)

QT_END_NAMESPACE

QT_END_HEADER

#endif

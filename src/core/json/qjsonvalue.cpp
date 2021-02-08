/****************************************************************************
**
** Copyright (c) 2012-2015 Barbara Geller
** Copyright (c) 2012-2015 Ansel Sermersheim
** Copyright (c) 2012-2014 Digia Plc and/or its subsidiary(-ies).
** Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qjsonobject.h"
#include "qjsonvalue.h"
#include "qjsonarray.h"
#include "qvariant.h"
#include "qstringlist.h"
#include "qdebug.h"
#include "qjson_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QJsonValue
    \inmodule QtCore
    \ingroup json
    \reentrant
    \since 4.9

    \brief The QJsonValue class encapsulates a value in JSON.

    A value in JSON can be one of 6 basic types:

    JSON is a format to store structured data. It has 6 basic data types:

    \list
    \li bool QJsonValue::Bool
    \li double QJsonValue::Double
    \li string QJsonValue::String
    \li array QJsonValue::Array
    \li object QJsonValue::Object
    \li null QJsonValue::Null
    \endlist

    A value can represent any of the above data types. In addition, QJsonValue has one special
    flag to represent undefined values. This can be queried with isUndefined().

    The type of the value can be queried with type() or accessors like isBool(), isString(), and so on.
    Likewise, the value can be converted to the type stored in it using the toBool(), toString() and so on.

    Values are strictly typed internally and contrary to QVariant will not attempt to do any implicit type
    conversions. This implies that converting to a type that is not stored in the value will return a default
    constructed return value.
*/

/*!
    Creates a QJsonValue of type \a type.

    The default is to create a Null value.
 */
QJsonValue::QJsonValue(Type type)
   : b(false), dbl(0), stringData(Q_NULLPTR), base(Q_NULLPTR), d(Q_NULLPTR), t(type)
{
}

/*!
    \internal
 */
QJsonValue::QJsonValue(QJsonPrivate::Data *data, QJsonPrivate::Base *privbase, const QJsonPrivate::Value &v)
   : b(false), dbl(0), stringData(Q_NULLPTR), base(Q_NULLPTR), d(Q_NULLPTR), t(static_cast<Type>(uint(v.type)))
{
   switch (t) {
      case Undefined:
      case Null:
         break;
      case Bool:
         b = v.toBoolean();
         break;
      case Double:
         dbl = v.toDouble(privbase);
         break;
      case String: {
         stringData = new QString(v.toString(privbase));
         break;
      }
      case Array:
      case Object:
         d = data;
         base = v.base(privbase);
         break;
   }
   if (d) {
      d->ref.ref();
   }
}

/*!
    Creates a value of type Bool, with value \a a.
 */
QJsonValue::QJsonValue(bool a)
   : b(a), dbl(0), stringData(Q_NULLPTR), base(Q_NULLPTR), d(Q_NULLPTR), t(Bool)
{
}

/*!
    Creates a value of type Double, with value \a n.
 */
QJsonValue::QJsonValue(double n)
   : b(false), dbl(n), stringData(Q_NULLPTR), base(Q_NULLPTR), d(Q_NULLPTR), t(Double)
{
}

/*!
    \overload
    Creates a value of type Double, with value \a n.
 */
QJsonValue::QJsonValue(int n)
   : b(false), dbl(n), stringData(Q_NULLPTR), base(Q_NULLPTR), d(Q_NULLPTR), t(Double)
{
}

/*!
    \overload
    Creates a value of type Double, with value \a n.
    NOTE: the integer limits for IEEE 754 double precision data is 2^53 (-9007199254740992 to +9007199254740992).
    If you pass in values outside this range expect a loss of precision to occur.
 */
QJsonValue::QJsonValue(qint64 n)
   : b(false), dbl(n), stringData(Q_NULLPTR), base(Q_NULLPTR), d(Q_NULLPTR), t(Double)
{
}

/*!
    Creates a value of type String, with value \a s.
 */
QJsonValue::QJsonValue(const QString &s)
   : b(false), dbl(0), stringData(new QString(s)), base(Q_NULLPTR), d(Q_NULLPTR), t(String)
{
}

/*!
    Creates a value of type String, with value \a s.
 */
QJsonValue::QJsonValue(QLatin1String s)
   : b(false), dbl(0), stringData(new QString(s)), base(Q_NULLPTR), d(Q_NULLPTR), t(String)
{
}

/*!
    Creates a value of type Array, with value \a a.
 */
QJsonValue::QJsonValue(const QJsonArray &a)
   : b(false), dbl(0), stringData(Q_NULLPTR), base(a.a), d(a.d), t(Array)
{
   if (d) {
      d->ref.ref();
   }
}

/*!
    Creates a value of type Object, with value \a o.
 */
QJsonValue::QJsonValue(const QJsonObject &o)
   : b(false), dbl(0), stringData(Q_NULLPTR), base(o.o), d(o.d), t(Object)
{
   if (d) {
      d->ref.ref();
   }
}


/*!
    Destroys the value.
 */
QJsonValue::~QJsonValue()
{
   if (t == String) {
      delete stringData;
   }

   if (d && !d->ref.deref()) {
      delete d;
   }
}

/*!
    Creates a copy of \a other.
 */
QJsonValue::QJsonValue(const QJsonValue &other)
   : b(other.b), dbl(other.dbl), stringData(Q_NULLPTR), base(Q_NULLPTR), d(other.d), t(other.t)
{
   if (d) {
      d->ref.ref();
   }

   if (t == String && other.stringData) {
      stringData = new QString(*other.stringData);
   }
}

/*!
    Assigns the value stored in \a other to this object.
 */
QJsonValue &QJsonValue::operator =(const QJsonValue &other)
{
   if (t == String) {
      delete stringData;
      stringData = Q_NULLPTR;
   }

   t = other.t;
   b = other.b;
   dbl = other.dbl;

   qAtomicAssign(d, other.d);

   if (t == String && other.stringData) {
      stringData = new QString(*other.stringData);
   }

   return *this;
}

/*!
    \fn bool QJsonValue::isNull() const

    Returns true if the value is null.
*/

/*!
    \fn bool QJsonValue::isBool() const

    Returns true if the value contains a boolean.

    \sa toBool()
 */

/*!
    \fn bool QJsonValue::isDouble() const

    Returns true if the value contains a double.

    \sa toDouble()
 */

/*!
    \fn bool QJsonValue::isString() const

    Returns true if the value contains a string.

    \sa toString()
 */

/*!
    \fn bool QJsonValue::isArray() const

    Returns true if the value contains an array.

    \sa toArray()
 */

/*!
    \fn bool QJsonValue::isObject() const

    Returns true if the value contains an object.

    \sa toObject()
 */

/*!
    \fn bool QJsonValue::isUndefined() const

    Returns true if the value is undefined. This can happen in certain
    error cases as e.g. accessing a non existing key in a QJsonObject.
 */


/*!
    Converts \a variant to a QJsonValue and returns it.

    The conversion will convert QVariant types as follows:

    \list
    \li QVariant::Bool to Bool
    \li QVariant::Int
    \li QVariant::Double
    \li QVariant::LongLong
    \li QVariant::ULongLong
    \li QVariant::UInt to Double
    \li QVariant::String to String
    \li QVariant::StringList
    \li QVariant::VariantList to Array
    \li QVariant::VariantMap to Object
    \endlist

    For all other QVariant types a conversion to a QString will be attempted. If the returned string
    is empty, a Null QJsonValue will be stored, otherwise a String value using the returned QString.

    \sa toVariant()
 */
QJsonValue QJsonValue::fromVariant(const QVariant &variant)
{
    switch (variant.type()) {
        case QVariant::Bool:
            return QJsonValue(variant.toBool());
        case QVariant::Int:
        case QVariant::Double:
        case QVariant::LongLong:
        case QVariant::ULongLong:
        case QVariant::UInt:
            return QJsonValue(variant.toDouble());
        case QVariant::String:
            return QJsonValue(variant.toString());
        case QVariant::StringList:
            return QJsonValue(QJsonArray::fromStringList(variant.toStringList()));
        case QVariant::List:
            return QJsonValue(QJsonArray::fromVariantList(variant.toList()));
        case QVariant::Map:
            return QJsonValue(QJsonObject::fromVariantMap(variant.toMap()));
        default:
            break;
    }

    QByteArray a;
    {
        QDataStream s(&a, QIODevice::WriteOnly);
        s << variant;
    }
    if (a.isEmpty()) {
        return QJsonValue();
    }

    QString result = QLatin1String("@(");
    result += QString::fromLatin1(a.constData(), a.size());
    result += QLatin1String(")@");
    return QJsonValue(result);
}

/*!
    Converts the value to a QVariant.

    The QJsonValue types will be converted as follows:

    \value Null     QVariant()
    \value Bool     QVariant::Bool
    \value Double   QVariant::Double
    \value String   QVariant::String
    \value Array    QVariantList
    \value Object   QVariantMap
    \value Undefined QVariant()

    \sa fromVariant()
 */
QVariant QJsonValue::toVariant() const
{
    switch (t) {
        case Bool:
            return b;
        case Double:
            return dbl;
        case String: {
            QVariant variant = toString();
            QString string = variant.toString();
            if (string.startsWith(QLatin1String("@(")) && string.endsWith(")@")) {
                QByteArray a(string.toLatin1());
                a.remove(0, 2);
                a.chop(2);
                QDataStream stream(&a, QIODevice::ReadOnly);
                QVariant result;
                stream >> result;
                return result;
            }
            return variant;
        }
        case Array:
            return d ?
                    QJsonArray(d, static_cast<QJsonPrivate::Array *>(base)).toVariantList() :
                    QVariantList();
        case Object:
            return d ?
                    QJsonObject(d, static_cast<QJsonPrivate::Object *>(base)).toVariantMap() :
                    QVariantMap();
        case Null:
        case Undefined:
            return QVariant();
    }

   return QVariant();
}

/*!
    \enum QJsonValue::Type

    This enum describes the type of the JSON value.

    \value Null     A Null value
    \value Bool     A boolean value. Use toBool() to convert to a bool.
    \value Double   A double. Use toDouble() to convert to a double.
    \value String   A string. Use toString() to convert to a QString.
    \value Array    An array. Use toArray() to convert to a QJsonArray.
    \value Object   An object. Use toObject() to convert to a QJsonObject.
    \value Undefined The value is undefined. This is usually returned as an
                    error condition, when trying to read an out of bounds value
                    in an array or a non existent key in an object.
*/

/*!
    Returns the type of the value.

    \sa QJsonValue::Type
 */
QJsonValue::Type QJsonValue::type() const
{
   return t;
}

/*!
    Converts the value to a bool and returns it.

    If type() is not bool, the \a defaultValue will be returned.
 */
bool QJsonValue::toBool(bool defaultValue) const
{
   if (t != Bool) {
      return defaultValue;
   }
   return b;
}

/*!
    Converts the value to an int and returns it.

    If type() is not Double or the value is not a whole number,
    the \a defaultValue will be returned.
 */
int QJsonValue::toInt(int defaultValue) const
{
   if (t == Double && int(dbl) == dbl) {
      return dbl;
   }
   return defaultValue;
}

/*!
    Converts the value to a double and returns it.

    If type() is not Double, the \a defaultValue will be returned.
 */
double QJsonValue::toDouble(double defaultValue) const
{
   if (t != Double) {
      return defaultValue;
   }
   return dbl;
}

/*!
    Converts the value to a QString and returns it.

    If type() is not String, the \a defaultValue will be returned.
 */
QString QJsonValue::toString(const QString &defaultValue) const
{
   if (t != String) {
      return defaultValue;
   }
   return *stringData;
}

/*!
    Converts the value to an array and returns it.

    If type() is not Array, the \a defaultValue will be returned.
 */
QJsonArray QJsonValue::toArray(const QJsonArray &defaultValue) const
{
   if (!d || t != Array) {
      return defaultValue;
   }

   return QJsonArray(d, static_cast<QJsonPrivate::Array *>(base));
}

/*!
    \overload

    Converts the value to an array and returns it.

    If type() is not Array, a QJsonArray() will be returned.
 */
QJsonArray QJsonValue::toArray() const
{
   return toArray(QJsonArray());
}

/*!
    Converts the value to an object and returns it.

    If type() is not Object, the \a defaultValue will be returned.
 */
QJsonObject QJsonValue::toObject(const QJsonObject &defaultValue) const
{
   if (!d || t != Object) {
      return defaultValue;
   }

   return QJsonObject(d, static_cast<QJsonPrivate::Object *>(base));
}

/*!
    \overload

    Converts the value to an object and returns it.

    If type() is not Object, the QJsonObject() will be returned.
 */
QJsonObject QJsonValue::toObject() const
{
   return toObject(QJsonObject());
}

/*!
    Returns true if the value is equal to \a other.
 */
bool QJsonValue::operator==(const QJsonValue &other) const
{
   if (t != other.t) {
      return false;
   }

   switch (t) {
      case Undefined:
      case Null:
         break;
      case Bool:
         return b == other.b;
      case Double:
         return dbl == other.dbl;
      case String:
         return toString() == other.toString();
      case Array:
         if (base == other.base) {
            return true;
         }
         if (!base || !other.base) {
            return false;
         }
         return QJsonArray(d, static_cast<QJsonPrivate::Array *>(base))
                == QJsonArray(other.d, static_cast<QJsonPrivate::Array *>(other.base));
      case Object:
         if (base == other.base) {
            return true;
         }
         if (!base || !other.base) {
            return false;
         }
         return QJsonObject(d, static_cast<QJsonPrivate::Object *>(base))
                == QJsonObject(other.d, static_cast<QJsonPrivate::Object *>(other.base));
   }
   return true;
}

/*!
    Returns true if the value is not equal to \a other.
 */
bool QJsonValue::operator!=(const QJsonValue &other) const
{
   return !(*this == other);
}

/*!
    \internal
 */
void QJsonValue::detach()
{
   if (!d) {
      return;
   }

   qAtomicAssign(d, d->clone(base));
   base = static_cast<QJsonPrivate::Object *>(d->header->root());
}


/*!
    \class QJsonValueRef
    \inmodule QtCore
    \reentrant
    \brief The QJsonValueRef class is a helper class for QJsonValue.

    \internal

    \ingroup json

    When you get an object of type QJsonValueRef, if you can assign to it,
    the assignment will apply to the character in the string from
    which you got the reference. That is its whole purpose in life.

    You can use it exactly in the same way as a reference to a QJsonValue.

    The QJsonValueRef becomes invalid once modifications are made to the
    string: if you want to keep the character, copy it into a QJsonValue.

    Most of the QJsonValue member functions also exist in QJsonValueRef.
    However, they are not explicitly documented here.
*/


QJsonValueRef &QJsonValueRef::operator =(const QJsonValue &val)
{
   if (is_object) {
      o->setValueAt(index, val);
   } else {
      a->replace(index, val);
   }

   return *this;
}

QJsonValueRef &QJsonValueRef::operator =(const QJsonValueRef &ref)
{
   if (is_object) {
      o->setValueAt(index, ref);
   } else {
      a->replace(index, ref);
   }

   return *this;
}

QJsonArray QJsonValueRef::toArray() const
{
   return toValue().toArray();
}

QJsonObject QJsonValueRef::toObject() const
{
   return toValue().toObject();
}

QJsonValue QJsonValueRef::toValue() const
{
   if (!is_object) {
      return a->at(index);
   }
   return o->valueAt(index);
}

QDebug operator<<(QDebug dbg, const QJsonValue &o)
{
   switch (o.t) {
      case QJsonValue::Undefined:
         dbg.nospace() << "QJsonValue(undefined)";
         break;
      case QJsonValue::Null:
         dbg.nospace() << "QJsonValue(null)";
         break;
      case QJsonValue::Bool:
         dbg.nospace() << "QJsonValue(bool, " << o.toBool() << ")";
         break;
      case QJsonValue::Double:
         dbg.nospace() << "QJsonValue(double, " << o.toDouble() << ")";
         break;
      case QJsonValue::String:
         dbg.nospace() << "QJsonValue(string, " << o.toString() << ")";
         break;
      case QJsonValue::Array:
         dbg.nospace() << "QJsonValue(array, ";
         dbg.nospace() << o.toArray();
         dbg.nospace() << ")";
         break;
      case QJsonValue::Object:
         dbg.nospace() << "QJsonValue(object, ";
         dbg.nospace() << o.toObject();
         dbg.nospace() << ")";
         break;
   }
   return dbg.space();
}

QT_END_NAMESPACE

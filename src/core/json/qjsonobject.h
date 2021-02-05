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

#ifndef QJSONOBJECT_H
#define QJSONOBJECT_H

#include "qjsonvalue.h"
#include <QtCore/qcontainerfwd.h>
#include <QtCore/qiterator.h>

QT_BEGIN_NAMESPACE


class QDebug;
typedef QMap<QString, QVariant> QVariantMap;
typedef QHash<QString, QVariant> QVariantHash;

class Q_CORE_EXPORT QJsonObject
{
 public:
   QJsonObject();
   ~QJsonObject();

   QJsonObject(const QJsonObject &other);
   QJsonObject &operator =(const QJsonObject &other);

   static QJsonObject fromVariantMap(const QVariantMap &map);
   QVariantMap toVariantMap() const;
   static QJsonObject fromVariantHash(const QVariantHash &map);
   QVariantHash toVariantHash() const;

   QStringList keys() const;
   int size() const;
   inline int count() const {
      return size();
   }
   inline int length() const {
      return size();
   }
   bool isEmpty() const;

   QJsonValue value(const QString &key) const;
   QJsonValue operator[] (const QString &key) const;
   QJsonValueRef operator[] (const QString &key);

   void remove(const QString &key);
   QJsonValue take(const QString &key);
   bool contains(const QString &key) const;

   bool operator==(const QJsonObject &other) const;
   bool operator!=(const QJsonObject &other) const;

   class const_iterator;

   class iterator
   {
      friend class const_iterator;
      friend class QJsonObject;
      QJsonObject *o;
      int i;

    public:
      typedef std::bidirectional_iterator_tag iterator_category;
      typedef int difference_type;
      typedef QJsonValue value_type;
      typedef QJsonValueRefPtr pointer;
      typedef QJsonValueRef reference;

      inline iterator() : o(0), i(0) {}
      inline iterator(QJsonObject *obj, int index) : o(obj), i(index) {}

      inline QString key() const {
         return o->keyAt(i);
      }
      inline QJsonValueRef value() const {
         return QJsonValueRef(o, i);
      }
      inline QJsonValueRef operator*() const {
         return QJsonValueRef(o, i);
      }
      inline QJsonValueRefPtr operator->() const { 
         return QJsonValueRefPtr(o, i);
      }
      inline bool operator==(const iterator &other) const {
         return i == other.i;
      }
      inline bool operator!=(const iterator &other) const {
         return i != other.i;
      }

      inline iterator &operator++() {
         ++i;
         return *this;
      }
      inline iterator operator++(int) {
         iterator r = *this;
         ++i;
         return r;
      }
      inline iterator &operator--() {
         --i;
         return *this;
      }
      inline iterator operator--(int) {
         iterator r = *this;
         --i;
         return r;
      }
      inline iterator operator+(int j) const {
         iterator r = *this;
         r.i += j;
         return r;
      }
      inline iterator operator-(int j) const {
         return operator+(-j);
      }
      inline iterator &operator+=(int j) {
         i += j;
         return *this;
      }
      inline iterator &operator-=(int j) {
         i -= j;
         return *this;
      }

    public:
      inline bool operator==(const const_iterator &other) const {
         return i == other.i;
      }
      inline bool operator!=(const const_iterator &other) const {
         return i != other.i;
      }
   };
   friend class iterator;

   class const_iterator
   {
      friend class iterator;
      const QJsonObject *o;
      int i;

    public:
      typedef std::bidirectional_iterator_tag iterator_category;
      typedef int difference_type;
      typedef QJsonValue* value_type;
      typedef QJsonValuePtr pointer;
      typedef QJsonValue reference;

      inline const_iterator() : o(0), i(0) {}
      inline const_iterator(const QJsonObject *obj, int index)
         : o(obj), i(index) {}
      inline const_iterator(const iterator &other)
         : o(other.o), i(other.i) {}

      inline QString key() const {
         return o->keyAt(i);
      }
      inline QJsonValue value() const {
         return o->valueAt(i);
      }
      inline QJsonValue operator*() const {
         return o->valueAt(i);
      }
      inline QJsonValuePtr operator->() const { 
         return  QJsonValuePtr(o->valueAt(i));
      }
      inline bool operator==(const const_iterator &other) const {
         return i == other.i;
      }
      inline bool operator!=(const const_iterator &other) const {
         return i != other.i;
      }

      inline const_iterator &operator++() {
         ++i;
         return *this;
      }
      inline const_iterator operator++(int) {
         const_iterator r = *this;
         ++i;
         return r;
      }
      inline const_iterator &operator--() {
         --i;
         return *this;
      }
      inline const_iterator operator--(int) {
         const_iterator r = *this;
         --i;
         return r;
      }
      inline const_iterator operator+(int j) const {
         const_iterator r = *this;
         r.i += j;
         return r;
      }
      inline const_iterator operator-(int j) const {
         return operator+(-j);
      }
      inline const_iterator &operator+=(int j) {
         i += j;
         return *this;
      }
      inline const_iterator &operator-=(int j) {
         i -= j;
         return *this;
      }

      inline bool operator==(const iterator &other) const {
         return i == other.i;
      }
      inline bool operator!=(const iterator &other) const {
         return i != other.i;
      }
   };
   friend class const_iterator;

   // STL style
   inline iterator begin() {
      detach();
      return iterator(this, 0);
   }
   inline const_iterator begin() const {
      return const_iterator(this, 0);
   }
   inline const_iterator constBegin() const {
      return const_iterator(this, 0);
   }
   inline iterator end() {
      detach();
      return iterator(this, size());
   }
   inline const_iterator end() const {
      return const_iterator(this, size());
   }
   inline const_iterator constEnd() const {
      return const_iterator(this, size());
   }
   iterator erase(iterator it);

   // more Qt
   typedef iterator Iterator;
   typedef const_iterator ConstIterator;
   iterator find(const QString &key);
   const_iterator find(const QString &key) const {
      return constFind(key);
   }
   const_iterator constFind(const QString &key) const;
   iterator insert(const QString &key, const QJsonValue &value);

   // STL compatibility
   typedef QJsonValue mapped_type;
   typedef QString key_type;
   typedef int size_type;

   inline bool empty() const {
      return isEmpty();
   }

 private:
   friend class QJsonPrivate::Data;
   friend class QJsonValue;
   friend class QJsonDocument;
   friend class QJsonValueRef;

   friend Q_CORE_EXPORT QDebug operator<<(QDebug, const QJsonObject &);

   QJsonObject(QJsonPrivate::Data *data, QJsonPrivate::Object *object);
   void detach(uint reserve = 0);
   void compact();

   QString keyAt(int i) const;
   QJsonValue valueAt(int i) const;
   void setValueAt(int i, const QJsonValue &val);

   QJsonPrivate::Data *d;
   QJsonPrivate::Object *o;
};

Q_CORE_EXPORT QDebug operator<<(QDebug, const QJsonObject &);

QT_END_NAMESPACE

#endif // QJSONOBJECT_H

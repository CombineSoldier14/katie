/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#include "qiconengine.h"
#include "qpainter.h"

QT_BEGIN_NAMESPACE

/*!
  \class QIconEngine

  \brief The QIconEngine class provides an abstract base class for QIcon renderers.

  \ingroup painting

  An icon engine provides the rendering functions for a QIcon. Each icon has a
  corresponding icon engine that is responsible for drawing the icon with a
  requested size, mode and state.

  The icon is rendered by the paint() function, and the icon can additionally be
  obtained as a pixmap with the pixmap() function (the default implementation
  simply uses paint() to achieve this). The addPixmap() function can be used to
  add new pixmaps to the icon engine, and is used by QIcon to add specialized
  custom pixmaps.

  The paint(), pixmap(), and addPixmap() functions are all virtual, and can
  therefore be reimplemented in subclasses of QIconEngine.

  \sa QIconEnginePlugin

*/

/*!
  \fn virtual void QIconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) = 0;

  Uses the given \a painter to paint the icon with the required \a mode and
  \a state into the rectangle \a rect.
*/

/*!  Returns the actual size of the icon the engine provides for the
  requested \a size, \a mode and \a state. The default implementation
  returns the given \a size.
 */
QSize QIconEngine::actualSize(const QSize &size, QIcon::Mode /*mode*/, QIcon::State /*state*/)
{
    return size;
}


/*!
  Destroys the icon engine.
 */
QIconEngine::~QIconEngine()
{
}


/*!
  Returns the icon as a pixmap with the required \a size, \a mode,
  and \a state. The default implementation creates a new pixmap and
  calls paint() to fill it.
*/
QPixmap QIconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)
{
    QPixmap pm(size);
    {
        QPainter p(&pm);
        paint(&p, QRect(QPoint(0,0),size), mode, state);
    }
    return pm;
}

/*!
  Called by QIcon::addPixmap(). Adds a specialized \a pixmap for the given
  \a mode and \a state. The default pixmap-based engine stores any supplied
  pixmaps, and it uses them instead of scaled pixmaps if the size of a pixmap
  matches the size of icon requested. Custom icon engines that implement
  scalable vector formats are free to ignores any extra pixmaps.
 */
void QIconEngine::addPixmap(const QPixmap &/*pixmap*/, QIcon::Mode /*mode*/, QIcon::State /*state*/)
{
}


/*!  Called by QIcon::addFile(). Adds a specialized pixmap from the
  file with the given \a fileName, \a size, \a mode and \a state. The
  default pixmap-based engine stores any supplied file names, and it
  loads the pixmaps on demand instead of using scaled pixmaps if the
  size of a pixmap matches the size of icon requested. Custom icon
  engines that implement scalable vector formats are free to ignores
  any extra files.
 */
void QIconEngine::addFile(const QString &/*fileName*/, const QSize &/*size*/, QIcon::Mode /*mode*/, QIcon::State /*state*/)
{
}

/*!
    Returns a key that identifies this icon engine.
 */
QString QIconEngine::key() const
{
    return QString();
}

/*!
    Returns a clone of this icon engine.
 */
QIconEngine *QIconEngine::clone() const
{
    return 0;
}

/*!
    Reads icon engine contents from the QDataStream \a in. Returns
    true if the contents were read; otherwise returns false.

    QIconEngine's default implementation always return false.
 */
bool QIconEngine::read(QDataStream &)
{
    return false;
}

/*!
    Writes the contents of this engine to the QDataStream \a out.
    Returns true if the contents were written; otherwise returns false.

    QIconEngine's default implementation always return false.
 */
bool QIconEngine::write(QDataStream &) const
{
    return false;
}

/*!
    \since 4.5

    Returns sizes of all images that are contained in the engine for the
    specific \a mode and \a state.

    \note This is a helper method and depends on icon engine support, it
    may not work with all icon engines.
 */
QList<QSize> QIconEngine::availableSizes(QIcon::Mode mode, QIcon::State state) const
{
    return QList<QSize>();
}

/*!
    \since 4.7

    Returns the name used to create the engine, if available.

    \note This is a helper method and depends on icon engine support, it
    may not work with all icon engines.
 */
QString QIconEngine::iconName() const
{
    return QString();
}

QT_END_NAMESPACE





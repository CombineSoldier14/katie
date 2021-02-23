/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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
#ifndef QKEYMAPPER_P_H
#define QKEYMAPPER_P_H

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

#include "qobject.h"
#include "qobject_p.h"
#include "qkeysequence.h"
#include "qlist.h"
#include "qlocale.h"
#include "qevent.h"

#if defined(Q_WS_X11)
#    include "qt_x11_p.h"
#endif

QT_BEGIN_NAMESPACE

class QKeyMapperPrivate;
class QKeyMapper : public QObject
{
    Q_OBJECT
public:
    explicit QKeyMapper();
    ~QKeyMapper();

    static QKeyMapper *instance();
    static void changeKeyboard();
    static QList<int> possibleKeys(QKeyEvent *e);

private:
    friend QKeyMapperPrivate *qt_keymapper_private();
    Q_DECLARE_PRIVATE(QKeyMapper)
    Q_DISABLE_COPY(QKeyMapper)
};

class QKeyEvent;
class QKeyMapperPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QKeyMapper)
public:
    QKeyMapperPrivate();
    ~QKeyMapperPrivate();

    void clearMappings();

    QLocale keyboardInputLocale;
    Qt::LayoutDirection keyboardInputDirection;
    QTextCodec *keyMapperCodec;

#if defined(Q_WS_X11)
    bool translateKeyEvent(QWidget *keywidget, const XEvent *event);
    Qt::KeyboardModifiers translateModifiers(int state);
#endif
};

QKeyMapperPrivate *qt_keymapper_private(); // from qkeymapper.cpp

QT_END_NAMESPACE

#endif // QKEYMAPPER_P_H

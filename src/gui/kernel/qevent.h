/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#ifndef QEVENT_H
#define QEVENT_H

#include <QtCore/qobject.h>
#include <QtCore/qnamespace.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtCore/qmap.h>
#include <QtCore/qset.h>
#include <QtCore/qcoreevent.h>
#include <QtCore/qmimedata.h>
#include <QtGui/qkeysequence.h>
#include <QtGui/qdrag.h>
#include <QtGui/qwindowdefs.h>
#include <QtGui/qregion.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class Q_GUI_EXPORT QInputEvent : public QEvent
{
public:
    QInputEvent(Type type, Qt::KeyboardModifiers modifiers = Qt::NoModifier);
    ~QInputEvent();
    inline Qt::KeyboardModifiers modifiers() const { return modState; }
    inline void setModifiers(Qt::KeyboardModifiers amodifiers) { modState = amodifiers; }
protected:
    Qt::KeyboardModifiers modState;
};

class Q_GUI_EXPORT QMouseEvent : public QInputEvent
{
public:
    QMouseEvent(Type type, const QPoint &pos, Qt::MouseButton button,
                Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
    QMouseEvent(Type type, const QPointF &pos, Qt::MouseButton button,
                Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
    QMouseEvent(Type type, const QPoint &pos, const QPoint &globalPos,
                Qt::MouseButton button, Qt::MouseButtons buttons,
                Qt::KeyboardModifiers modifiers);
    QMouseEvent(Type type, const QPointF &pos, const QPointF &globalPos,
                Qt::MouseButton button, Qt::MouseButtons buttons,
                Qt::KeyboardModifiers modifiers);
    ~QMouseEvent();

    inline const QPoint pos() const { return p.toPoint(); }
    inline const QPointF &posF() const { return p; }
    inline const QPoint globalPos() const { return g.toPoint(); }
    inline const QPointF &globalPoFs() const { return g; }
    inline int x() const { return qRound(p.x()); }
    inline int y() const { return qRound(p.y()); }
    inline int globalX() const { return qRound(g.x()); }
    inline int globalY() const { return qRound(g.y()); }
    inline Qt::MouseButton button() const { return b; }
    inline Qt::MouseButtons buttons() const { return mouseState; }

protected:
    QPointF p, g;
    Qt::MouseButton b;
    Qt::MouseButtons mouseState;
};

class Q_GUI_EXPORT QHoverEvent : public QEvent
{
public:
    QHoverEvent(Type type, const QPoint &pos, const QPoint &oldPos);
    QHoverEvent(Type type, const QPointF &pos, const QPointF &oldPos);
    ~QHoverEvent();

    inline const QPoint pos() const { return p.toPoint(); }
    inline const QPointF &posF() const { return p; }
    inline const QPoint oldPos() const { return op.toPoint(); }
    inline const QPointF &oldPosF() const { return op; }

protected:
    QPointF p, op;
};

#ifndef QT_NO_WHEELEVENT
class Q_GUI_EXPORT QWheelEvent : public QInputEvent
{
public:
    QWheelEvent(const QPoint &pos, int delta,
                Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers,
                Qt::Orientation orient = Qt::Vertical);
    QWheelEvent(const QPointF &pos, int delta,
                Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers,
                Qt::Orientation orient = Qt::Vertical);
    QWheelEvent(const QPoint &pos, const QPoint& globalPos, int delta,
                Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers,
                Qt::Orientation orient = Qt::Vertical);
    QWheelEvent(const QPointF &pos, const QPointF& globalPos, int delta,
                Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers,
                Qt::Orientation orient = Qt::Vertical);
    ~QWheelEvent();

    inline int delta() const { return d; }
    inline const QPoint pos() const { return p.toPoint(); }
    inline const QPointF &posF() const { return p; }
    inline const QPoint globalPos()   const { return g.toPoint(); }
    inline const QPointF &globalPosF()   const { return g; }
    inline int x() const { return qRound(p.x()); }
    inline int y() const { return qRound(p.y()); }
    inline int globalX() const { return qRound(g.x()); }
    inline int globalY() const { return qRound(g.y()); }

    inline Qt::MouseButtons buttons() const { return mouseState; }
    Qt::Orientation orientation() const { return o; }

protected:
    QPointF p, g;
    int d;
    Qt::MouseButtons mouseState;
    Qt::Orientation o;
};
#endif

class Q_GUI_EXPORT QKeyEvent : public QInputEvent
{
public:
    QKeyEvent(Type type, int key, Qt::KeyboardModifiers modifiers, const QString& text = QString(),
              bool autorep = false, int count = 1);
    QKeyEvent(Type type, int key, Qt::KeyboardModifiers modifiers,
              quint32 nativeScanCode, quint32 nativeVirtualKey, quint32 nativeModifiers,
              const QString &text = QString(), bool autorep = false, int count = 1);
    ~QKeyEvent();

    int key() const { return k; }
#ifndef QT_NO_SHORTCUT
    bool matches(QKeySequence::StandardKey key) const;
#endif
    Qt::KeyboardModifiers modifiers() const;
    inline QString text() const { return txt; }
    inline bool isAutoRepeat() const { return autor; }
    inline int count() const { return c; }

    inline quint32 nativeScanCode() const { return nScanCode; }
    inline quint32 nativeVirtualKey() const { return nVirtualKey; }
    inline quint32 nativeModifiers() const { return nModifiers; }


protected:
    QString txt;
    int k;
    int c;
    quint32 nScanCode;
    quint32 nVirtualKey;
    quint32 nModifiers;
    bool autor;
};


class Q_GUI_EXPORT QFocusEvent : public QEvent
{
public:
    QFocusEvent(Type type, Qt::FocusReason reason=Qt::OtherFocusReason);
    ~QFocusEvent();

    inline bool gotFocus() const { return type() == FocusIn; }
    inline bool lostFocus() const { return type() == FocusOut; }

    Qt::FocusReason reason() const;

private:
    Qt::FocusReason m_reason;
};


class Q_GUI_EXPORT QPaintEvent : public QEvent
{
public:
    QPaintEvent(const QRegion& paintRegion);
    QPaintEvent(const QRect &paintRect);
    ~QPaintEvent();

    inline const QRect &rect() const { return m_rect; }
    inline const QRegion &region() const { return m_region; }


protected:
    friend class QApplication;
    friend class QCoreApplication;
    QRect m_rect;
    QRegion m_region;
};

class QUpdateLaterEvent : public QEvent
{
public:
    QUpdateLaterEvent(const QRegion& paintRegion);
    ~QUpdateLaterEvent();

    inline const QRegion &region() const { return m_region; }

protected:
    QRegion m_region;
};

class Q_GUI_EXPORT QMoveEvent : public QEvent
{
public:
    QMoveEvent(const QPoint &pos, const QPoint &oldPos);
    ~QMoveEvent();

    inline const QPoint &pos() const { return p; }
    inline const QPoint &oldPos() const { return oldp;}
protected:
    QPoint p, oldp;
    friend class QApplication;
    friend class QCoreApplication;
};


class Q_GUI_EXPORT QResizeEvent : public QEvent
{
public:
    QResizeEvent(const QSize &size, const QSize &oldSize);
    ~QResizeEvent();

    inline const QSize &size() const { return s; }
    inline const QSize &oldSize()const { return olds;}
protected:
    QSize s, olds;
    friend class QApplication;
    friend class QCoreApplication;
};


class Q_GUI_EXPORT QCloseEvent : public QEvent
{
public:
    QCloseEvent();
    ~QCloseEvent();
};


class Q_GUI_EXPORT QShowEvent : public QEvent
{
public:
    QShowEvent();
    ~QShowEvent();
};


class Q_GUI_EXPORT QHideEvent : public QEvent
{
public:
    QHideEvent();
    ~QHideEvent();
};

#ifndef QT_NO_CONTEXTMENU
class Q_GUI_EXPORT QContextMenuEvent : public QInputEvent
{
public:
    enum Reason { Mouse, Keyboard, Other };

    QContextMenuEvent(QContextMenuEvent::Reason reason, const QPoint &pos,
                      const QPoint &globalPos, Qt::KeyboardModifiers modifiers);
    QContextMenuEvent(QContextMenuEvent::Reason reason, const QPoint &pos, const QPoint &globalPos);
    QContextMenuEvent(QContextMenuEvent::Reason reason, const QPoint &pos);
    ~QContextMenuEvent();

    inline int x() const { return p.x(); }
    inline int y() const { return p.y(); }
    inline int globalX() const { return gp.x(); }
    inline int globalY() const { return gp.y(); }

    inline const QPoint& pos() const { return p; }
    inline const QPoint& globalPos() const { return gp; }

    inline QContextMenuEvent::Reason reason() const { return reas; }

protected:
    QPoint p;
    QPoint gp;
    QContextMenuEvent::Reason reas;
};
#endif // QT_NO_CONTEXTMENU

#ifndef QT_NO_DRAGANDDROP

class QMimeData;

class Q_GUI_EXPORT QDropEvent : public QEvent
{
public:
    QDropEvent(const QPoint& pos, Qt::DropActions actions, const QMimeData *data,
               Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Type type = Drop);
    ~QDropEvent();

    inline const QPoint &pos() const { return p; }
    inline Qt::MouseButtons mouseButtons() const { return mouseState; }
    inline Qt::KeyboardModifiers keyboardModifiers() const { return modState; }

    inline Qt::DropActions possibleActions() const { return act; }
    inline Qt::DropAction proposedAction() const { return default_action; }
    inline void acceptProposedAction() { drop_action = default_action; accept(); }

    inline Qt::DropAction dropAction() const { return drop_action; }
    void setDropAction(Qt::DropAction action);

    QWidget* source() const;
    inline const QMimeData *mimeData() const { return mdata; }

protected:
    friend class QApplication;
    QPoint p;
    Qt::MouseButtons mouseState;
    Qt::KeyboardModifiers modState;
    Qt::DropActions act;
    Qt::DropAction drop_action;
    Qt::DropAction default_action;
    const QMimeData *mdata;
};


class Q_GUI_EXPORT QDragMoveEvent : public QDropEvent
{
public:
    QDragMoveEvent(const QPoint &pos, Qt::DropActions actions, const QMimeData *data,
                   Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Type type = DragMove);
    ~QDragMoveEvent();

    inline QRect answerRect() const { return rect; }

    inline void accept() { QDropEvent::accept(); }
    inline void ignore() { QDropEvent::ignore(); }

    inline void accept(const QRect & r) { accept(); rect = r; }
    inline void ignore(const QRect & r) { ignore(); rect = r; }


protected:
    friend class QApplication;
    QRect rect;
};


class Q_GUI_EXPORT QDragEnterEvent : public QDragMoveEvent
{
public:
    QDragEnterEvent(const QPoint &pos, Qt::DropActions actions, const QMimeData *data,
                    Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
    ~QDragEnterEvent();
};


class Q_GUI_EXPORT QDragLeaveEvent : public QEvent
{
public:
    QDragLeaveEvent();
    ~QDragLeaveEvent();
};
#endif // QT_NO_DRAGANDDROP


class Q_GUI_EXPORT QHelpEvent : public QEvent
{
public:
    QHelpEvent(Type type, const QPoint &pos, const QPoint &globalPos);
    ~QHelpEvent();

    inline int x() const { return p.x(); }
    inline int y() const { return p.y(); }
    inline int globalX() const { return gp.x(); }
    inline int globalY() const { return gp.y(); }

    inline const QPoint& pos()  const { return p; }
    inline const QPoint& globalPos() const { return gp; }

private:
    QPoint p;
    QPoint gp;
};

#ifndef QT_NO_STATUSTIP
class Q_GUI_EXPORT QStatusTipEvent : public QEvent
{
public:
    QStatusTipEvent(const QString &tip);
    ~QStatusTipEvent();

    inline QString tip() const { return s; }
private:
    QString s;
};
#endif

#ifndef QT_NO_WHATSTHIS
class Q_GUI_EXPORT QWhatsThisClickedEvent : public QEvent
{
public:
    QWhatsThisClickedEvent(const QString &href);
    ~QWhatsThisClickedEvent();

    inline QString href() const { return s; }
private:
    QString s;
};
#endif

#ifndef QT_NO_ACTION

class QAction;

class Q_GUI_EXPORT QActionEvent : public QEvent
{
    QAction *act, *bef;
public:
    QActionEvent(QEvent::Type type, QAction *action, QAction *before = 0);
    ~QActionEvent();

    inline QAction *action() const { return act; }
    inline QAction *before() const { return bef; }
};
#endif

#ifndef QT_NO_SHORTCUT
class Q_GUI_EXPORT QShortcutEvent : public QEvent
{
public:
    QShortcutEvent(const QKeySequence &key, int id, bool ambiguous = false);
    ~QShortcutEvent();

    inline const QKeySequence &key() const { return sequence; }
    inline int shortcutId() const { return sid; }
    inline bool isAmbiguous() const { return ambig; }
protected:
    QKeySequence sequence;
    bool ambig;
    int  sid;
};
#endif

class Q_GUI_EXPORT QWindowStateChangeEvent: public QEvent
{
public:
    QWindowStateChangeEvent(Qt::WindowStates aOldState);
    QWindowStateChangeEvent(Qt::WindowStates aOldState, bool isOverride);
    ~QWindowStateChangeEvent();

    inline Qt::WindowStates oldState() const { return ostate; }
    inline bool isOverride() const { return m_override; };

private:
    bool m_override;
    Qt::WindowStates ostate;
};


#ifndef QT_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const QEvent *);
#endif

#ifndef QT_NO_SHORTCUT
inline bool operator==(QKeyEvent *e, QKeySequence::StandardKey key){return (e ? e->matches(key) : false);}
inline bool operator==(QKeySequence::StandardKey key, QKeyEvent *e){return (e ? e->matches(key) : false);}
#endif // QT_NO_SHORTCUT

class QTouchEventTouchPointPrivate;
class Q_GUI_EXPORT QTouchEvent : public QInputEvent
{
public:
    class Q_GUI_EXPORT TouchPoint
    {
    public:
        TouchPoint(int id = -1);
        TouchPoint(const QTouchEvent::TouchPoint &other);
        ~TouchPoint();

        int id() const;

        Qt::TouchPointState state() const;
        bool isPrimary() const;

        QPointF pos() const;
        QPointF startPos() const;
        QPointF lastPos() const;

        QPointF scenePos() const;
        QPointF startScenePos() const;
        QPointF lastScenePos() const;

        QPointF screenPos() const;
        QPointF startScreenPos() const;
        QPointF lastScreenPos() const;

        QPointF normalizedPos() const;
        QPointF startNormalizedPos() const;
        QPointF lastNormalizedPos() const;

        QRectF rect() const;
        QRectF sceneRect() const;
        QRectF screenRect() const;

        qreal pressure() const;

        // internal
        void setId(int id);
        void setState(Qt::TouchPointStates state);
        void setPos(const QPointF &pos);
        void setScenePos(const QPointF &scenePos);
        void setScreenPos(const QPointF &screenPos);
        void setNormalizedPos(const QPointF &normalizedPos);
        void setStartPos(const QPointF &startPos);
        void setStartScenePos(const QPointF &startScenePos);
        void setStartScreenPos(const QPointF &startScreenPos);
        void setStartNormalizedPos(const QPointF &startNormalizedPos);
        void setLastPos(const QPointF &lastPos);
        void setLastScenePos(const QPointF &lastScenePos);
        void setLastScreenPos(const QPointF &lastScreenPos);
        void setLastNormalizedPos(const QPointF &lastNormalizedPos);
        void setRect(const QRectF &rect);
        void setSceneRect(const QRectF &sceneRect);
        void setScreenRect(const QRectF &screenRect);
        void setPressure(qreal pressure);
        QTouchEvent::TouchPoint &operator=(const QTouchEvent::TouchPoint &other);

    private:
        QTouchEventTouchPointPrivate *d;
        friend class QApplication;
        friend class QApplicationPrivate;
    };

    enum DeviceType {
        TouchScreen,
        TouchPad
    };

    QTouchEvent(QEvent::Type eventType,
                QTouchEvent::DeviceType deviceType = TouchScreen,
                Qt::KeyboardModifiers modifiers = Qt::NoModifier,
                Qt::TouchPointStates touchPointStates = 0,
                const QList<QTouchEvent::TouchPoint> &touchPoints = QList<QTouchEvent::TouchPoint>());
    ~QTouchEvent();

    inline QWidget *widget() const { return _widget; }
    inline QTouchEvent::DeviceType deviceType() const { return _deviceType; }
    inline Qt::TouchPointStates touchPointStates() const { return _touchPointStates; }
    inline const QList<QTouchEvent::TouchPoint> &touchPoints() const { return _touchPoints; }

    // internal
    inline void setWidget(QWidget *awidget) { _widget = awidget; }
    inline void setDeviceType(DeviceType adeviceType) { _deviceType = adeviceType; }
    inline void setTouchPointStates(Qt::TouchPointStates aTouchPointStates) { _touchPointStates = aTouchPointStates; }
    inline void setTouchPoints(const QList<QTouchEvent::TouchPoint> &atouchPoints) { _touchPoints = atouchPoints; }

protected:
    QWidget *_widget;
    QTouchEvent::DeviceType _deviceType;
    Qt::TouchPointStates _touchPointStates;
    QList<QTouchEvent::TouchPoint> _touchPoints;

    friend class QApplication;
    friend class QApplicationPrivate;
};

#ifndef QT_NO_GESTURES

class QGesture;
class QGestureEventPrivate;

class Q_GUI_EXPORT QGestureEvent : public QEvent
{
public:
    QGestureEvent(const QList<QGesture *> &gestures);
    ~QGestureEvent();

    QList<QGesture *> gestures() const;
    QGesture *gesture(Qt::GestureType type) const;

    QList<QGesture *> activeGestures() const;
    QList<QGesture *> canceledGestures() const;

#ifdef Q_NO_USING_KEYWORD
    inline void setAccepted(bool accepted) { QEvent::setAccepted(accepted); }
    inline bool isAccepted() const { return QEvent::isAccepted(); }

    inline void accept() { QEvent::accept(); }
    inline void ignore() { QEvent::ignore(); }
#else
    using QEvent::setAccepted;
    using QEvent::isAccepted;
    using QEvent::accept;
    using QEvent::ignore;
#endif

    void setAccepted(QGesture *, bool);
    void accept(QGesture *);
    void ignore(QGesture *);
    bool isAccepted(QGesture *) const;

    void setAccepted(Qt::GestureType, bool);
    void accept(Qt::GestureType);
    void ignore(Qt::GestureType);
    bool isAccepted(Qt::GestureType) const;

    void setWidget(QWidget *widget);
    QWidget *widget() const;

#ifndef QT_NO_GRAPHICSVIEW
    QPointF mapToGraphicsScene(const QPointF &gesturePoint) const;
#endif

private:
    QList<QGesture *> m_gestures;
    QWidget *m_widget;
    QMap<Qt::GestureType, bool> m_accepted;
    QMap<Qt::GestureType, QWidget *> m_targetWidgets;

    friend class QApplication;
    friend class QGestureManager;
};
#endif // QT_NO_GESTURES

QT_END_NAMESPACE

QT_END_HEADER

#endif // QEVENT_H

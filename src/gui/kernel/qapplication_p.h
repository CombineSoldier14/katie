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

#ifndef QAPPLICATION_P_H
#define QAPPLICATION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of qapplication_*.cpp, qwidget*.cpp, qcolor_x11.cpp, qfiledialog.cpp
// and many other.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

#include "qplatformdefs.h"
#include "QtGui/qapplication.h"
#include "QtGui/qevent.h"
#include "QtGui/qfont.h"
#include "QtGui/qcursor.h"
#include "QtGui/qregion.h"
#include "QtCore/qmutex.h"
#include "QtCore/qtranslator.h"
#include "QtCore/qbasictimer.h"
#include "QtCore/qhash.h"
#include "QtCore/qpointer.h"
#include "qcoreapplication_p.h"
#include "QtGui/qshortcutmap_p.h"
#include "QtCore/qpoint.h"
#include "QtCore/qdatetime.h"

QT_BEGIN_NAMESPACE

class QClipboard;
class QGraphicsScene;
class QGraphicsSystem;
class QObject;
class QWidget;
class QSocketNotifier;
#ifndef QT_NO_GESTURES
class QGestureManager;
#endif

extern bool qt_is_gui_used;
#ifndef QT_NO_CLIPBOARD
extern QClipboard *qt_clipboard;
#endif

#ifndef QT_NO_TABLET
struct QTabletDeviceData
{
    int minPressure;
    int maxPressure;
    int minTanPressure;
    int maxTanPressure;
    int minX, maxX, minY, maxY, minZ, maxZ;
    inline QPointF scaleCoord(int coordX, int coordY, int outOriginX, int outExtentX,
                              int outOriginY, int outExtentY) const;

#if defined(Q_WS_X11)
    QPointer<QWidget> widgetToGetPress;

    int deviceType;
    enum {
        TOTAL_XINPUT_EVENTS = 64
    };
    void *device;
    int eventCount;
    long unsigned int eventList[TOTAL_XINPUT_EVENTS]; // XEventClass is in fact a long unsigned int

    int xinput_motion;
    int xinput_key_press;
    int xinput_key_release;
    int xinput_button_press;
    int xinput_button_release;
    int xinput_proximity_in;
    int xinput_proximity_out;
#endif
};

static inline int sign(int x)
{
    return x >= 0 ? 1 : -1;
}

inline QPointF QTabletDeviceData::scaleCoord(int coordX, int coordY,
                                            int outOriginX, int outExtentX,
                                            int outOriginY, int outExtentY) const
{
    QPointF ret;

    if (sign(outExtentX) == sign(maxX))
        ret.setX(((coordX - minX) * qAbs(outExtentX) / qAbs(qreal(maxX - minX))) + outOriginX);
    else
        ret.setX(((qAbs(maxX) - (coordX - minX)) * qAbs(outExtentX) / qAbs(qreal(maxX - minX)))
                 + outOriginX);

    if (sign(outExtentY) == sign(maxY))
        ret.setY(((coordY - minY) * qAbs(outExtentY) / qAbs(qreal(maxY - minY))) + outOriginY);
    else
        ret.setY(((qAbs(maxY) - (coordY - minY)) * qAbs(outExtentY) / qAbs(qreal(maxY - minY)))
                 + outOriginY);

    return ret;
}

typedef QList<QTabletDeviceData> QTabletDeviceDataList;
QTabletDeviceDataList *qt_tablet_devices();
#endif

typedef QHash<QByteArray, QFont> FontHash;
FontHash *qt_app_fonts_hash();

typedef QHash<QByteArray, QPalette> PaletteHash;
PaletteHash *qt_app_palettes_hash();

class Q_GUI_EXPORT QApplicationPrivate : public QCoreApplicationPrivate
{
    Q_DECLARE_PUBLIC(QApplication)
public:
    QApplicationPrivate(int &argc, char **argv, QApplication::Type type);
    ~QApplicationPrivate();

#if defined(Q_WS_X11)
#ifndef QT_NO_SETTINGS
    static bool x11_apply_settings();
#endif
    static void reset_instance_pointer();
#endif
    static bool quitOnLastWindowClosed;
    static void emitLastWindowClosed();
    static bool autoSipEnabled;
    static QString desktopStyleKey();

    void createEventDispatcher();
    QString appName() const;
    static void dispatchEnterLeave(QWidget *enter, QWidget *leave);

    //modality
    static void enterModal(QWidget*);
    static void leaveModal(QWidget*);
    static void enterModal_sys(QWidget*);
    static void leaveModal_sys(QWidget*);
    static bool isBlockedByModal(QWidget *widget);
    static bool modalState();
    static bool tryModalHelper(QWidget *widget, QWidget **rettop = 0);

    bool notify_helper(QObject *receiver, QEvent * e);

    void construct(
#ifdef Q_WS_X11
                   Display *dpy = 0, Qt::HANDLE visual = 0, Qt::HANDLE cmap = 0
#endif
                   );
    void initialize();
    void process_cmdline();

#if defined(Q_WS_X11)
    static void x11_initialize_style();
#endif

    enum KeyPlatform {
        KB_None  = 2,
        KB_X11   = 4,
        KB_KDE   = 8,
        KB_Gnome = 16,
        KB_CDE   = 32,
        KB_All   = 0xffff
    };

    static uint currentPlatform();
    bool inPopupMode() const;
    void closePopup(QWidget *popup);
    void openPopup(QWidget *popup);
    static void setFocusWidget(QWidget *focus, Qt::FocusReason reason);
    static QWidget *focusNextPrevChild_helper(QWidget *toplevel, bool next);

#ifndef QT_NO_SESSIONMANAGER
    QSessionManager *session_manager;
    QString session_id;
    QString session_key;
    bool is_session_restored;
#endif

#ifndef QT_NO_CURSOR
    QList<QCursor> cursor_list;
#endif
#ifndef QT_NO_GRAPHICSVIEW
    // Maintain a list of all scenes to ensure font and palette propagation to
    // all scenes.
    QList<QGraphicsScene *> scene_list;
#endif

    QBasicTimer toolTipWakeUp, toolTipFallAsleep;
    QPoint toolTipPos, toolTipGlobalPos, hoverGlobalPos;
    QPointer<QWidget> toolTipWidget;
#ifndef QT_NO_SHORTCUT
    QShortcutMap shortcutMap;
#endif

    static Qt::MouseButtons mouse_buttons;
    static Qt::KeyboardModifiers modifier_buttons;

    static QSize app_strut;
    static QWidgetList *popupWidgets;
    static QStyle *app_style;
    static int app_cspec;
    static QPalette *app_pal;
    static QPalette *sys_pal;
    static QPalette *set_pal;
    static QGraphicsSystem *graphics_system;
    static QString graphics_system_name;

private:
    static QFont *app_font; // private for a reason! Always use QApplication::font() instead!
public:
    static QFont *sys_font;
    static QFont *set_font;
    static QWidget *main_widget;
    static QWidget *focus_widget;
    static QWidget *hidden_focus_widget;
    static QWidget *active_window;
    static QIcon *app_icon;
    static bool obey_desktop_settings;
    static int  cursor_flash_time;
    static int  mouse_double_click_time;
    static int  keyboard_input_time;
#ifndef QT_NO_WHEELEVENT
    static int  wheel_scroll_lines;
#endif

    static bool animate_ui;
    static bool animate_menu;
    static bool animate_tooltip;
    static bool animate_combo;
    static bool fade_menu;
    static bool fade_tooltip;
    static bool animate_toolbox;
    static bool widgetCount; // Coupled with -widgetcount switch
    static bool load_testability; // Coupled with -testability switch

    static void setSystemPalette(const QPalette &pal);
    static void setPalette_helper(const QPalette &palette, const char* className, bool clearWidgetPaletteHash);
    static void setSystemFont(const QFont &font);

#if defined(Q_WS_X11)
    static void applyX11SpecificCommandLineArguments(QWidget *main_widget);
#endif

    static QApplicationPrivate *instance() { return self; }

    static QString styleOverride;

#ifdef QT_KEYPAD_NAVIGATION
    static QWidget *oldEditFocus;
    static Qt::NavigationMode navigationMode;
#endif

#if defined(Q_WS_X11)
    void _q_alertTimeOut();
    QHash<QWidget *, QTimer *> alertTimerHash;
#endif
#ifndef QT_NO_STYLE_STYLESHEET
    static QString styleSheet;
#endif
    static QPointer<QWidget> leaveAfterRelease;
    static QWidget *pickMouseReceiver(QWidget *candidate, const QPoint &globalPos, QPoint &pos,
                                      QEvent::Type type, Qt::MouseButtons buttons,
                                      QWidget *buttonDown, QWidget *alienWidget);
    static bool sendMouseEvent(QWidget *receiver, QMouseEvent *event, QWidget *alienWidget,
                               QWidget *native, QWidget **buttonDown, QPointer<QWidget> &lastMouseReceiver,
                               bool spontaneous = true);
#if defined(Q_WS_X11)
    void sendSyntheticEnterLeave(QWidget *widget);
#endif

#ifndef QT_NO_GESTURES
    QGestureManager *gestureManager;
    QWidget *gestureWidget;
#endif
#if defined(Q_WS_X11)
    QPixmap *move_cursor;
    QPixmap *copy_cursor;
    QPixmap *link_cursor;
#endif
    QPixmap getPixmapCursor(Qt::CursorShape cshape);

    QMap<int, QWeakPointer<QWidget> > widgetForTouchPointId;
    QMap<int, QTouchEvent::TouchPoint> appCurrentTouchPoints;
    static void updateTouchPointsForWidget(QWidget *widget, QTouchEvent *touchEvent);
    void initializeMultitouch();
    void initializeMultitouch_sys();
    void cleanupMultitouch();
    void cleanupMultitouch_sys();
    int findClosestTouchPointId(const QPointF &screenPos);
    void appendTouchPoint(const QTouchEvent::TouchPoint &touchPoint);
    void removeTouchPoint(int touchPointId);
    static void translateRawTouchEvent(QWidget *widget,
                                       QTouchEvent::DeviceType deviceType,
                                       const QList<QTouchEvent::TouchPoint> &touchPoints);

#ifdef QT_RX71_MULTITOUCH
    bool hasRX71MultiTouch;

    struct RX71TouchPointState {
        QSocketNotifier *socketNotifier;
        QTouchEvent::TouchPoint touchPoint;

        int minX, maxX, scaleX;
        int minY, maxY, scaleY;
        int minZ, maxZ;
    };
    QList<RX71TouchPointState> allRX71TouchPoints;

    bool readRX71MultiTouchEvents(int deviceNumber);
    void fakeMouseEventFromRX71TouchEvent();
    void _q_readRX71MultiTouchEvents();
#endif


private:
    static QApplicationPrivate *self;

    static void giveFocusAccordingToFocusPolicy(QWidget *w,
                                                Qt::FocusPolicy focusPolicy,
                                                Qt::FocusReason focusReason);
    static bool shouldSetFocus(QWidget *w, Qt::FocusPolicy policy);


    static bool isAlien(QWidget *);
};

Q_GUI_EXPORT void qt_translateRawTouchEvent(QWidget *window,
                                            QTouchEvent::DeviceType deviceType,
                                            const QList<QTouchEvent::TouchPoint> &touchPoints);

#if defined(Q_WS_X11)
  extern void qt_x11_enforce_cursor(QWidget *, bool);
  extern void qt_x11_enforce_cursor(QWidget *);
#endif

QT_END_NAMESPACE

#endif // QAPPLICATION_P_H

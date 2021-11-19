/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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


#include <QtTest/QtTest>
#include <qapplication.h>
#include <qlineedit.h>
#include <qmenu.h>
#include <qlabel.h>
#include <qdialog.h>
#include <qevent.h>
#include <qlineedit.h>
#include <QBoxLayout>

#include "../../shared/util.h"

//TESTED_CLASS=
//TESTED_FILES=gui/kernel/qevent.h gui/kernel/qevent.cpp

class FocusLineEdit : public QLineEdit
{
public:
    FocusLineEdit( QWidget* parent = 0, const char* name = 0 ) : QLineEdit(name, parent) {}
    int focusInEventReason;
    int focusOutEventReason;
    bool focusInEventRecieved;
    bool focusInEventGotFocus;
    bool focusOutEventRecieved;
    bool focusOutEventLostFocus;
protected:
    virtual void keyPressEvent( QKeyEvent *e )
    {
        // qDebug( QString("keyPressEvent: %1").arg(e->key()) );
        QLineEdit::keyPressEvent( e );
    }
    void focusInEvent( QFocusEvent* e )
    {
        QLineEdit::focusInEvent( e );
        focusInEventReason = e->reason();
        focusInEventGotFocus = e->gotFocus();
        focusInEventRecieved = true;
    }
    void focusOutEvent( QFocusEvent* e )
    {
        QLineEdit::focusOutEvent( e );
        focusOutEventReason = e->reason();
        focusOutEventLostFocus = !e->gotFocus();
        focusOutEventRecieved = true;
    }
};

class tst_QFocusEvent : public QObject
{
    Q_OBJECT

public:
    tst_QFocusEvent();
    virtual ~tst_QFocusEvent();


    void initWidget();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
private slots:
    void checkReason_Tab();
    void checkReason_ShiftTab();
    void checkReason_BackTab();
    void checkReason_Popup();
    void checkReason_focusWidget();
    void checkReason_Shortcut();
    void checkReason_ActiveWindow();

private:
    QWidget* testFocusWidget;
    FocusLineEdit* childFocusWidgetOne;
    FocusLineEdit* childFocusWidgetTwo;
};

tst_QFocusEvent::tst_QFocusEvent()
{
}

tst_QFocusEvent::~tst_QFocusEvent()
{

}

void tst_QFocusEvent::initTestCase()
{
    testFocusWidget = new QWidget( 0 );
    childFocusWidgetOne = new FocusLineEdit( testFocusWidget );
    childFocusWidgetOne->setGeometry( 10, 10, 180, 20 );
    childFocusWidgetTwo = new FocusLineEdit( testFocusWidget );
    childFocusWidgetTwo->setGeometry( 10, 50, 180, 20 );

    //qApp->setMainWidget( testFocusWidget ); Qt4
    testFocusWidget->resize( 200,100 );
    testFocusWidget->show();
}

void tst_QFocusEvent::cleanupTestCase()
{
    delete testFocusWidget;
}

void tst_QFocusEvent::init()
{
}

void tst_QFocusEvent::cleanup()
{
    childFocusWidgetTwo->setGeometry( 10, 50, 180, 20 );
}

void tst_QFocusEvent::initWidget()
{
    // On X11 we have to ensure the event was processed before doing any checking, on Windows
    // this is processed straight away.
    QApplication::setActiveWindow(childFocusWidgetOne);

    for (int i = 0; i < 1000; ++i) {
        if (childFocusWidgetOne->isActiveWindow() && childFocusWidgetOne->hasFocus())
            break;
        childFocusWidgetOne->activateWindow();
        childFocusWidgetOne->setFocus();
        qApp->processEvents();
        QTest::qWait(100);
    }

    // The first lineedit should have focus
    QVERIFY( childFocusWidgetOne->hasFocus() );

    childFocusWidgetOne->focusInEventRecieved = false;
    childFocusWidgetOne->focusInEventGotFocus = false;
    childFocusWidgetOne->focusInEventReason = -1;
    childFocusWidgetOne->focusOutEventRecieved = false;
    childFocusWidgetOne->focusOutEventLostFocus = false;
    childFocusWidgetOne->focusOutEventReason = -1;
    childFocusWidgetTwo->focusInEventRecieved = false;
    childFocusWidgetTwo->focusInEventGotFocus = false;
    childFocusWidgetTwo->focusInEventReason = -1;
    childFocusWidgetTwo->focusOutEventRecieved = false;
    childFocusWidgetTwo->focusOutEventLostFocus = false;
    childFocusWidgetTwo->focusOutEventReason = -1;
}

void tst_QFocusEvent::checkReason_Tab()
{
    initWidget();

    // Now test the tab key
    QTest::keyClick( childFocusWidgetOne, Qt::Key_Tab );

    QVERIFY(childFocusWidgetOne->focusOutEventRecieved);
    QVERIFY(childFocusWidgetTwo->focusInEventRecieved);
    QVERIFY(childFocusWidgetOne->focusOutEventLostFocus);
    QVERIFY(childFocusWidgetTwo->focusInEventGotFocus);

    QVERIFY( childFocusWidgetTwo->hasFocus() );
    QCOMPARE( childFocusWidgetOne->focusOutEventReason, (int) Qt::TabFocusReason );
    QCOMPARE( childFocusWidgetTwo->focusInEventReason, (int) Qt::TabFocusReason );
}

void tst_QFocusEvent::checkReason_ShiftTab()
{
    initWidget();

    // Now test the shift + tab key
    QTest::keyClick( childFocusWidgetOne, Qt::Key_Tab, Qt::ShiftModifier );

    QVERIFY(childFocusWidgetOne->focusOutEventRecieved);
    QVERIFY(childFocusWidgetTwo->focusInEventRecieved);
    QVERIFY(childFocusWidgetOne->focusOutEventLostFocus);
    QVERIFY(childFocusWidgetTwo->focusInEventGotFocus);

    QVERIFY( childFocusWidgetTwo->hasFocus() );
    QCOMPARE( childFocusWidgetOne->focusOutEventReason, (int)Qt::BacktabFocusReason );
    QCOMPARE( childFocusWidgetTwo->focusInEventReason, (int)Qt::BacktabFocusReason );

}

/*!
    In this test we verify that the Qt::KeyBacktab key is handled in a qfocusevent
*/
void tst_QFocusEvent::checkReason_BackTab()
{
    initWidget();
    QVERIFY( childFocusWidgetOne->hasFocus() );

    // Now test the backtab key
    QTest::keyClick( childFocusWidgetOne, Qt::Key_Backtab );
    QTest::qWait(200);

    QTRY_VERIFY(childFocusWidgetOne->focusOutEventRecieved);
    QVERIFY(childFocusWidgetTwo->focusInEventRecieved);
    QVERIFY(childFocusWidgetOne->focusOutEventLostFocus);
    QVERIFY(childFocusWidgetTwo->focusInEventGotFocus);

    QVERIFY( childFocusWidgetTwo->hasFocus() );
    QCOMPARE( childFocusWidgetOne->focusOutEventReason, int(Qt::BacktabFocusReason) );
    QCOMPARE( childFocusWidgetTwo->focusInEventReason, int(Qt::BacktabFocusReason) );
}

void tst_QFocusEvent::checkReason_Popup()
{
    initWidget();

    QMenu* popupMenu = new QMenu( testFocusWidget );
    popupMenu->addMenu( "Test" );
    popupMenu->popup( QPoint(0,0) );
    QVERIFY(QTest::qWaitForWindowShown(popupMenu));

    QTRY_VERIFY(childFocusWidgetOne->focusOutEventLostFocus);

    QTRY_VERIFY( childFocusWidgetOne->hasFocus() );
    QVERIFY( !childFocusWidgetOne->focusInEventRecieved );
    QVERIFY( childFocusWidgetOne->focusOutEventRecieved );
    QVERIFY( !childFocusWidgetTwo->focusInEventRecieved );
    QVERIFY( !childFocusWidgetTwo->focusOutEventRecieved );
    QCOMPARE( childFocusWidgetOne->focusOutEventReason, int(Qt::PopupFocusReason));

    popupMenu->hide();

    QVERIFY(childFocusWidgetOne->focusInEventRecieved);
    QVERIFY(childFocusWidgetOne->focusInEventGotFocus);

    QVERIFY( childFocusWidgetOne->hasFocus() );
    QVERIFY( childFocusWidgetOne->focusInEventRecieved );
    QVERIFY( childFocusWidgetOne->focusOutEventRecieved );
    QVERIFY( !childFocusWidgetTwo->focusInEventRecieved );
    QVERIFY( !childFocusWidgetTwo->focusOutEventRecieved );
}

void tst_QFocusEvent::checkReason_Shortcut()
{
    initWidget();

    QLabel* label = new QLabel( "&Test", testFocusWidget );
    label->setBuddy( childFocusWidgetTwo );
    label->setGeometry( 10, 50, 90, 20 );
    childFocusWidgetTwo->setGeometry( 105, 50, 95, 20 );
    label->show();

    QVERIFY( childFocusWidgetOne->hasFocus() );
    QVERIFY( !childFocusWidgetTwo->hasFocus() );

    QTest::keyClick( label, Qt::Key_T, Qt::AltModifier );

    QVERIFY(childFocusWidgetOne->focusOutEventRecieved);
    QVERIFY(childFocusWidgetTwo->focusInEventRecieved);
    QVERIFY(childFocusWidgetOne->focusOutEventLostFocus);
    QVERIFY(childFocusWidgetTwo->focusInEventGotFocus);

    QVERIFY( childFocusWidgetTwo->hasFocus() );
    QVERIFY( !childFocusWidgetOne->focusInEventRecieved );
    QVERIFY( childFocusWidgetOne->focusOutEventRecieved );
    QCOMPARE( childFocusWidgetOne->focusOutEventReason, (int)Qt::ShortcutFocusReason );
    QVERIFY( childFocusWidgetTwo->focusInEventRecieved );
    QCOMPARE( childFocusWidgetTwo->focusInEventReason, (int)Qt::ShortcutFocusReason );
    QVERIFY( !childFocusWidgetTwo->focusOutEventRecieved );

    label->hide();
    QVERIFY( childFocusWidgetTwo->hasFocus() );
    QVERIFY( !childFocusWidgetOne->hasFocus() );
}

void tst_QFocusEvent::checkReason_focusWidget()
{
    // This test checks that a widget doesn't loose
    // its focuswidget just because the focuswidget looses focus.
    QWidget window1;
    QWidget frame1;
    QWidget frame2;
    QLineEdit edit1;
    QLineEdit edit2;

    QVBoxLayout outerLayout;
    outerLayout.addWidget(&frame1);
    outerLayout.addWidget(&frame2);
    window1.setLayout(&outerLayout);

    QVBoxLayout leftLayout;
    QVBoxLayout rightLayout;
    leftLayout.addWidget(&edit1);
    rightLayout.addWidget(&edit2);
    frame1.setLayout(&leftLayout);
    frame2.setLayout(&rightLayout);
    window1.show();

    edit1.setFocus();
    QTest::qWait(100);
    edit2.setFocus();

    QVERIFY(frame1.focusWidget() != 0);
    QVERIFY(frame2.focusWidget() != 0);
}

void tst_QFocusEvent::checkReason_ActiveWindow()
{
    initWidget();

    QDialog* d = new QDialog( testFocusWidget );
    d->show();
    d->activateWindow(); // ### CDE
    QApplication::setActiveWindow(d);
    QTest::qWaitForWindowShown(d);

    QTRY_VERIFY(childFocusWidgetOne->focusOutEventRecieved);
    QVERIFY(childFocusWidgetOne->focusOutEventLostFocus);

    QVERIFY( !childFocusWidgetOne->focusInEventRecieved );
    QVERIFY( childFocusWidgetOne->focusOutEventRecieved );
    QCOMPARE( childFocusWidgetOne->focusOutEventReason, (int)Qt::ActiveWindowFocusReason);
    QVERIFY( !childFocusWidgetOne->hasFocus() );

    d->hide();
    QTest::qWait(100);

    QTRY_VERIFY(childFocusWidgetOne->focusInEventRecieved);
    QVERIFY(childFocusWidgetOne->focusInEventGotFocus);

    QVERIFY( childFocusWidgetOne->hasFocus() );
    QVERIFY( childFocusWidgetOne->focusInEventRecieved );
    QCOMPARE( childFocusWidgetOne->focusInEventReason, (int)Qt::ActiveWindowFocusReason);
}

QTEST_MAIN(tst_QFocusEvent)

#include "moc_tst_qfocusevent.cpp"

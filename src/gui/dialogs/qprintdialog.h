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

#ifndef QPRINTDIALOG_H
#define QPRINTDIALOG_H

#include <QtGui/qabstractprintdialog.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


#ifndef QT_NO_PRINTDIALOG

class QPrintDialogPrivate;
class QPrinter;

class Q_GUI_EXPORT QPrintDialog : public QAbstractPrintDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPrintDialog)
    Q_ENUMS(PrintDialogOption)
    Q_PROPERTY(PrintDialogOptions options READ options WRITE setOptions)

public:
    explicit QPrintDialog(QPrinter *printer, QWidget *parent = 0);
    explicit QPrintDialog(QWidget *parent = 0);
    ~QPrintDialog();

    int exec();
#if defined (Q_OS_UNIX) && !defined(Q_WS_MAC)
    virtual void accept();
#endif
    void done(int result);


    void setOption(PrintDialogOption option, bool on = true);
    bool testOption(PrintDialogOption option) const;
    void setOptions(PrintDialogOptions options);
    PrintDialogOptions options() const;

#if defined(Q_OS_UNIX) || defined(Q_WS_MAC) || defined(Q_OS_WIN)
    void setVisible(bool visible);
#endif

#ifdef Q_NO_USING_KEYWORD
    void open() { QDialog::open(); }
#else
    using QDialog::open;
#endif
    void open(QObject *receiver, const char *member);

#ifdef Q_NO_USING_KEYWORD
    void accepted() { QDialog::accepted(); }
#else
    using QDialog::accepted;
#endif

Q_SIGNALS:
    void accepted(QPrinter *printer);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_chbPrintLastFirstToggled(bool))
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
    Q_PRIVATE_SLOT(d_func(), void _q_collapseOrExpandDialog())
#endif
# if defined(Q_OS_UNIX) && !defined(Q_OS_MAC) && !defined(QT_NO_MESSAGEBOX)
    Q_PRIVATE_SLOT(d_func(), void _q_checkFields())
# endif
    friend class QUnixPrintWidget;
};

#endif // QT_NO_PRINTDIALOG

QT_END_NAMESPACE

QT_END_HEADER

#endif // QPRINTDIALOG_H

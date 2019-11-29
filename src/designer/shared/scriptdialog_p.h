/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of Katie Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef SCRIPTDIALOG_H
#define SCRIPTDIALOG_H

#include "shared_global_p.h"

#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE

class QDesignerDialogGuiInterface;

class QTextEdit;

namespace qdesigner_internal {

    // Dialog for showing script errors
    class  QDESIGNER_SHARED_EXPORT ScriptDialog : public QDialog {
        Q_OBJECT

    public:
        explicit ScriptDialog(QDesignerDialogGuiInterface *dialogGui, QWidget *parent);
        bool editScript(QString &script);

    private slots:
        void slotAccept();

    private:
        QString trimmedScript() const;
        bool checkScript();

        QDesignerDialogGuiInterface *m_dialogGui;
        QTextEdit *m_textEdit;
    };
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // SCRIPTDIALOG_H

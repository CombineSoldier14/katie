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

#ifndef QDESIGNER_COMMAND2_H
#define QDESIGNER_COMMAND2_H

#include "shared_global_p.h"
#include "qdesigner_formwindowcommand_p.h"

#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class LayoutCommand;
class BreakLayoutCommand;

/* This command changes the type of a managed layout on a widget (including
 * red layouts of type 'QLayoutWidget') into another type, maintaining the
 * applicable properties. It does this by chaining BreakLayoutCommand and
 * LayoutCommand, parametrizing them not to actually delete/reparent
 * QLayoutWidget's. */

class QDESIGNER_SHARED_EXPORT MorphLayoutCommand : public QDesignerFormWindowCommand {
    Q_DISABLE_COPY(MorphLayoutCommand)
public:
    explicit MorphLayoutCommand(QDesignerFormWindowInterface *formWindow);
    virtual ~MorphLayoutCommand();

    bool init(QWidget *w, int newType);

    static bool canMorph(const QDesignerFormWindowInterface *formWindow, QWidget *w, int *ptrToCurrentType = 0);

    virtual void redo();
    virtual void undo();

private:
    static QString formatDescription(QDesignerFormEditorInterface *core, const QWidget *w, int oldType, int newType);

    BreakLayoutCommand *m_breakLayoutCommand;
    LayoutCommand *m_layoutCommand;
    int m_newType;
    QWidgetList m_widgets;
    QWidget *m_layoutBase;
};

// Change the alignment of a widget in a managed grid/box layout cell.
class LayoutAlignmentCommand : public QDesignerFormWindowCommand {
    Q_DISABLE_COPY(LayoutAlignmentCommand)
public:
    explicit LayoutAlignmentCommand(QDesignerFormWindowInterface *formWindow);

    bool init(QWidget *w, Qt::Alignment alignment);

    virtual void redo();
    virtual void undo();

    // Find out alignment and return whether command is enabled.
    static Qt::Alignment alignmentOf(const QDesignerFormEditorInterface *core, QWidget *w, bool *enabled = 0);

private:
    static void applyAlignment(const QDesignerFormEditorInterface *core, QWidget *w, Qt::Alignment a);

    Qt::Alignment m_newAlignment;
    Qt::Alignment m_oldAlignment;
    QWidget *m_widget;
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // QDESIGNER_COMMAND2_H

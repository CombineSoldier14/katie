/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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

#include "label_taskmenu.h"
#include "inplace_editor.h"

#include <QtDesigner/abstractformwindow.h>

#include <QtGui/QAction>
#include <QtGui/QStyle>
#include <QtGui/QStyleOption>
#include <QtGui/QTextDocument>

static const char *textPropertyC = "text";

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

// -------- LabelTaskMenuInlineEditor
class LabelTaskMenuInlineEditor : public  TaskMenuInlineEditor
{
public:
    LabelTaskMenuInlineEditor(QLabel *button, QObject *parent);

protected:
    virtual QRect editRectangle() const;
};

LabelTaskMenuInlineEditor::LabelTaskMenuInlineEditor(QLabel *w, QObject *parent) :
      TaskMenuInlineEditor(w, ValidationRichText, QLatin1String(textPropertyC), parent)
{
}

QRect LabelTaskMenuInlineEditor::editRectangle() const
{
    QStyleOptionButton opt;
    opt.init(widget());
    return opt.rect;
}

// --------------- LabelTaskMenu

LabelTaskMenu::LabelTaskMenu(QLabel *label, QObject *parent)
    : QDesignerTaskMenu(label, parent),
      m_label(label),
      m_editRichTextAction(new QAction(tr("Change rich text..."), this)),
      m_editPlainTextAction(new QAction(tr("Change plain text..."), this))
{
    LabelTaskMenuInlineEditor *editor = new LabelTaskMenuInlineEditor(label, this);
    connect(m_editPlainTextAction, SIGNAL(triggered()), editor, SLOT(editText()));
    m_taskActions.append(m_editPlainTextAction);

    connect(m_editRichTextAction, SIGNAL(triggered()), this, SLOT(editRichText()));
    m_taskActions.append(m_editRichTextAction);

    QAction *sep = new QAction(this);
    sep->setSeparator(true);
    m_taskActions.append(sep);
}

QAction *LabelTaskMenu::preferredEditAction() const
{
    if (m_label->textFormat () == Qt::PlainText) return m_editPlainTextAction;
    return Qt::mightBeRichText(m_label->text()) ? m_editRichTextAction : m_editPlainTextAction;
}

QList<QAction*> LabelTaskMenu::taskActions() const
{
    return m_taskActions + QDesignerTaskMenu::taskActions();
}

void LabelTaskMenu::editRichText()
{
    changeTextProperty(QLatin1String(textPropertyC), QString(), MultiSelectionMode, m_label->textFormat());
}

}
QT_END_NAMESPACE
#include <moc_label_taskmenu.h>

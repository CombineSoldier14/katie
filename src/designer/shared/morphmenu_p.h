/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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

#ifndef MORPH_COMMAND_H
#define MORPH_COMMAND_H

#include "qdesigner_formwindowcommand_p.h"

QT_BEGIN_NAMESPACE

class QAction;
class QSignalMapper;
class QMenu;

namespace qdesigner_internal {

/* Conveniene morph menu that acts on a single widget. */
class Q_DESIGNER_EXPORT MorphMenu : public QObject {
    Q_DISABLE_COPY(MorphMenu)
    Q_OBJECT
public:
    typedef QList<QAction *> ActionList;

    explicit MorphMenu(QObject *parent = Q_NULLPTR);

    void populate(QWidget *w, QDesignerFormWindowInterface *fw, ActionList& al);
    void populate(QWidget *w, QDesignerFormWindowInterface *fw, QMenu& m);

private slots:
    void slotMorph(const QString &newClassName);

private:
    bool populateMenu(QWidget *w, QDesignerFormWindowInterface *fw);

    QAction *m_subMenuAction;
    QMenu *m_menu;
    QSignalMapper *m_mapper;

    QWidget *m_widget;
    QDesignerFormWindowInterface *m_formWindow;
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // MORPH_COMMAND_H

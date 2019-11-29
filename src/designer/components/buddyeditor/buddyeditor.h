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

#ifndef BUDDYEDITOR_H
#define BUDDYEDITOR_H

#include "buddyeditor_global.h"

#include <connectionedit_p.h>
#include <QtCore/QPointer>
#include <QtCore/QSet>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;

class QLabel;

namespace qdesigner_internal {

class QT_BUDDYEDITOR_EXPORT BuddyEditor : public ConnectionEdit
{
    Q_OBJECT

public:
    BuddyEditor(QDesignerFormWindowInterface *form, QWidget *parent);

    QDesignerFormWindowInterface *formWindow() const;
    virtual void setBackground(QWidget *background);
    virtual void deleteSelected();

public slots:
    virtual void updateBackground();
    virtual void widgetRemoved(QWidget *w);
    void autoBuddy();

protected:
    virtual QWidget *widgetAt(const QPoint &pos) const;
    virtual Connection *createConnection(QWidget *source, QWidget *destination);
    virtual void endConnection(QWidget *target, const QPoint &pos);
    virtual void createContextMenu(QMenu &menu);

private:
    QWidget *findBuddy(QLabel *l, const QWidgetList &existingBuddies) const;

    QPointer<QDesignerFormWindowInterface> m_formWindow;
    bool m_updating;
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif

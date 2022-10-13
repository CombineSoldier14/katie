/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the tools applications of the Katie Toolkit.
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

#ifndef CPPWRITEDECLARATION_H
#define CPPWRITEDECLARATION_H

#include "treewalker.h"

QT_BEGIN_NAMESPACE

class QTextStream;
class Driver;
class Uic;

struct Option;

namespace CPP {

struct WriteDeclaration : public TreeWalker
{
    WriteDeclaration(Uic *uic);

    void acceptUI(DomUI *node);
    void acceptWidget(DomWidget *node);
    void acceptSpacer(DomSpacer *node);
    void acceptLayout(DomLayout *node);
    void acceptActionGroup(DomActionGroup *node);
    void acceptAction(DomAction *node);
    void acceptButtonGroup(const DomButtonGroup *buttonGroup);

private:
    Uic *m_uic;
    Driver *m_driver;
    QTextStream &m_output;
    const Option &m_option;
};

} // namespace CPP

QT_END_NAMESPACE

#endif // CPPWRITEDECLARATION_H

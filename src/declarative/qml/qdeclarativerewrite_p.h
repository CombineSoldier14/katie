/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVEREWRITE_P_H
#define QDECLARATIVEREWRITE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "textwriter_p.h"
#include "qdeclarativejslexer_p.h"
#include "qdeclarativejsparser_p.h"
#include "qdeclarativejsnodepool_p.h"

QT_BEGIN_NAMESPACE

namespace QDeclarativeRewrite {
using namespace QDeclarativeJS;

class SharedBindingTester : protected AST::Visitor
{
    bool _sharable;
public:
    bool isSharable(const QString &code);
    bool isSharable(AST::Node *Node);
    
    virtual bool visit(AST::FunctionDeclaration *) { _sharable = false; return false; }
    virtual bool visit(AST::FunctionExpression *) { _sharable = false; return false; }
    virtual bool visit(AST::CallExpression *) { _sharable = false; return false; }
};

class RewriteBinding: protected AST::Visitor
{
    unsigned _position;
    TextWriter *_writer;
    QByteArray _name;

public:
    QString operator()(const QString &code, bool *ok = 0, bool *sharable = 0);
    QString operator()(QDeclarativeJS::AST::Node *node, const QString &code, bool *sharable = 0);

    //name of the function:  used for the debugger
    void setName(const QByteArray &name) { _name = name; }

protected:
    using AST::Visitor::visit;

    void accept(AST::Node *node);
    QString rewrite(QString code, unsigned position, AST::Statement *node);

    virtual bool visit(AST::Block *ast);
    virtual bool visit(AST::ExpressionStatement *ast);

    virtual bool visit(AST::DoWhileStatement *ast);
    virtual void endVisit(AST::DoWhileStatement *ast);

    virtual bool visit(AST::WhileStatement *ast);
    virtual void endVisit(AST::WhileStatement *ast);

    virtual bool visit(AST::ForStatement *ast);
    virtual void endVisit(AST::ForStatement *ast);

    virtual bool visit(AST::LocalForStatement *ast);
    virtual void endVisit(AST::LocalForStatement *ast);

    virtual bool visit(AST::ForEachStatement *ast);
    virtual void endVisit(AST::ForEachStatement *ast);

    virtual bool visit(AST::LocalForEachStatement *ast);
    virtual void endVisit(AST::LocalForEachStatement *ast);

private:
    int _inLoop;
};

} // namespace QDeclarativeRewrite

QT_END_NAMESPACE

#endif // QDECLARATIVEREWRITE_P_H


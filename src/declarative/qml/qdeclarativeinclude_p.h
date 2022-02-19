/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVEINCLUDE_P_H
#define QDECLARATIVEINCLUDE_P_H

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

#include <QtCore/qobject.h>
#include <QtCore/qurl.h>
#include <QtScript/qscriptvalue.h>

#include "qdeclarativecontext_p.h"
#include "qdeclarativeguard_p.h"

QT_BEGIN_NAMESPACE

class QDeclarativeEngine;
class QScriptContext;
class QScriptEngine;
class QDeclarativeInclude : public QObject
{
    Q_OBJECT
public:
    enum Status {
        Ok = 0,
        Loading = 1,
        NetworkError = 2,
        Exception = 3
    };

    QDeclarativeInclude(const QUrl &, QDeclarativeEngine *, QScriptContext *ctxt);
    ~QDeclarativeInclude();

    void setCallback(const QScriptValue &);
    QScriptValue callback() const;

    QScriptValue result() const;

    static QScriptValue resultValue(QScriptEngine *, Status status = Loading);
    static void callback(QScriptEngine *, QScriptValue &callback, QScriptValue &status);

    static QScriptValue include(QScriptContext *ctxt, QScriptEngine *engine);
    static QScriptValue worker_include(QScriptContext *ctxt, QScriptEngine *engine);

private:
    QDeclarativeEngine *m_engine;
    QScriptEngine *m_scriptEngine;

    QUrl m_url;
    QScriptValue m_callback;
    QScriptValue m_result;
    QDeclarativeGuardedContextData m_context;
    QScriptValue m_scope[2];
};

QT_END_NAMESPACE

#endif // QDECLARATIVEINCLUDE_P_H


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

#ifndef GENERATOR_H
#define GENERATOR_H

#include "moc.h"

QT_BEGIN_NAMESPACE

class Generator
{
    FILE *out;
    ClassDef *cdef;
    QVector<uint> meta_data;
public:
    Generator(ClassDef *classDef, const QList<QByteArray> &metaTypes, FILE *outfile = Q_NULLPTR);
    void generateCode();
private:
    void generateClassInfos();
    void generateFunctions(QList<FunctionDef> &list, const char *functype, int type);
    void generateFunctionRevisions(QList<FunctionDef>& list, const char *functype);
    void generateEnums(int index);
    void generateProperties();
    void generateMetacall();
    void generateStaticMetacall();
    void generateSignal(FunctionDef *def, int index);

    int strreg(const char *); // registers a string and returns its id
    QList<QByteArray> strings;
    QByteArray purestSuperClass;
    QList<QByteArray> metaTypes;
};

QT_END_NAMESPACE

#endif // GENERATOR_H

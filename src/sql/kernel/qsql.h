/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the QtSql module of the Katie Toolkit.
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

#ifndef QSQL_H
#define QSQL_H

#include <QtCore/qglobal.h>


QT_BEGIN_NAMESPACE

namespace QSql
{
    enum Location
    {
        BeforeFirstRow = -1,
        AfterLastRow = -2
    };

    enum ParamTypeFlag
    {
        In = 0x00000001,
        Out = 0x00000002,
        InOut = In | Out,
        Binary = 0x00000004
    };
    Q_DECLARE_FLAGS(ParamType, ParamTypeFlag)

    enum TableType
    {
        Tables = 0x01,
        SystemTables = 0x02,
        Views = 0x04,
        AllTables = 0xff
    };

    enum NumericalPrecisionPolicy
    {
        LowPrecisionInt32    = 0x01,
        LowPrecisionInt64    = 0x02,
        LowPrecisionDouble   = 0x04,

        HighPrecision        = 0
    };

}

Q_DECLARE_OPERATORS_FOR_FLAGS(QSql::ParamType)

QT_END_NAMESPACE


#endif // QSQL_H

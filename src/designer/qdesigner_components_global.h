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

#ifndef QDESIGNER_COMPONENTS_GLOBAL_H
#define QDESIGNER_COMPONENTS_GLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

#define QDESIGNER_COMPONENTS_EXTERN Q_DECL_EXPORT
#define QDESIGNER_COMPONENTS_IMPORT Q_DECL_IMPORT

#ifdef QT_DESIGNER_STATIC
#  define QDESIGNER_COMPONENTS_EXPORT
#elif defined(QDESIGNER_COMPONENTS_LIBRARY)
#  define QDESIGNER_COMPONENTS_EXPORT QDESIGNER_COMPONENTS_EXTERN
#else
#  define QDESIGNER_COMPONENTS_EXPORT QDESIGNER_COMPONENTS_IMPORT
#endif


QT_END_NAMESPACE
QT_END_HEADER

#endif // QDESIGNER_COMPONENTS_GLOBAL_H

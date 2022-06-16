/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QVECTORPATH_P_H
#define QVECTORPATH_P_H

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

#include <qpaintengine.h>
#include "qpaintengine_p.h"
#include "qpainter_p.h"
#include "qguicommon_p.h"

QT_BEGIN_NAMESPACE

class Q_GUI_EXPORT QVectorPath
{
public:
    enum Hint {
        // Shape hints, in 0x000000ff, access using shape()
        AreaShapeMask           = 0x0001,       // shape covers an area
        NonConvexShapeMask      = 0x0002,       // shape is not convex
        CurvedShapeMask         = 0x0004,       // shape contains curves...
        LinesShapeMask          = 0x0008,
        RectangleShapeMask      = 0x0010,
        ShapeMask               = 0x001f,

        // Shape hints merged into basic shapes..
        LinesHint               = LinesShapeMask,
        RectangleHint           = AreaShapeMask | RectangleShapeMask,
        EllipseHint             = AreaShapeMask | CurvedShapeMask,
        ConvexPolygonHint       = AreaShapeMask,
        PolygonHint             = AreaShapeMask | NonConvexShapeMask,
        RoundedRectHint         = AreaShapeMask | CurvedShapeMask,
        ArbitraryShapeHint      = AreaShapeMask | NonConvexShapeMask | CurvedShapeMask,

        // Other hints
        ControlPointRect        = 0x0400, // Set if the control point rect has been calculated...

        // Shape rendering specifiers...
        OddEvenFill             = 0x1000,
        WindingFill             = 0x2000,
        ImplicitClose           = 0x4000
    };

    // ### Falcon: introduca a struct XY for points so lars is not so confused...
    QVectorPath(const qreal *points,
                int count,
                const QPainterPath::ElementType *elements = 0,
                uint hints = ArbitraryShapeHint)
        : m_elements(elements),
          m_points(points),
          m_count(count),
          m_hints(hints)
    {
    }

    QRectF controlPointRect() const;

    inline Hint shape() const { return (Hint) (m_hints & ShapeMask); }
    inline bool isConvex() const { return (m_hints & NonConvexShapeMask) == 0; }
    inline bool isCurved() const { return m_hints & CurvedShapeMask; }

    inline bool hasImplicitClose() const { return m_hints & ImplicitClose; }
    inline bool hasWindingFill() const { return m_hints & WindingFill; }

    inline uint hints() const { return m_hints; }

    inline const QPainterPath::ElementType *elements() const { return m_elements; }
    inline const qreal *points() const { return m_points; }
    inline bool isEmpty() const { return m_points == 0; }

    inline int elementCount() const { return m_count; }
    inline const QPainterPath convertToPainterPath() const;

    static inline uint polygonFlags(QPaintEngine::PolygonDrawMode mode);

private:
    Q_DISABLE_COPY(QVectorPath)

    const QPainterPath::ElementType *m_elements;
    const qreal *m_points;
    const int m_count;

    mutable uint m_hints;
    mutable QRealRect m_cp_rect;
};

inline uint QVectorPath::polygonFlags(QPaintEngine::PolygonDrawMode mode) {
    switch (mode) {
    case QPaintEngine::ConvexMode: return ConvexPolygonHint | ImplicitClose;
    case QPaintEngine::OddEvenMode: return PolygonHint | OddEvenFill | ImplicitClose;
    case QPaintEngine::WindingMode: return PolygonHint | WindingFill | ImplicitClose;
    case QPaintEngine::PolylineMode: return PolygonHint;
    default: return 0;
    }
}

Q_GUI_EXPORT const QVectorPath &qtVectorPathForPath(const QPainterPath &path);

QT_END_NAMESPACE


#endif

/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#ifndef QPRINTPREVIEWWIDGET_H
#define QPRINTPREVIEWWIDGET_H

#include <QtGui/qwidget.h>
#include <QtGui/qprinter.h>

#ifndef QT_NO_PRINTPREVIEWWIDGET

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QPrintPreviewWidgetPrivate;

class Q_GUI_EXPORT QPrintPreviewWidget : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPrintPreviewWidget)
public:

    enum ViewMode {
        SinglePageView,
        FacingPagesView,
        AllPagesView
    };

    enum ZoomMode {
        CustomZoom,
        FitToWidth,
        FitInView
    };

    explicit QPrintPreviewWidget(QPrinter *printer, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    explicit QPrintPreviewWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    ~QPrintPreviewWidget();

    qreal zoomFactor() const;
    QPrinter::Orientation orientation() const;
    ViewMode viewMode() const;
    ZoomMode zoomMode() const;
    int currentPage() const;
    int pageCount() const;
    void setVisible(bool visible);

public Q_SLOTS:
    void print();

    void zoomIn(qreal zoom = 1.1);
    void zoomOut(qreal zoom = 1.1);
    void setZoomFactor(qreal zoomFactor);
    void setOrientation(QPrinter::Orientation orientation);
    void setViewMode(ViewMode viewMode);
    void setZoomMode(ZoomMode zoomMode);
    void setCurrentPage(int pageNumber);

    void fitToWidth();
    void fitInView();
    void setLandscapeOrientation();
    void setPortraitOrientation();
    void setSinglePageViewMode();
    void setFacingPagesViewMode();
    void setAllPagesViewMode();

    void updatePreview();

Q_SIGNALS:
    void paintRequested(QPrinter *printer);
    void previewChanged();

private:
    Q_PRIVATE_SLOT(d_func(), void _q_fit())
    Q_PRIVATE_SLOT(d_func(), void _q_updateCurrentPage())
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QT_NO_PRINTPREVIEWWIDGET
#endif // QPRINTPREVIEWWIDGET_H

/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the Qt Designer of the Katie Toolkit.
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

#ifndef STRINGLISTEDITOR_H
#define STRINGLISTEDITOR_H

#include "ui_stringlisteditor.h"
#include <QtCore/QStringList>

QT_BEGIN_NAMESPACE
class QStringListModel;

namespace qdesigner_internal {

class StringListEditor : public QDialog, private Ui_Dialog
{
    Q_OBJECT
public:
    ~StringListEditor();
    void setStringList(const QStringList &stringList);
    QStringList stringList() const;

    static QStringList getStringList(
        QWidget *parent, const QStringList &init = QStringList(), int *result = 0);

private slots:
    void on_upButton_clicked();
    void on_downButton_clicked();
    void on_newButton_clicked();
    void on_deleteButton_clicked();
    void on_valueEdit_textEdited(const QString &text);
    void currentIndexChanged(const QModelIndex &current, const QModelIndex &previous);
    void currentValueChanged();

private:
    StringListEditor(QWidget *parent = Q_NULLPTR);
    void updateUi();
    int currentIndex() const;
    void setCurrentIndex(int index);
    int count() const;
    QString stringAt(int index) const;
    void setStringAt(int index, const QString &value);
    void removeString(int index);
    void insertString(int index, const QString &value);
    void editString(int index);

    QStringListModel *m_model;
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // STRINGLISTEDITOR_H

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

#include "fontpanel.h"

#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QFontComboBox>
#include <QtCore/QTimer>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

FontPanel::FontPanel(QWidget *parentWidget) :
    QGroupBox(parentWidget),
    m_previewLineEdit(new QLineEdit),
    m_familyComboBox(new QFontComboBox),
    m_styleComboBox(new QComboBox),
    m_pointSizeComboBox(new QComboBox),
    m_previewFontUpdateTimer(0)
{
    setTitle(tr("Font"));

    QFormLayout *formLayout = new QFormLayout(this);

    connect(m_familyComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(slotFamilyChanged(QFont)));
    formLayout->addRow(tr("&Family"), m_familyComboBox);

    m_styleComboBox->setEditable(false);
    connect(m_styleComboBox,  SIGNAL(currentIndexChanged(int)),  this, SLOT(slotStyleChanged(int)));
    formLayout->addRow(tr("&Style"), m_styleComboBox);

    m_pointSizeComboBox->setEditable(false);
    connect(m_pointSizeComboBox, SIGNAL(currentIndexChanged(int)),  this, SLOT(slotPointSizeChanged(int)));
    formLayout->addRow(tr("&Point size"), m_pointSizeComboBox);

    m_previewLineEdit->setReadOnly(true);
    formLayout->addRow(m_previewLineEdit);
}

QFont FontPanel::selectedFont() const
{
    QFont rc = m_familyComboBox->currentFont();
    const QString family = rc.family();
    rc.setPointSize(pointSize());
    const QString styleDescription = styleString();
    if (styleDescription.contains(QLatin1String("Italic")))
        rc.setStyle(QFont::StyleItalic);
    else if (styleDescription.contains(QLatin1String("Oblique")))
        rc.setStyle(QFont::StyleOblique);
    else
        rc.setStyle(QFont::StyleNormal);
    rc.setBold(m_fontDatabase.bold(family, styleDescription));

    // Weight < 0 asserts...
    const int weight = m_fontDatabase.weight(family, styleDescription);
    if (weight >= 0)
        rc.setWeight(weight);
    return rc;
}

void FontPanel::setSelectedFont(const QFont &f)
{
    m_familyComboBox->setCurrentFont(f);

    updateFamily(family());

    const int pointSizeIndex = closestPointSizeIndex(f.pointSize());
    m_pointSizeComboBox->setCurrentIndex( pointSizeIndex);

    const QString styleString = m_fontDatabase.styleString(f);
    const int styleIndex = m_styleComboBox->findText(styleString);
    m_styleComboBox->setCurrentIndex(styleIndex);
    slotUpdatePreviewFont();
}

QString FontPanel::family() const
{
    const int currentIndex = m_familyComboBox->currentIndex();
    return currentIndex != -1 ?  m_familyComboBox->currentFont().family() : QString();
}

int FontPanel::pointSize() const
{
    const int currentIndex = m_pointSizeComboBox->currentIndex();
    return currentIndex != -1 ? m_pointSizeComboBox->itemData(currentIndex).toInt() : 9;
}

QString FontPanel::styleString() const
{
    const int currentIndex = m_styleComboBox->currentIndex();
    return currentIndex != -1 ? m_styleComboBox->itemText(currentIndex) : QString();
}

void FontPanel::slotFamilyChanged(const QFont &)
{
    updateFamily(family());
    delayedPreviewFontUpdate();
}

void FontPanel::slotStyleChanged(int)
{
    updatePointSizes(family(), styleString());
    delayedPreviewFontUpdate();
}

void FontPanel::slotPointSizeChanged(int)
{
    delayedPreviewFontUpdate();
}

void FontPanel::updateFamily(const QString &family)
{
    // Update styles and trigger update of point sizes.
    // Try to maintain selection or select normal
    const QString oldStyleString = styleString();

    const QStringList styles = m_fontDatabase.styles(family);
    const bool hasStyles = !styles.empty();

    m_styleComboBox->setCurrentIndex(-1);
    m_styleComboBox->clear();
    m_styleComboBox->setEnabled(hasStyles);

    int normalIndex = -1;
    const QString normalStyle = QLatin1String("Normal");

    if (hasStyles) {
        foreach (const QString &style, styles) {
            // try to maintain selection or select 'normal' preferably
            const int newIndex = m_styleComboBox->count();
            m_styleComboBox->addItem(style);
            if (oldStyleString == style) {
                m_styleComboBox->setCurrentIndex(newIndex);
            } else {
                if (oldStyleString ==  normalStyle)
                    normalIndex = newIndex;
            }
        }
        if (m_styleComboBox->currentIndex() == -1 && normalIndex != -1)
            m_styleComboBox->setCurrentIndex(normalIndex);
    }
    updatePointSizes(family, styleString());
}

int FontPanel::closestPointSizeIndex(int desiredPointSize) const
{
    //  try to maintain selection or select closest.
    int closestIndex = -1;
    int closestAbsError = 0xFFFF;

    const int pointSizeCount = m_pointSizeComboBox->count();
    for (int i = 0; i < pointSizeCount; i++) {
        const int itemPointSize = m_pointSizeComboBox->itemData(i).toInt();
        const int absError = qAbs(desiredPointSize - itemPointSize);
        if (absError < closestAbsError) {
            closestIndex  = i;
            closestAbsError = absError;
            if (closestAbsError == 0)
                break;
        } else {    // past optimum
            if (absError > closestAbsError) {
                break;
            }
        }
    }
    return closestIndex;
}


void FontPanel::updatePointSizes(const QString &family, const QString &styleString)
{
    const int oldPointSize = pointSize();

    QList<int> pointSizes =  m_fontDatabase.pointSizes(family, styleString);
    if (pointSizes.empty())
        pointSizes = QFontDatabase::standardSizes();

    const bool hasSizes = !pointSizes.empty();
    m_pointSizeComboBox->clear();
    m_pointSizeComboBox->setEnabled(hasSizes);
    m_pointSizeComboBox->setCurrentIndex(-1);

    //  try to maintain selection or select closest.
    if (hasSizes) {
        foreach (int pointSize, pointSizes) {
            m_pointSizeComboBox->addItem(QString::number(pointSize), QVariant(pointSize));
        }
        const int closestIndex = closestPointSizeIndex(oldPointSize);
        if (closestIndex != -1)
            m_pointSizeComboBox->setCurrentIndex(closestIndex);
    }
}

void FontPanel::slotUpdatePreviewFont()
{
    m_previewLineEdit->setFont(selectedFont());
}

void FontPanel::delayedPreviewFontUpdate()
{
    if (!m_previewFontUpdateTimer) {
        m_previewFontUpdateTimer = new QTimer(this);
        connect(m_previewFontUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdatePreviewFont()));
        m_previewFontUpdateTimer->setInterval(0);
        m_previewFontUpdateTimer->setSingleShot(true);
    }
    if (m_previewFontUpdateTimer->isActive())
        return;
    m_previewFontUpdateTimer->start();
}

QT_END_NAMESPACE
#include "moc_fontpanel.h"

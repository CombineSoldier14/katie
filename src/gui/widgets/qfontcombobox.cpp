/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#include "qfontcombobox.h"

#ifndef QT_NO_FONTCOMBOBOX

#include "qstringlistmodel.h"
#include "qitemdelegate.h"
#include "qlistview.h"
#include "qpainter.h"
#include "qevent.h"
#include "qapplication.h"
#include "qcombobox_p.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

class QFontFamilyDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit QFontFamilyDelegate(QObject *parent);

    // painting
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    QIcon truetype;
    QIcon bitmap;
};

QFontFamilyDelegate::QFontFamilyDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{
    truetype = QIcon(QLatin1String(":/trolltech/styles/commonstyle/images/fonttruetype-16.png"));
    bitmap = QIcon(QLatin1String(":/trolltech/styles/commonstyle/images/fontbitmap-16.png"));
}

void QFontFamilyDelegate::paint(QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    QString text = index.data(Qt::DisplayRole).toString();
    QFont font(option.font);
    font.setPointSize(QFontInfo(font).pointSize() * 3 / 2);
    font.setFamily(text);

    QRect r = option.rect;

    if (option.state & QStyle::State_Selected) {
        painter->save();
        painter->setBrush(option.palette.highlight());
        painter->setPen(Qt::NoPen);
        painter->drawRect(option.rect);
        painter->setPen(QPen(option.palette.highlightedText(), 0));
    }

    const QIcon *icon = &bitmap;
    if (QFontDatabase().isSmoothlyScalable(text)) {
        icon = &truetype;
    }
    QSize actualSize = icon->actualSize(r.size());

    icon->paint(painter, r, Qt::AlignLeft|Qt::AlignVCenter);
    if (option.direction == Qt::RightToLeft)
        r.setRight(r.right() - actualSize.width() - 4);
    else
        r.setLeft(r.left() + actualSize.width() + 4);

    QFont old = painter->font();
    painter->setFont(font);

    // If the ascent of the font is larger than the height of the rect,
    // we will clip the text, so it's better to align the tight bounding rect in this case
    // This is specifically for fonts where the ascent is very large compared to
    // the descent, like certain of the Stix family.
    QFontMetricsF fontMetrics(font);
    if (fontMetrics.ascent() > r.height()) {
        QRectF tbr = fontMetrics.tightBoundingRect(text);
        painter->drawText(r.x(), r.y() + (r.height() + tbr.height()) / 2.0, text);
    } else {
        painter->drawText(r, Qt::AlignVCenter|Qt::AlignLeft|Qt::TextSingleLine, text);
    }

    painter->setFont(old);

    if (option.state & QStyle::State_Selected)
        painter->restore();

}

QSize QFontFamilyDelegate::sizeHint(const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    QString text = index.data(Qt::DisplayRole).toString();
    QFont font(option.font);
//     font.setFamily(text);
    font.setPointSize(QFontInfo(font).pointSize() * 3/2);
    QFontMetrics fontMetrics(font);
    return QSize(fontMetrics.width(text), fontMetrics.height());
}


class QFontComboBoxPrivate : public QComboBoxPrivate
{
public:
    inline QFontComboBoxPrivate() { filters = QFontComboBox::AllFonts; }

    QFontComboBox::FontFilters filters;
    QFont currentFont;

    void _q_updateModel();
    void _q_currentChanged(const QString &);

    Q_DECLARE_PUBLIC(QFontComboBox)
};


void QFontComboBoxPrivate::_q_updateModel()
{
    Q_Q(QFontComboBox);
    const int scalableMask = (QFontComboBox::ScalableFonts | QFontComboBox::NonScalableFonts);
    const int spacingMask = (QFontComboBox::ProportionalFonts | QFontComboBox::MonospacedFonts);

    QStringListModel *m = qobject_cast<QStringListModel *>(q->model());
    if (!m)
        return;

    QFontDatabase fdb;
    QStringList list = fdb.families();
    QStringList result;

    int offset = 0;
    QFontInfo fi(currentFont);

    for (int i = 0; i < list.size(); ++i) {
        if ((filters & scalableMask) && (filters & scalableMask) != scalableMask) {
            if (bool(filters & QFontComboBox::ScalableFonts) != fdb.isSmoothlyScalable(list.at(i)))
                continue;
        }
        if ((filters & spacingMask) && (filters & spacingMask) != spacingMask) {
            if (bool(filters & QFontComboBox::MonospacedFonts) != fdb.isFixedPitch(list.at(i)))
                continue;
        }
        result += list.at(i);
        if (list.at(i) == fi.family() || list.at(i).startsWith(fi.family() + QLatin1String(" [")))
            offset = result.count() - 1;
    }
    list = result;

    //we need to block the signals so that the model doesn't emit reset
    //this prevents the current index from changing
    //it will be updated just after this
    ///TODO: we should finda way to avoid blocking signals and have a real update of the model
    const bool old = m->blockSignals(true);
    m->setStringList(list);
    m->blockSignals(old);

    if (list.isEmpty()) {
        if (currentFont != QFont()) {
            currentFont = QFont();
            emit q->currentFontChanged(currentFont);
        }
    } else {
        q->setCurrentIndex(offset);
    }
}


void QFontComboBoxPrivate::_q_currentChanged(const QString &text)
{
    Q_Q(QFontComboBox);
    if (currentFont.family() != text) {
        currentFont.setFamily(text);
        emit q->currentFontChanged(currentFont);
    }
}

/*!
    \class QFontComboBox
    \brief The QFontComboBox widget is a combobox that lets the user
    select a font family.

    \since 4.2
    \ingroup basicwidgets

    The combobox is populated with an alphabetized list of font
    family names, such as Arial, Helvetica, and Times New Roman.
    Family names are displayed using the actual font when possible.
    For fonts such as Symbol, where the name is not representable in
    the font itself, a sample of the font is displayed next to the
    family name.

    QFontComboBox is often used in toolbars, in conjunction with a
    QComboBox for controlling the font size and two \l{QToolButton}s
    for bold and italic.

    When the user selects a new font, the currentFontChanged() signal
    is emitted in addition to currentIndexChanged().

    Call setFontFilters() to filter out certain types of fonts as e.g.
    non scalable fonts or monospaced fonts.

    \image windowsxp-fontcombobox.png Screenshot of QFontComboBox on Windows XP

    \sa QComboBox, QFont, QFontInfo, QFontMetrics, QFontDatabase, {Character Map Example}
*/

/*!
    \fn void QFontComboBox::setCurrentFont(const QFont &font);
*/

/*!
    Constructs a font combobox with the given \a parent.
*/
QFontComboBox::QFontComboBox(QWidget *parent)
    : QComboBox(*new QFontComboBoxPrivate, parent)
{
    Q_D(QFontComboBox);
    d->currentFont = font();
    setEditable(true);

    QStringListModel *m = new QStringListModel(this);
    setModel(m);
    setItemDelegate(new QFontFamilyDelegate(this));
    QListView *lview = qobject_cast<QListView*>(view());
    if (lview)
        lview->setUniformItemSizes(true);

    connect(this, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(_q_currentChanged(QString)));

    connect(qApp, SIGNAL(fontDatabaseChanged()),
            this, SLOT(_q_updateModel()));
}


/*!
    Destroys the combobox.
*/
QFontComboBox::~QFontComboBox()
{
}

/*!
  \enum QFontComboBox::FontFilter

  This enum can be used to only show certain types of fonts in the font combo box.

  \value AllFonts Show all fonts
  \value ScalableFonts Show scalable fonts
  \value NonScalableFonts Show non scalable fonts
  \value MonospacedFonts Show monospaced fonts
  \value ProportionalFonts Show proportional fonts
*/

/*!
    \property QFontComboBox::fontFilters
    \brief the filter for the combobox

    By default, all fonts are listed.
*/
void QFontComboBox::setFontFilters(FontFilters filters)
{
    Q_D(QFontComboBox);
    d->filters = filters;
    d->_q_updateModel();
}

QFontComboBox::FontFilters QFontComboBox::fontFilters() const
{
    Q_D(const QFontComboBox);
    return d->filters;
}

/*!
    \property QFontComboBox::currentFont
    \brief the currently selected font

    \sa currentFontChanged(), currentIndex, currentText
*/
QFont QFontComboBox::currentFont() const
{
    Q_D(const QFontComboBox);
    return d->currentFont;
}

void QFontComboBox::setCurrentFont(const QFont &font)
{
    Q_D(QFontComboBox);
    if (font != d->currentFont) {
        d->currentFont = font;
        d->_q_updateModel();
        if (d->currentFont == font) { //else the signal has already be emitted by _q_updateModel
            emit currentFontChanged(d->currentFont);
        }
    }
}

/*!
    \fn QFontComboBox::currentFontChanged(const QFont &font)

    This signal is emitted whenever the current font changes, with
    the new \a font.

    \sa currentFont
*/

/*!
    \reimp
*/
bool QFontComboBox::event(QEvent *e)
{
    if (e->type() == QEvent::Resize) {
        QListView *lview = qobject_cast<QListView*>(view());
        if (lview)
            lview->window()->setFixedWidth(width() * 5 / 3);
    }
    return QComboBox::event(e);
}

/*!
    \reimp
*/
QSize QFontComboBox::sizeHint() const
{
    QSize sz = QComboBox::sizeHint();
    QFontMetrics fm(font());
    sz.setWidth(fm.width(QLatin1Char('m'))*14);
    return sz;
}

QT_END_NAMESPACE

#include "moc_qfontcombobox.h"
#include "moc_qfontcombobox.cpp"

#endif // QT_NO_FONTCOMBOBOX

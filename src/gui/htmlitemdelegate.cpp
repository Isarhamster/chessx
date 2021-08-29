#include <QPainter>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

#include "htmlitemdelegate.h"

HTMLItemDelegate::HTMLItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{}

void HTMLItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QStyleOptionViewItem options = option; // silences deprecated QstyleOptionViewItem warning
    initStyleOption(&options, index);

    painter->save();

    QTextDocument doc;
    doc.setHtml(options.text);

    QFont f = doc.defaultFont();
    f.setPointSizeF(option.font.pointSizeF());
    doc.setDefaultFont(f);

    options.text = "";
    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

    QSize iconSize = options.icon.actualSize(options.rect.size());
    // right shit the icon
    painter->translate(options.rect.left() + iconSize.width(), options.rect.top());
    QRect clip(0, 0, options.rect.width() + iconSize.width(), options.rect.height());

    painter->setClipRect(clip);
    QAbstractTextDocumentLayout::PaintContext ctx;

    // Adjust color palette if the cell is selected
    if (option.state & QStyle::State_Selected)
        ctx.palette.setColor(QPalette::Text, option.palette.color(QPalette::Active, QPalette::HighlightedText));
    ctx.clip = clip;

    // Vertical Center alignment instead of the default top alignment
    painter->translate(0, 0.5*(options.rect.height() - doc.size().height()));

    doc.documentLayout()->draw(painter, ctx);
    painter->restore();
}

QSize HTMLItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

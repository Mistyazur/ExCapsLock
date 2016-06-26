#include "cmditemdelegate.h"
#include <QApplication>
#include <QPainter>
#include <QTextDocument>
#include <QStyle>
#include <QAbstractTextDocumentLayout>
#include <QDebug>

CmdItemDelegate::CmdItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

void CmdItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    QStyle *style = opt.widget? opt.widget->style() : QApplication::style();

    // Painting item without text
    opt.text = QString();
    if (opt.state & QStyle::State_Selected)
        opt.backgroundBrush = QBrush(QColor("#073655"));
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

    // Set text color
    QAbstractTextDocumentLayout::PaintContext ctx;
    ctx.palette.setColor(QPalette::Text, opt.palette.color(QPalette::Inactive, QPalette::WindowText));

    // Painting text
    QTextDocument doc;
    doc.setHtml(opt.text);
    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt);
    painter->save();
    painter->translate(textRect.topLeft());
    painter->setClipRect(textRect.translated(-textRect.topLeft()));
    doc.documentLayout()->draw(painter, ctx);
    painter->restore();
}

QSize CmdItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);

    QTextDocument doc;
    doc.setHtml(options.text);
    doc.setTextWidth(options.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
}

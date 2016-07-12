#include "cmditemdelegate.h"
#include "CmdItem/cmditem.h"
#include "cmditemsortfilterproxymodel.h"
#include <QApplication>
#include <QStyle>
#include <QPainter>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

#define CSS_TEXTDOC	"h6 { color: #bdbdbd; font: 13px; font-family: 'monospace'; margin: 0px 0px 4px 4px; }"		\
                    "p { color: #6f6f6f; font: 13px; font-family: 'monospace'; margin: 4px 0px 0px 4px; }"		\
                    "hl { color: #0097fb; }"																	\
                    "table { color: #bdbdbd; font: 12px; font-family: 'monospace'; }"


QTextDocument doc;

CmdItemDelegate::CmdItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    doc.setDefaultStyleSheet(CSS_TEXTDOC);
}

void CmdItemDelegate::searchChanged(QString searchText)
{
    m_searchText = searchText;
}

void CmdItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRegExp rx("", Qt::CaseInsensitive, QRegExp::RegExp);
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    QStyle *style = opt.widget? opt.widget->style() : QApplication::style();

    // Set html
    CmdItemSortFilterProxyModel *proxyModel = (CmdItemSortFilterProxyModel *)index.model();
    QStandardItemModel *stdModel = (QStandardItemModel *)proxyModel->sourceModel();
    CmdItem *item = (CmdItem *)stdModel->itemFromIndex(proxyModel->mapToSource(index));
    doc.setHtml(item->html(m_searchText));

    // Painting item without text
    opt.text.clear();
    if (opt.state & QStyle::State_Selected)
        opt.backgroundBrush = QBrush(QColor("#063656"));
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

    // Set text color
    QAbstractTextDocumentLayout::PaintContext ctx;
    ctx.palette.setColor(QPalette::Text, opt.palette.color(QPalette::Normal, QPalette::WindowText));

    // Painting text
    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt);
    painter->save();
    painter->translate(textRect.topLeft());
    painter->setClipRect(textRect.translated(-textRect.topLeft()));
    doc.documentLayout()->draw(painter, ctx);
    painter->restore();
}

QSize CmdItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    // Set html
    CmdItemSortFilterProxyModel *proxyModel = (CmdItemSortFilterProxyModel *)index.model();
    QStandardItemModel *stdModel = (QStandardItemModel *)proxyModel->sourceModel();
    CmdItem *item = (CmdItem *)stdModel->itemFromIndex(proxyModel->mapToSource(index));
    doc.setHtml(item->html(m_searchText));
    doc.setTextWidth(opt.rect.width());

    return QSize(doc.idealWidth(), doc.size().height());
}

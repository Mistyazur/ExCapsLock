#include "cmditemdelegate.h"
#include <QApplication>
#include <QPainter>
#include <QTextDocument>
#include <QStyle>
#include <QAbstractTextDocumentLayout>
#include <QDebug>

#define CSS_TEXTDOC	"c { color: #bdbdbd; font: 14px; font-family: 'Consolas'; }"	\
                        "p { color: #6e7e8e; font : 14px; font-family: 'Consolas'; }"	\
                        "hl { color: #0097fb; font : bold; }"


QTextDocument doc;

CmdItemDelegate::CmdItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    doc.setDefaultStyleSheet(CSS_TEXTDOC);
}

void CmdItemDelegate::keywordChanged(QString keyword)
{
    m_keyword = keyword;
//    QRegExp rxlen("(\\d+)(?:\\s*)(cm|inch)");
    QRegExp rxlen("(\\d+)(\\s*)(cm|inch(es)?)");
     int pos = rxlen.indexIn("Length: 189cm");
     if (pos > -1) {
         QString value = rxlen.cap(1); // "189"
         QString unit = rxlen.cap(2);  // "cm"
         qDebug()<<rxlen.cap(0)<<rxlen.cap(1)<<rxlen.cap(2)<<rxlen.cap(3);
     }
}

void CmdItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    QStyle *style = opt.widget? opt.widget->style() : QApplication::style();

    // Set text
//    doc.setHtml("<c>Add</c><p><hl>A</hl>dd.</p>");
    QString text = opt.text;
//    qDebug()<<1<<text;
//    QString search0, search1;
//    foreach(QChar c, m_keyword)
//        search0 += QString("(\\b%1[a-zA-Z]*\\b)").arg(c) + "(.*)";
//    search1 = m_keyword;
//    QRegExp rx(search0, Qt::CaseInsensitive, QRegExp::RegExp);
//    qDebug()<<2<<search0<<rx.cap(0)<<rx.cap(1)<<rx.cap(2);
//    if (!m_keyword.isEmpty())
//    {
//        int pos = 0;
//        while ((pos = rx.indexIn(text, pos)) != -1)
//        {
//            qDebug()<<3<<text.mid(pos, rx.matchedLength())<<text;
//            text.replace(pos, rx.matchedLength(), QString("<hl>%1</hl>").arg(text.mid(pos, rx.matchedLength())));
//            pos += rx.matchedLength();
//        }
//    }

    doc.setHtml(opt.text);

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

//    doc.setHtml("<c>Add</c><p><hl>A</hl>dd.</p>");
    doc.setHtml(opt.text);
    doc.setTextWidth(opt.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
}

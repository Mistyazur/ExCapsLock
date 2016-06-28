#include "cmditemdelegate.h"
#include "CmdItem/cmditem.h"
#include <QApplication>
#include <QPainter>
#include <QTextDocument>
#include <QStyle>
#include <QAbstractTextDocumentLayout>

#define CSS_TEXTDOC	"h6 { color: #bdbdbd; font: 13px; font-family: 'monospace'; margin: 0px 0px 4px 4px; }"		\
                        "p { color: #6f6f6f; font: 13px; font-family: 'monospace'; margin: 4px 0px 0px 4px; }"		\
                        "hl { color: #0097fb; }"


QTextDocument doc;

CmdItemDelegate::CmdItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    doc.setDefaultStyleSheet(CSS_TEXTDOC);
}

void CmdItemDelegate::keywordChanged(QString keyword)
{
    m_keyword = keyword;
}

void CmdItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRegExp rx("", Qt::CaseInsensitive, QRegExp::RegExp);
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    QStyle *style = opt.widget? opt.widget->style() : QApplication::style();

    // Get caption and pragraph
    QString caption = index.data(CMD_CAPTION).toString();
    QString paragraph = index.data(CMD_PARAGRAPH).toString();

    // Make keyword in caption highlight
    if (!caption.isEmpty() && !m_keyword.isEmpty())
    {
        QString search;
        foreach(QChar c, m_keyword)
            search += QString("(?:\\b(%1)[a-zA-Z]*\\b)").arg(c) + "(?:.*)";
        rx.setPattern(search);
        if (rx.indexIn(caption) != -1)
        {
            for (int i = m_keyword.length(); i > 0; --i)
            {
                int pos = rx.pos(i);
                int len = rx.cap(i).length();
                if (pos != -1)
                {
                    caption.replace(pos, len, highlight(caption.mid(pos, len)));
                }
            }
        }
        else
        {
            int pos = caption.indexOf(m_keyword, 0, Qt::CaseInsensitive);
            caption.replace(pos, m_keyword.length(), highlight(caption.mid(pos, m_keyword.length())));
        }
    }

    // Set text
    if (!caption.isEmpty())
    {
        caption = "<h6>" + caption + "</h6>";
    }
    if(!paragraph.isEmpty())
    {
        paragraph = "<p>" + paragraph + "</p>";
    }
    doc.setHtml(caption + paragraph);

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

    QString caption = index.data(CMD_CAPTION).toString();
    QString paragraph = index.data(CMD_PARAGRAPH).toString();

    if (!caption.isEmpty())
    {
        caption = "<h6>" + caption + "</h6>";
    }
    if(!paragraph.isEmpty())
    {
        paragraph = "<p>" + paragraph + "</p>";
    }

    doc.setHtml(caption + paragraph);
    doc.setTextWidth(opt.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
}

QString CmdItemDelegate::highlight(const QString &text) const
{
    return QString("<hl>%1</hl>").arg(text);
}

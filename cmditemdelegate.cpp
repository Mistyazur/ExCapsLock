#include "cmditemdelegate.h"
#include <QApplication>
#include <QPainter>
#include <QTextDocument>
#include <QStyle>
#include <QAbstractTextDocumentLayout>
#include <QDebug>

#define CSS_TEXTDOC		"h6 { color: #bdbdbd; font: 13px; font-family: 'monospace'; margin: 0px 0px 0px 5px; }"		\
                        "p { color: #6f6f6f; font: 13px; font-family: 'monospace'; margin: 8px 0px 0px 5px; }"		\
                        "hl { color: #0097fb; }"

QTextDocument doc;

CmdItemDelegate::CmdItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    doc.setDefaultStyleSheet(CSS_TEXTDOC);
}

QString CmdItemDelegate::caption(const QString &text)
{
    return QString("<h6>%1</h6>").arg(text);
}

QString CmdItemDelegate::paragraph(const QString &text)
{
    return QString("<p>%1</p>").arg(text);
}

QString CmdItemDelegate::highlight(const QString &text)
{
   return QString("<hl>%1</hl>").arg(text);
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

    // Get caption text
    QString capText = opt.text;
    rx.setPattern(S_CAPTION(".+"));
    if (rx.indexIn(capText) != -1)
        capText = rx.cap(0);
    else
        capText = opt.text;
    qDebug()<<capText;

    // Make keyword highlight
    QString text = opt.text;
    if (!m_keyword.isEmpty())
    {
        QString search;
        foreach(QChar c, m_keyword)
            search += QString("(?:\\b(%1)[a-zA-Z]*\\b)").arg(c) + "(?:.*)";
        rx.setPattern(search);
        if (rx.indexIn(capText) != -1)
        {
            for (int i = m_keyword.length(); i > 0; --i)
            {
                int pos = rx.pos(i);
                int len = rx.cap(i).length();
                if (pos != -1)
                {
                    text.replace(pos, len, S_HIGHLIGHT(text.mid(pos, len)));
                }
            }
        }
        else
        {
            text.replace(capText.indexOf(m_keyword, 0, Qt::CaseInsensitive),
                         m_keyword.length(),
                         S_HIGHLIGHT(text.mid(capText.indexOf(m_keyword, 0, Qt::CaseInsensitive), m_keyword.length())));
        }
    }

    // Set text
    doc.setHtml(text);

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

    doc.setHtml(opt.text);
    doc.setTextWidth(opt.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
}

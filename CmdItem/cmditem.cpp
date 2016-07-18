#include "cmditem.h"

CmdItem::CmdItem(const QString &text, QObject *parent) :
    QStandardItem(text), QObject(parent)
{
    m_resultModel = new QStandardItemModel();
}

CmdItem::~CmdItem()
{
    delete m_resultModel;
}

void CmdItem::autoUpdate()
{

}

bool CmdItem::isAutoUpdate()
{
    return false;
}

const QString CmdItem::html(const QString &searchKeyword)
{
    return QString("<h6>%1</h6>").arg(highlight(text(), searchKeyword));
}

QStandardItemModel *CmdItem::resultModel()
{
    return m_resultModel;
}

const QString CmdItem::highlight(QString source, const QString &keyword)
{
    QRegExp rx("", Qt::CaseInsensitive, QRegExp::RegExp);

    if (!source.isEmpty() && !keyword.isEmpty())
    {
        QString search;
        foreach(QChar c, keyword)
            search += QString("(?:\\b(%1)[a-zA-Z]*\\b)").arg(c) + "(?:.*)";
        rx.setPattern(search);
        if (rx.indexIn(source) != -1)
        {
            for (int i = keyword.length(); i > 0; --i)
            {
                int pos = rx.pos(i);
                int len = rx.cap(i).length();
                if (pos != -1)
                {
                    source.replace(pos, len, QString("<hl>%1</hl>").arg(source.mid(pos, len)));
                }
            }
        }
        else
        {
            int pos = source.indexOf(keyword, 0, Qt::CaseInsensitive);
            source.replace(pos, keyword.length(), QString("<hl>%1</hl>").arg(source.mid(pos, keyword.length())));
        }
    }

    return source;
}

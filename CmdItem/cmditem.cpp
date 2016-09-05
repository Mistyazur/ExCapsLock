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

const QString CmdItem::html(const QString &search)
{
    return QString("<h6>%1</h6>").arg(highlight(text(), search));
}

QStandardItemModel *CmdItem::resultModel()
{
    return m_resultModel;
}

const QString CmdItem::highlight(QString source, const QString &search)
{
    if (!source.isEmpty() && !search.isEmpty())
    {
        QRegExp rx(search, Qt::CaseInsensitive, QRegExp::RegExp);
        if (rx.indexIn(source) != -1)
        {
            for (int i = rx.captureCount(); i >= 1; --i)
            {
                int pos = rx.pos(i);
                int len = rx.cap(i).length();
                if (pos != -1) {
                    source.replace(pos, len, QString("<hl>%1</hl>").arg(source.mid(pos, len)));
                }
            }
        }
    }

    return source;
}

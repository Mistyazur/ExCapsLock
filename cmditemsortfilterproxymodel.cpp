#include "cmditemsortfilterproxymodel.h"

#define MATCH_NONE		0
#define MATCH_CONTAINS	1
#define MATCH_INITIAL	2

CmdItemSortFilterProxyModel::CmdItemSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

void CmdItemSortFilterProxyModel::searchChanged(QString searchText)
{
    m_serachText = searchText;
}

bool CmdItemSortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    bool bRet = false;
    QString &left = source_left.data().toString();
    QString &right = source_right.data().toString();

    int leftMatchType = matchType(left);
    int rightMatchType = matchType(right);
    if (leftMatchType == rightMatchType)
    {
        bRet = (left.localeAwareCompare(right) <= 0);
    }
    else
    {
        bRet = (leftMatchType > rightMatchType);
    }

    return bRet;
}

int CmdItemSortFilterProxyModel::matchType(const QString &text) const
{
    int nRet = MATCH_NONE;

    QRegExp rx("", Qt::CaseInsensitive, QRegExp::RegExp);
    if (!m_serachText.isEmpty())
    {
        QString search;
        foreach(QChar c, m_serachText)
            search += QString("\\b%1[a-zA-Z]*\\b").arg(c) + ".*";
        rx.setPattern(search);
        if (rx.indexIn(text) != -1)
        {
            nRet = MATCH_INITIAL;
        }
        else if (text.indexOf(m_serachText, 0, Qt::CaseInsensitive) != -1)
        {
            nRet = MATCH_CONTAINS;
        }
    }

    return nRet;
}

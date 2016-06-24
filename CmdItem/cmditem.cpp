#include "cmditem.h"

CmdItem::CmdItem(const QString &text) :
    QStandardItem(text)
{

}

const QStandardItemModel &CmdItem::resModel()
{
    return m_resModel;
}

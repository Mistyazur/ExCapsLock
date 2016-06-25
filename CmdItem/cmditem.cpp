#include "cmditem.h"

CmdItem::CmdItem(const QString &text) :
    QStandardItem(text)
{
    m_resModel = new QStandardItemModel();
}

CmdItem::~CmdItem()
{
    delete m_resModel;
}

QStandardItemModel *CmdItem::resModel()
{
    return m_resModel;
}

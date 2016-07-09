#include "cmditem.h"

CmdItem::CmdItem(const QString &text, QObject *parent) :
    QStandardItem(text), QObject(parent)
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

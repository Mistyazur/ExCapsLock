#ifndef CMDITEM_H
#define CMDITEM_H

#include <QStandardItem>
#include <QStandardItemModel>

#define CMD_CAPTION	Qt::DisplayRole
#define CMD_PARAGRAPH	Qt::UserRole + 1

class CmdItem : public QStandardItem
{
public:
    CmdItem(const QString &text);
    ~CmdItem();
    virtual bool exec() = 0;
    QStandardItemModel *resModel();
protected:
    QStandardItemModel *m_resModel;
};

#endif // CMDITEM_H

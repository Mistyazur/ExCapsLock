#ifndef CMDITEM_H
#define CMDITEM_H

#include <QStandardItem>
#include <QStandardItemModel>

#define CMD_FAILED	0
#define CMD_SUCCEED	1
#define CMD_QUERY	2

#define CMD_CAPTION	Qt::DisplayRole
#define CMD_PARAGRAPH	Qt::UserRole + 1

class CmdItem : public QStandardItem
{
public:
    CmdItem(const QString &text);
    ~CmdItem();
    virtual int exec() = 0;
    QStandardItemModel *resModel();
protected:
    QStandardItemModel *m_resModel;
};

#endif // CMDITEM_H

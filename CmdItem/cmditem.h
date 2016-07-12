#ifndef CMDITEM_H
#define CMDITEM_H

#include <QStandardItem>
#include <QStandardItemModel>

class CmdItem : public QStandardItem , public QObject
{
public:
    CmdItem(const QString &text, QObject *parent = nullptr);
    ~CmdItem();
    virtual bool exec() = 0;
    virtual const QString html(const QString &searchKeyword);
    QStandardItemModel *resModel();
protected:
    QStandardItemModel *m_resModel;

    const QString highlight(QString source, const QString &keyword);
};

#endif // CMDITEM_H

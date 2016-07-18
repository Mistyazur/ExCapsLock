#ifndef CMDITEM_H
#define CMDITEM_H

#include <QStandardItem>
#include <QStandardItemModel>


class CmdItem : public QObject, public QStandardItem
{
    Q_OBJECT
public:
    CmdItem(const QString &text, QObject *parent = nullptr);
    ~CmdItem();

    virtual bool exec() = 0;
    virtual const QString html(const QString &searchKeyword);

    QStandardItemModel *resultModel();
public slots:
    virtual void reset();
protected:
    QStandardItemModel *m_resultModel;

    const QString highlight(QString source, const QString &keyword);
};

#endif // CMDITEM_H

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
    virtual void autoUpdate();
    virtual bool isAutoUpdate();
    virtual const QString html(const QString &search);

    QStandardItemModel *resultModel();
public slots:
protected:
    QStandardItemModel *m_resultModel;

    const QString highlight(QString source, const QString &search);
};

#endif // CMDITEM_H

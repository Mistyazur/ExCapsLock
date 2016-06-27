#ifndef CMDITEMSORTFILTERPROXYMODEL_H
#define CMDITEMSORTFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class CmdItemSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    CmdItemSortFilterProxyModel(QObject *parent = 0);
public slots:
    void keywordChanged(QString keyword);
protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;
private:
    QString m_keyword;

    int matchType(const QString &text) const;
};

#endif // CMDITEMSORTFILTERPROXYMODEL_H

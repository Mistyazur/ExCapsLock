#ifndef CMDITEMSORTFILTERPROXYMODEL_H
#define CMDITEMSORTFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class CmdItemSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    CmdItemSortFilterProxyModel(QObject *parent = 0);
};

#endif // CMDITEMSORTFILTERPROXYMODEL_H

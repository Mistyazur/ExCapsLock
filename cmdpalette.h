#ifndef CMDPALETTE_H
#define CMDPALETTE_H

#include "shadowwidget.h"
#include "cmditemsortfilterproxymodel.h"
#include "cmditemdelegate.h"
#include "CmdItem/cmditem.h"
#include <QTimer>
#include <QKeyEvent>
#include <QStandardItemModel>

namespace Ui {
class Cmd;
}

class CmdPalette : public ShadowWidget
{
    Q_OBJECT

public:
    explicit CmdPalette(ShadowWidget *parent = 0);
    ~CmdPalette();
    void activate();
    void reset();
signals:
    void searchChanged(QString searchText);
public slots:
    void cmdActivate(const QModelIndex &index);
    void autoUpdate();
    void textChanged();
protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::Cmd *ui;
    QTimer m_autoUpdateTimer;
    QStandardItemModel *m_stdModel;
    CmdItemSortFilterProxyModel *m_proxyModel;
    CmdItemDelegate *m_delegate;
    CmdItem *m_currentItem;

    void addItemToSourceModel(CmdItem *item);
    void updateCmdView(QStandardItemModel *model);
};

#endif // CMDPALETTE_H

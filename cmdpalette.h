#ifndef CMDPALETTE_H
#define CMDPALETTE_H

#include "shadowwidget.h"
#include "cmditemsortfilterproxymodel.h"
#include "cmditemdelegate.h"
#include "CmdItem/cmditem.h"
#include "cmditem/systemcmd.h"
#include "cmditem/power.h"
#include "CmdItem/applister.h"
#include "CmdItem/appregister.h"
#include "CmdItem/proclister.h"
#include "CmdItem/winlister.h"
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
    bool activate();
    void activateWindowSearch();
    void deactivate();
signals:
    void searchChanged(QString search);
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
    CmdItem *m_currCmdItem;
    Power *m_powerSleep;
    Power *m_powerHibernate;
    Power *m_powerShutDown;
    Power *m_powerRestart;
    Power *m_powerScreenSaver;
    AppLister *m_appLaunchAsUserLister;
    AppLister *m_appLaunchAsSystemLister;
    AppRegister *m_appNew;
    ProcLister *m_procList;
    winLister *m_winList;

    void cmdExecute();
    void updateCmdView(QStandardItemModel *model);
};

#endif // CMDPALETTE_H

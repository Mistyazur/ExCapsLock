#ifndef CMDPALETTE_H
#define CMDPALETTE_H

#include "shadowwidget.h"
#include "cmditemsortfilterproxymodel.h"
#include "cmditemdelegate.h"
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
    void keywordChanged(QString keyword);
public slots:
    void textChanged();
    void cmdActivate(const QModelIndex &index);
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::Cmd *ui;
    QStandardItemModel *m_stdModel;
    CmdItemSortFilterProxyModel *m_proxyModel;
    CmdItemDelegate *m_delegate;

    void updateCmdView(QStandardItemModel *model);
};

#endif // CMDPALETTE_H

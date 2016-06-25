#ifndef CMDPALETTE_H
#define CMDPALETTE_H

#include <QKeyEvent>
#include <QStandardItemModel>
#include "shadowwidget.h"

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
public slots:
    void textEdited();
    void cmdActivate(const QModelIndex &index);
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::Cmd *ui;
    QStandardItemModel *m_stdModel;

    void updateList(QStandardItemModel *model);
};

#endif // CMDPALETTE_H

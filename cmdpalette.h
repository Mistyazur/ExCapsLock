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
public slots:
    void textEdited();
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::Cmd *ui;
    QStandardItemModel *m_stdModel;
};

#endif // CMDPALETTE_H

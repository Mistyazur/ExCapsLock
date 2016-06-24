#ifndef CMDPALETTE_H
#define CMDPALETTE_H

#include <QWidget>
#include <QKeyEvent>
#include <QPixmap>
#include <QStandardItemModel>

namespace Ui {
class Cmd;
}

class CmdPalette : public QWidget
{
    Q_OBJECT

public:
    explicit CmdPalette(QWidget *parent = 0);
    ~CmdPalette();
public slots:
    void textEdited();
protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::Cmd *ui;
    QPixmap m_shadow;
    QStandardItemModel* m_stdModel;
};

#endif // CMDPALETTE_H

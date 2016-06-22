#ifndef CMD_H
#define CMD_H

#include <QWidget>
#include <QKeyEvent>
#include <QPixmap>
#include <QStandardItemModel>

namespace Ui {
class Cmd;
}

class Cmd : public QWidget
{
    Q_OBJECT

public:
    explicit Cmd(QWidget *parent = 0);
    ~Cmd();
public slots:
    void textEdited();
protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::Cmd *ui;
    QPixmap m_shadow;
    QStandardItemModel* m_stdModel;
};

#endif // CMD_H

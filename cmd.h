#ifndef CMD_H
#define CMD_H

#include <QWidget>

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
private:
    Ui::Cmd *ui;
};

#endif // CMD_H

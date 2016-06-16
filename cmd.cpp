#include "cmd.h"
#include "ui_cmd.h"

Cmd::Cmd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cmd)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Cmd::~Cmd()
{
    delete ui;
}

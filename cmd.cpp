#include "cmd.h"
#include "ui_cmd.h"
#include <Windows.h>
#include <QDebug>
#include <QStringListModel>


Cmd::Cmd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cmd)
{
    // Set window attributes
    HWND hwnd = (HWND)winId();
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    ::SetWindowLong(hwnd, GWL_EXSTYLE, ::GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);

    // Set UI
    ui->setupUi(this);
//    ui->listView->hide();

    connect(ui->lineEdit, &QLineEdit::textEdited, this, &Cmd::textEdited);
    qDebug()<<ui->listView->sizeHint()<<ui->lineEdit->sizeHint();
}

Cmd::~Cmd()
{
    delete ui;
}

void Cmd::textEdited()
{
//    qDebug()<<"1";
    int n = ui->lineEdit->text().toInt();
    QStringList num;
    for (int i = 0; i < n ; i++)
    {
        qDebug()<<i;
        num.append(QString::number(i));
    }
    //    QStringList num;
    //    num<<QString("1")<<QString("2")<<QString("3")<<QString("4");
        QStringListModel *model = new QStringListModel(num);
        ui->listView->setModel(model);
}

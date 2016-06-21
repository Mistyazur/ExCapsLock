#include "cmd.h"
#include "ui_cmd.h"
#include <Windows.h>
#include <QDebug>
#include <QtMath>
#include <QStringListModel>
#include <QPainter>
#include <QPainterPath>

Cmd::Cmd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cmd),
    m_shadow("../ExCapsLock/src/window_shadow.png")
{
    // Set window attributes
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // Set UI
    setFixedSize(600, 400);
    ui->setupUi(this);
//    ui->listView->hide()

    // Activate this window
    activateWindow();

    connect(ui->lineEdit, &QLineEdit::textEdited, this, &Cmd::textEdited);
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

// Draw shadow
void Cmd::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect bottom(5, 136, 200, 7);
    QRect top(5, 0, 200, 3);
    QRect left(0, 3, 5, 133);
    QRect right(205, 3, 5, 133);
    QRect topRight(205, 0, 5, 3);
    QRect topLeft(0, 0, 5, 3);
    QRect bottomLeft(0, 136, 5, 7);
    QRect bottomRight(205, 136, 5, 7);

    QRect tBottom(5, this->height() - 7, this->width() - 10, 7);
    QRect tTop(5, 0, this->width() - 10, 3);
    QRect tLeft(0, 3, 5, this->height() - 10);
    QRect tRight(this->width() - 5, 3, 5, this->height() - 10);
    QRect tTopLeft(0, 0, 5, 3);
    QRect tTopRight(this->width() - 5, 0, 5, 3);
    QRect tBottomLeft(0, this->height() - 7, 5, 7);
    QRect tBottomRight(this->width() - 5, this->height() - 7, 5, 7);

    painter.drawPixmap(tBottom, m_shadow, bottom);
    painter.drawPixmap(tTop, m_shadow, top);
    painter.drawPixmap(tLeft, m_shadow, left);
    painter.drawPixmap(tRight, m_shadow, right);
    painter.drawPixmap(tTopRight, m_shadow, topRight);
    painter.drawPixmap(tTopLeft, m_shadow, topLeft);
    painter.drawPixmap(tBottomLeft, m_shadow, bottomLeft);
    painter.drawPixmap(tBottomRight, m_shadow, bottomRight);
}

void Cmd::keyPressEvent(QKeyEvent *event)
{
    if (event->nativeVirtualKey() == VK_ESCAPE)
        hide();
}

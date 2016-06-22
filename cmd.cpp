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
//    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // Set UI
    setFixedSize(600, 400);
    ui->setupUi(this);
//    ui->listView->hide();
    ui->listView->setFocusPolicy(Qt::NoFocus);

    // Activate this window
    activateWindow();

    // Model for list view
    m_stdModel = new QStandardItemModel(this);
    m_stdModel->setItem(0, 0,new QStandardItem("1"));
    m_stdModel->setItem(1, 0,new QStandardItem("2"));
    m_stdModel->setItem(2, 0,new QStandardItem("3"));
    ui->listView->setModel(m_stdModel);
    QModelIndex index = m_stdModel->index(1, 0);
    ui->listView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);

    connect(ui->lineEdit, &QLineEdit::textEdited, this, &Cmd::textEdited);
}

Cmd::~Cmd()
{
    delete ui;
}

void Cmd::textEdited()
{
////    qDebug()<<"1";
//    int n = ui->lineEdit->text().toInt();
//    QStringList num;
//    for (int i = 0; i < n ; i++)
//    {
//        qDebug()<<i;
//        num.append(QString::number(i));
//    }
//    //    QStringList num;
//    //    num<<QString("1")<<QString("2")<<QString("3")<<QString("4");
//        QStringListModel *model = new QStringListModel(num);
//        ui->listView->setModel(model);
//        QModelIndex index = model->createIndex(0, 0);
//        if ( index.isValid() )
//             ui->listView->selectionModel()->select( index, QItemSelectionModel::Select );

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
    else if (event->nativeVirtualKey() == VK_RETURN)
    {

        foreach(const QModelIndex &index, ui->listView->selectionModel()->selectedIndexes()){
               qDebug()<<index.data(Qt::DisplayRole).toString();
           }
    }
    else if (event->nativeVirtualKey() == VK_DOWN)
    {
        QItemSelectionModel *selectionModel = ui->listView->selectionModel();
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        {
            qDebug()<<index.data(Qt::DisplayRole).toString() << index.row() <<index.column() << m_stdModel->rowCount();
            if (index.row() < m_stdModel->rowCount() - 1)
            {
                selectionModel->select(m_stdModel->index(index.row() + 1, index.column()), QItemSelectionModel::ClearAndSelect);
            }
        }
    }
    else if (event->nativeVirtualKey() == VK_UP)
    {
        QItemSelectionModel *selectionModel = ui->listView->selectionModel();
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        {
            qDebug()<<index.data(Qt::DisplayRole).toString() << index.row() <<index.column();
            if (index.row() > 0)
            {
                selectionModel->select(m_stdModel->index(index.row() - 1, index.column()), QItemSelectionModel::ClearAndSelect);
            }
        }
    }
}

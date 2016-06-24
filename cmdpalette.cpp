#include "cmdpalette.h"
#include "ui_cmd.h"
#include "systemcmd.h"
#include "power.h"
#include <Windows.h>
#include <QDebug>
#include <QtMath>
#include <QPainter>


CmdPalette::CmdPalette(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cmd),
    m_shadow("../ExCapsLock/src/window_shadow.png")
{
    // Set window attributes
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // Activate this window. Function "activateWindow" doesn't work.
    setWindowState(Qt::WindowActive);

    // Set UI
    setFixedSize(600, 400);
    ui->setupUi(this);
    ui->listView->setFocusPolicy(Qt::NoFocus);

    // Model for list view
    m_stdModel = new QStandardItemModel(this);
    ui->listView->setModel(m_stdModel);

    m_stdModel->setItem(0, 0, new Power("Power: Sleep", 0));
    m_stdModel->setItem(1, 0, new Power("Power: Hibernate", 1));
    m_stdModel->setItem(2, 0, new Power("Power: Shut Down", 2));
    m_stdModel->setItem(3, 0, new Power("Power: Restart", 3));
    ui->listView->selectionModel()->setCurrentIndex(m_stdModel->index(0, 0), QItemSelectionModel::ClearAndSelect);

    connect(ui->lineEdit, &QLineEdit::textEdited, this, &CmdPalette::textEdited);
}

CmdPalette::~CmdPalette()
{
    delete ui;
}

void CmdPalette::textEdited()
{
}

// Draw shadow
void CmdPalette::paintEvent(QPaintEvent *)
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

void CmdPalette::keyPressEvent(QKeyEvent *event)
{
    if (event->nativeVirtualKey() == VK_ESCAPE)
        hide();
    else if (event->nativeVirtualKey() == VK_RETURN)
    {
        foreach(const QModelIndex &index, ui->listView->selectionModel()->selectedIndexes())
        {
            ((CmdItem *)m_stdModel->itemFromIndex(index))->exec();
        }
    }
    else if (event->nativeVirtualKey() == VK_DOWN)
    {
        QApplication::postEvent(ui->listView, new QKeyEvent(event->type(), event->key(), event->modifiers()));
    }
    else if (event->nativeVirtualKey() == VK_UP)
    {
        QApplication::postEvent(ui->listView, new QKeyEvent(event->type(), event->key(), event->modifiers()));
//        QItemSelectionModel *selectionModel = ui->listView->selectionModel();
//        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
//        {
//            qDebug()<<index.data(Qt::DisplayRole).toString() << index.row() <<index.column();
//            if (index.row() > 0)
//            {
//                selectionModel->select(m_stdModel->index(index.row() - 1, index.column()), QItemSelectionModel::ClearAndSelect);
//            }
//        }
    }
}

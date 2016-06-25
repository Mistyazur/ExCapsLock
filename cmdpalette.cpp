#include "cmdpalette.h"
#include "ui_cmd.h"
#include "cmditem/systemcmd.h"
#include "cmditem/power.h"
#include "CmdItem/apps.h"
#include <QDebug>
#include <QtMath>
#include <QPainter>
#include <Windows.h>

CmdPalette::CmdPalette(ShadowWidget *parent) :
    ShadowWidget(parent),
    ui(new Ui::Cmd)
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
    ui->listView->setWordWrap(true);

    // Model for list view
    m_stdModel = new QStandardItemModel(this);
    ui->listView->setModel(m_stdModel);

    m_stdModel->setItem(0, new Power("Power: Sleep", 0));
    m_stdModel->setItem(1, new Power("Power: Hibernate", 1));
    m_stdModel->setItem(2, new Power("Power: Shut Down", 2));
    m_stdModel->setItem(3, new Power("Power: Restart", 3));
    m_stdModel->setItem(4, new Apps("Run App"));
    ui->listView->selectionModel()->setCurrentIndex(m_stdModel->index(0, 0), QItemSelectionModel::ClearAndSelect);

    connect(ui->lineEdit, &QLineEdit::textEdited, this, &CmdPalette::textEdited);
    connect(ui->listView, &QListView::activated, this, &CmdPalette::cmdActivate);
}

CmdPalette::~CmdPalette()
{
    delete ui;
}

void CmdPalette::textEdited()
{
}

void CmdPalette::cmdActivate(const QModelIndex &index)
{
    CmdItem *item = (CmdItem *)((QStandardItemModel *)ui->listView->model())->itemFromIndex(index);
    item->exec();
    if (item->resModel()->columnCount() > 0)
    {
        ui->listView->setModel(item->resModel());
    }
}

void CmdPalette::keyPressEvent(QKeyEvent *event)
{
    if (event->nativeVirtualKey() == VK_ESCAPE)
        hide();
    else if (event->nativeVirtualKey() == VK_RETURN)
    {
        QApplication::postEvent(ui->listView, new QKeyEvent(event->type(), event->key(), event->modifiers()));
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

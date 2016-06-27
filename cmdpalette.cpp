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

    // Set UI
    setFixedSize(600, 400);
    ui->setupUi(this);
    ui->listView->setFocusPolicy(Qt::NoFocus);
    ui->listView->setWordWrap(true);

    // Proxy model for list view
    m_proxyModel = new CmdItemSortFilterProxyModel(this);

    // Source model for list view
    m_stdModel = new QStandardItemModel(this);
//    m_stdModel->setItem(0, new Power("Power: Sleep", 0));
//    m_stdModel->setItem(1, new Power("Power: Hibernate", 1));
//    m_stdModel->setItem(2, new Power("Power: Shut Down", 2));
//    m_stdModel->setItem(3, new Power("Power: Restart", 3));
//    m_stdModel->setItem(4, new Apps("Run App"));
    m_stdModel->setItem(0, new QStandardItem("<c>Install My Extension<\c>"));
    m_stdModel->setItem(1, new QStandardItem("<c>Extension: Install Extension<\c>"));
    m_stdModel->setItem(2, new QStandardItem("<c>Extension: View<\c>"));
    m_stdModel->setItem(3, new QStandardItem("<c>Extension: email<\c>"));
    updateCmdView(m_stdModel);

    // Delegate for highlight input matches
    m_delegate = new CmdItemDelegate(this);
    ui->listView->setItemDelegate(m_delegate);

    connect(ui->lineEdit, &QLineEdit::textEdited, this, &CmdPalette::textEdited);
    connect(ui->listView, &QListView::activated, this, &CmdPalette::cmdActivate);
    connect(this, &CmdPalette::keywordChanged, m_delegate, &CmdItemDelegate::keywordChanged);

    activate();
}

CmdPalette::~CmdPalette()
{
    delete ui;
}

void CmdPalette::activate()
{
    if (!isVisible())
    {
        // Show and activate this window. Function "activateWindow" doesn't work.
        show();
        setWindowState(Qt::WindowNoState);
        setWindowState(Qt::WindowActive);
//        ::SetFocus((HWND)winId());
    }
}

void CmdPalette::textEdited()
{
    // Install My Extension
    // Extension: Install Extension
    // Extension: View
    // Extension: email
    // (\bi[a-zA-Z]*\b.*\be[a-zA-Z]*\b)|(.*ie.*)
    QString search, search0, search1;
    foreach(QChar c, ui->lineEdit->text())
        search0 += QString("\\b%1[a-zA-Z]*\\b").arg(c) + ".*";
    search1 = ui->lineEdit->text();
    search = QString("(%1)|(%2)").arg(search0).arg(search1);

    // Set filter
    QRegExp regExp(search, Qt::CaseInsensitive, QRegExp::RegExp);
    m_proxyModel->setFilterRegExp(regExp);

    // Set highlight
    emit keywordChanged(ui->lineEdit->text());

    // Update
    ui->listView->update();
}

void CmdPalette::cmdActivate(const QModelIndex &index)
{
    CmdItem *item = (CmdItem *)((QStandardItemModel *)m_proxyModel->sourceModel())->itemFromIndex(m_proxyModel->mapToSource(index));
    item->exec();
    if (item->resModel()->columnCount() > 0)
    {
        updateCmdView(item->resModel());
    }
    else
    {
        reset();
    }
}

void CmdPalette::keyPressEvent(QKeyEvent *event)
{
    if (event->nativeVirtualKey() == VK_ESCAPE)
    {
        reset();
    }
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

void CmdPalette::reset()
{
    ui->lineEdit->clear();
    updateCmdView(m_stdModel);
    hide();
}

void CmdPalette::updateCmdView(QStandardItemModel *model)
{
    m_proxyModel->setSourceModel(model);
    m_proxyModel->sort(0, Qt::AscendingOrder);
    ui->listView->setModel(m_proxyModel);
    ui->listView->selectionModel()->setCurrentIndex(m_proxyModel->index(0, 0), QItemSelectionModel::ClearAndSelect);
}

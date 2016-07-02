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
    m_stdModel->setItem(0, new Power("Power: Sleep", 0));
    m_stdModel->setItem(1, new Power("Power: Hibernate", 1));
    m_stdModel->setItem(2, new Power("Power: Shut Down", 2));
    m_stdModel->setItem(3, new Power("Power: Restart", 3));
    m_stdModel->setItem(4, new Power("Power: Save Screen", 4));
    m_stdModel->setItem(5, new Apps("Run App"));
//    m_stdModel->setItem(0, new QStandardItem("Install My Extension"));
//    m_stdModel->setItem(1, new QStandardItem("Extension: Install Extension"));
//    m_stdModel->setItem(2, new QStandardItem("Extension: View"));
//    m_stdModel->setItem(3, new QStandardItem("Extension: email"));
    updateCmdView(m_stdModel);

    // Delegate for highlight input matches
    m_delegate = new CmdItemDelegate(this);
    ui->listView->setItemDelegate(m_delegate);

    connect(ui->lineEdit, &QLineEdit::textChanged, this, &CmdPalette::textChanged);
    connect(ui->listView, &QListView::activated, this, &CmdPalette::cmdActivate);
    connect(this, &CmdPalette::keywordChanged, m_delegate, &CmdItemDelegate::keywordChanged);
    connect(this, &CmdPalette::keywordChanged, m_proxyModel, &CmdItemSortFilterProxyModel::keywordChanged);
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

void CmdPalette::textChanged()
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

    // Set highlight and sort by keyword
    emit keywordChanged(ui->lineEdit->text());

    // Update to make highlight effect
    ui->listView->update();

    // Sort to make affect
    m_proxyModel->sort(-1, Qt::AscendingOrder);
    m_proxyModel->sort(0, Qt::AscendingOrder);

    // Select first item
    ui->listView->selectionModel()->setCurrentIndex(m_proxyModel->index(0, 0), QItemSelectionModel::ClearAndSelect);
}

void CmdPalette::cmdActivate(const QModelIndex &index)
{
    CmdItem *item = (CmdItem *)((QStandardItemModel *)m_proxyModel->sourceModel())->itemFromIndex(m_proxyModel->mapToSource(index));
    item->exec();

    if (item->resModel()->rowCount() > 0)
    {
        ui->lineEdit->clear();
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
    // Set model
    m_proxyModel->setSourceModel(model);
    ui->listView->setModel(m_proxyModel);

    // Clear filter regexp
    m_proxyModel->setFilterRegExp("");

    // Sort
    m_proxyModel->sort(-1, Qt::AscendingOrder);
    m_proxyModel->sort(0, Qt::AscendingOrder);

    // Select first item
    ui->listView->selectionModel()->setCurrentIndex(m_proxyModel->index(0, 0), QItemSelectionModel::ClearAndSelect);
}

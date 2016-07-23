#include "cmdpalette.h"
#include "ui_cmd.h"
#include "cmditem/systemcmd.h"
#include "cmditem/power.h"
#include "CmdItem/applister.h"
#include "CmdItem/appregister.h"
#include "CmdItem/proclister.h"
#include "CmdItem/winlister.h"
#include <QDebug>
#include <QtMath>
#include <QPainter>
#include <Windows.h>

CmdPalette::CmdPalette(ShadowWidget *parent) :
    ShadowWidget(parent),
    ui(new Ui::Cmd)
{
    m_currentItem = nullptr;

    // Set window attributes

    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint/* | Qt::WindowStaysOnTopHint*/);
    setAttribute(Qt::WA_TranslucentBackground);

    // Set window topmost (Qt::WindowStaysOnTopHint does not work when run as administrator)

    ::SetWindowPos((HWND)winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

    // Set UI

    setFixedSize(600, 400);
    ui->setupUi(this);
    ui->listView->setFocusPolicy(Qt::NoFocus);

    // Proxy model for list view

    m_proxyModel = new CmdItemSortFilterProxyModel(this);

    // Source model for list view

    m_stdModel = new QStandardItemModel(this);

    Power *powerSleep = new Power("Power: Sleep", 0, this);
    Power *powerHibernate = new Power("Power: Hibernate", 1, this);
    Power *powerShutDown = new Power("Power: Shut Down", 2, this);
    Power *powerRestart = new Power("Power: Restart", 3, this);
    Power *powerScreenSaver = new Power("Power: Screen Saver", 4, this);
    AppLister *appList = new AppLister("App: Run", this);
    AppRegister *appNew = new AppRegister("App: New", this);
    ProcLister *procList = new ProcLister("Process: Kill", this);
    winLister *winList = new winLister("Window: Switch");

    connect(appNew, &AppRegister::updateApps, appList, &AppLister::updateApps);

    addItemToSourceModel(powerSleep);
    addItemToSourceModel(powerHibernate);
    addItemToSourceModel(powerShutDown);
    addItemToSourceModel(powerRestart);
    addItemToSourceModel(powerScreenSaver);
    addItemToSourceModel(appList);
    addItemToSourceModel(appNew);
    addItemToSourceModel(procList);
    addItemToSourceModel(winList);
    updateCmdView(m_stdModel);

    // Delegate for highlight input matches

    m_delegate = new CmdItemDelegate(this);
    ui->listView->setItemDelegate(m_delegate);

    connect(ui->lineEdit, &QLineEdit::textChanged, this, &CmdPalette::textChanged);
    connect(ui->listView, &QListView::activated, this, &CmdPalette::cmdActivate);
    connect(this, &CmdPalette::searchChanged, m_delegate, &CmdItemDelegate::searchChanged);

   // Timer for updating list view automatically

    connect(&m_autoUpdateTimer, &QTimer::timeout, this, &CmdPalette::autoUpdate);
}

CmdPalette::~CmdPalette()
{
    delete ui;
}

void CmdPalette::activate()
{
    if (!isVisible())
    {
        // Show
        show();

        // Set window foreground
        ::SetForegroundWindow((HWND)winId());

        // Set line edit focus
        ui->lineEdit->setFocus();
    }
}

void CmdPalette::reset()
{
    // Clear text.
    ui->lineEdit->clear();

    // Update list view to standard model
    updateCmdView(m_stdModel);
    hide();
}

void CmdPalette::cmdActivate(const QModelIndex &index)
{
    m_currentItem = (CmdItem *)((QStandardItemModel *)m_proxyModel->sourceModel())->itemFromIndex(m_proxyModel->mapToSource(index));
    m_currentItem->exec();

    if (m_currentItem->resultModel()->rowCount() > 0)
    {
        ui->lineEdit->clear();
        updateCmdView(m_currentItem->resultModel());

        // Update every second if it's auto-updated
        if (m_currentItem->isAutoUpdate())
            m_autoUpdateTimer.start(1000);
    }
    else
    {
        reset();
    }
}

void CmdPalette::autoUpdate()
{
    if (m_currentItem)
    {
        m_currentItem->autoUpdate();

        // Update list view
        ((QWidget *)ui->listView)->update();
    }
}

void CmdPalette::textChanged()
{
    QString search;
    foreach(QChar c, ui->lineEdit->text())
        search += QString("(?:.*\\b)?(%1)").arg(c);
    search += "(?:.*)";
    qDebug()<<search;

    // Set filter
    QRegExp regExp(search, Qt::CaseInsensitive, QRegExp::RegExp);
    m_proxyModel->setFilterRegExp(regExp);

    // Set highlight and sort by keyword
//    emit searchChanged(ui->lineEdit->text());
    emit searchChanged(search);

//    // Update to make highlight effect
//    ui->listView->update();

    // Sort to make affect
    m_proxyModel->sort(-1, Qt::AscendingOrder);
    m_proxyModel->sort(0, Qt::AscendingOrder);

    // Select first item
    ui->listView->selectionModel()->setCurrentIndex(m_proxyModel->index(0, 0), QItemSelectionModel::ClearAndSelect);
}

void CmdPalette::closeEvent(QCloseEvent *event)
{
    // Ignore ALT+F4
    event->ignore();
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

void CmdPalette::addItemToSourceModel(CmdItem *item)
{
    static int index = 0;

    m_stdModel->setItem(index, item);

    ++index;
}

void CmdPalette::updateCmdView(QStandardItemModel *model)
{
    // Stop timer
    m_autoUpdateTimer.stop();

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

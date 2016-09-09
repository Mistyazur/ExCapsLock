#include "cmdpalette.h"
#include "ui_cmd.h"

#include <QDebug>
#include <QtMath>
#include <QPainter>
#include <Windows.h>

#define UPDATE_INTERVAL	1000

CmdPalette::CmdPalette(ShadowWidget *parent) :
    ShadowWidget(parent),
    ui(new Ui::Cmd)
{
    m_currCmdItem = nullptr;

    // Set window attributes

    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint/* | Qt::WindowStaysOnTopHint*/);
    setAttribute(Qt::WA_TranslucentBackground);

    // Set window topmost (Qt::WindowStaysOnTopHint does not work when run as administrator)
    ::SetWindowPos((HWND)winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

    // Set UI

    setFixedSize(600, 400);
    ui->setupUi(this);

    // Proxy model for list view
    m_proxyModel = new CmdItemSortFilterProxyModel(this);

    // Source model for list view
    m_stdModel = new QStandardItemModel(this);

    m_powerSleep = new Power("Power: Sleep", 0, this);
    m_powerHibernate = new Power("Power: Hibernate", 1, this);
    m_powerShutDown = new Power("Power: Shut Down", 2, this);
    m_powerRestart = new Power("Power: Restart", 3, this);
    m_powerScreenSaver = new Power("Power: Screen Saver", 4, this);
    m_appLaunchAsUserLister = new AppLister("App: Run", false, this);
    m_appLaunchAsSystemLister = new AppLister("App: Run as system", true, this);
    m_appNew = new AppRegister("App: New", this);
    m_procList = new ProcLister("Process: Kill", this);
    m_winList = new winLister("Window: Search", this);

    int index = 0;
    m_stdModel->setItem(index++, m_powerSleep);
    m_stdModel->setItem(index++, m_powerHibernate);
    m_stdModel->setItem(index++, m_powerShutDown);
    m_stdModel->setItem(index++, m_powerRestart);
    m_stdModel->setItem(index++, m_powerScreenSaver);
    m_stdModel->setItem(index++, m_appLaunchAsUserLister);
    m_stdModel->setItem(index++, m_appLaunchAsSystemLister);
    m_stdModel->setItem(index++, m_appNew);
    m_stdModel->setItem(index++, m_procList);
    m_stdModel->setItem(index++, m_winList);
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

bool CmdPalette::activate()
{
    if (isVisible())
        return false;

    show();
    activateWindow();

    return true;
}

void CmdPalette::activateWindowSearch()
{
    if (activate()) {
        m_currCmdItem = m_winList;
        cmdExecute();
    }
}

void CmdPalette::deactivate()
{
    // Reset item to null
    m_currCmdItem = nullptr;

    // Update list view to standard model
    updateCmdView(m_stdModel);

    // Hide palette
    hide();
}

void CmdPalette::cmdActivate(const QModelIndex &index)
{
    m_currCmdItem = (CmdItem *)((QStandardItemModel *)m_proxyModel->sourceModel())->itemFromIndex(m_proxyModel->mapToSource(index));
    cmdExecute();
}

void CmdPalette::autoUpdate()
{
    if (m_currCmdItem) {
        m_currCmdItem->autoUpdate();

        // Update list view
        ui->listView->update();
    }
}

void CmdPalette::textChanged()
{
    QString &text = ui->lineEdit->text();
    QString search;

    for (int i = 0; i < text.length(); ++i) {
       if (i == 0)
           // First char must be border
           search += QString("(?:.*\\b)(%1)").arg(text.at(i));
       else
           // char can be border or next to previous char
           search += QString("(?:.*\\b)?(%1)").arg(text.at(i));
    }
    search += "(?:.*)";

    // Set filter

    QRegExp regExp(search, Qt::CaseInsensitive, QRegExp::RegExp);
    m_proxyModel->setFilterRegExp(regExp);

    // Set highlight and sort by keyword
    emit searchChanged(search);

    // Update to make highlight effect
    ui->listView->update();

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
    if (event->nativeVirtualKey() == VK_RETURN) {
        QApplication::postEvent(ui->listView, new QKeyEvent(event->type(), event->key(), event->modifiers()));
    } else if (event->nativeVirtualKey() == VK_DOWN) {
        QApplication::postEvent(ui->listView, new QKeyEvent(event->type(), event->key(), event->modifiers()));
    } else if (event->nativeVirtualKey() == VK_UP) {
        QApplication::postEvent(ui->listView, new QKeyEvent(event->type(), event->key(), event->modifiers()));
    }
}

void CmdPalette::cmdExecute()
{
    m_currCmdItem->exec();
    if (m_currCmdItem->resultModel()->rowCount() > 0) {
        updateCmdView(m_currCmdItem->resultModel());
    } else {
        deactivate();
    }
}

void CmdPalette::updateCmdView(QStandardItemModel *model)
{
    // Timer control

    static CmdItem *prevCmdItem = nullptr;

    if (m_currCmdItem == nullptr) {
        if (m_autoUpdateTimer.isActive())
            m_autoUpdateTimer.stop();
    } else {
        if (m_currCmdItem != prevCmdItem) {
            if (m_autoUpdateTimer.isActive())
                m_autoUpdateTimer.stop();
            if (m_currCmdItem->isAutoUpdate())
                m_autoUpdateTimer.start(UPDATE_INTERVAL);
        }
    }

    prevCmdItem = m_currCmdItem;

    // Clear text.
    ui->lineEdit->clear();

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

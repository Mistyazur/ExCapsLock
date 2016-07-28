#include "SingleApplication.h"
#include <QtNetwork/QLocalSocket>
#include <QFileInfo>

#define TIME_OUT	(500)    // 500ms

SingleApplication::SingleApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , w(NULL)
    , m_isRunning(false)
    , m_localServer(NULL)
{

    m_serverName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();

    initLocalConnection();
}


bool SingleApplication::isRunning()
{
    return m_isRunning;
}

void SingleApplication::newLocalConnection()
{
    QLocalSocket *socket = m_localServer->nextPendingConnection();
    if(socket) {
        socket->waitForReadyRead(2*TIME_OUT);
        delete socket;

        activateWindow();
    }
}

void SingleApplication::initLocalConnection()
{
    m_isRunning = false;

    // Try to connect

    QLocalSocket socket;
    socket.connectToServer(m_serverName);
    if(socket.waitForConnected(TIME_OUT))
    {
        fprintf(stderr, "%s already running.\n",
                m_serverName.toLocal8Bit().constData());
        m_isRunning = true;

        return;
    }

    // Can't connect to server, Create a server.

    newLocalServer();
}

void SingleApplication::newLocalServer()
{
    m_localServer = new QLocalServer(this);
    connect(m_localServer, &QLocalServer::newConnection, this, &SingleApplication::newLocalConnection);
    if(!m_localServer->listen(m_serverName))
    {
        if(m_localServer->serverError() == QAbstractSocket::AddressInUseError) {
            QLocalServer::removeServer(m_serverName);
            m_localServer->listen(m_serverName);
        }
    }
}

void SingleApplication::activateWindow()
{
    if(w) {
        w->show();
        w->raise();
        w->activateWindow();
    }
}

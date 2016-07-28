#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QObject>
#include <QApplication>
#include <QtNetwork/QLocalServer>
#include <QWidget>

class SingleApplication : public QApplication
{
    Q_OBJECT
public:
    SingleApplication(int &argc, char **argv);
    bool isRunning();

    QWidget *w;
private slots:
    void newLocalConnection();
private:
    void initLocalConnection();
    void newLocalServer();
    void activateWindow();

    bool m_isRunning;
    QString m_serverName;
    QLocalServer *m_localServer;
};

#endif // SINGLEAPPLICATION_H

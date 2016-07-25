#include "keyboardhook.h"
#include <QApplication>
#include <QSettings>
#include <QProcess>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <Windows.h>

#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"

void setAutoStartEnabled(bool enabled)
{
    QString appName = QApplication::applicationName();
    QSettings *settings = new QSettings(REG_RUN, QSettings::NativeFormat);
    if(enabled)
    {
        QString application_path = QApplication::applicationFilePath();
        settings->setValue(appName, application_path.replace("/", "\\"));
    }
    else
    {
        settings->remove(appName);
    }
    delete settings;
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QFile *logFile = nullptr;
    if (logFile == nullptr)
    {
        WCHAR szAppFileName[MAX_PATH];
        GetModuleFileName(NULL, szAppFileName, MAX_PATH);

        QString &appFileDirName = QString::fromWCharArray(szAppFileName).replace("\\", "/");
        QString &appFileDir = appFileDirName.left(appFileDirName.lastIndexOf("/"));
        QString logName = appFileDir + "/"
                + QDateTime::currentDateTime().toString("MMddyyyy_hhmmss")
                + ".log";
        logFile = new QFile(logName);
        if (!logFile->open(QFile::ReadWrite))
        {
            delete logFile;
            return;
        }
    }

    QTextStream ts(logFile);
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        ts<<"[Debug]: "<<localMsg.constData()<<"\r\n";
        break;
    case QtInfoMsg:
        ts<<"[Info]: "<<localMsg.constData()<<"\r\n";
        break;
    case QtWarningMsg:
        ts<<"[Warning]: "<<localMsg.constData()<<"\r\n";
        break;
    case QtCriticalMsg:
        ts<<"[Critical]: "<<localMsg.constData()<<"\r\n";
        break;
    case QtFatalMsg:
        ts<<"[Fatal]: "<<localMsg.constData()<<"\r\n";
        abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

    QApplication a(argc, argv);

    QStringList &argl = a.arguments();
    if (argl.count() >= 2)
    {
       const QString &arg = argl.at(1);
       if (arg.compare("/ir", Qt::CaseInsensitive) == 0)
           setAutoStartEnabled(true);
       else if (arg.compare("/ur", Qt::CaseInsensitive) == 0)
           setAutoStartEnabled(false);
       else if (arg.compare("/is", Qt::CaseInsensitive) == 0)
           QProcess::startDetached(QString("SCHTASKS /CREATE /TN \"ExCapsLock\" /TR \"%1\" /SC ONLOGON /RL Highest /F").arg(QApplication::applicationFilePath()));
       else if (arg.compare("/us", Qt::CaseInsensitive) == 0)
           QProcess::startDetached("SCHTASKS /DELETE /TN \"ExCapsLock\" /F");

       return 0;
    }

    KeyboardHook kh;

    return a.exec();
}

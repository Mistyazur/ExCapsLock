#include "keyboardhook.h"
#include "singleapplication.h"
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
    if (logFile == nullptr) {
        WCHAR szAppFileName[MAX_PATH];
        GetModuleFileName(NULL, szAppFileName, MAX_PATH);

        QString &appFileDirName = QString::fromWCharArray(szAppFileName).replace("\\", "/");
        QString &appFileDir = appFileDirName.left(appFileDirName.lastIndexOf("/"));
        QString logName = appFileDir + "/"
                + QCoreApplication::applicationName()
                + QDateTime::currentDateTime().toString("_MMddyyyy-hhmmss")
                + ".log";
        logFile = new QFile(logName);
        if (!logFile->open(QFile::ReadWrite)) {
            delete logFile;
            return;
        }
    }

    QTextStream ts(logFile);
    QByteArray &localMsg = msg.toLocal8Bit();
    QString &logTime = QDateTime::currentDateTime().toString("[MM/dd hh:mm:ss.zzz]");
    switch (type) {
    case QtDebugMsg:
        ts<<"[D]"<<logTime<<" "<<localMsg.constData()<<"\r\n";
        break;
    case QtInfoMsg:
        ts<<"[I]: "<<logTime<<" "<<localMsg.constData()<<"\r\n";
        break;
    case QtWarningMsg:
        ts<<"[W]: "<<logTime<<" "<<localMsg.constData()<<"\r\n";
        break;
    case QtCriticalMsg:
        ts<<"[C]: "<<logTime<<" "<<localMsg.constData()<<"\r\n";
        break;
    case QtFatalMsg:
        ts<<"[F]: "<<logTime<<" "<<localMsg.constData()<<"\r\n";
        abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

    SingleApplication a(argc, argv);

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
    }
    else
    {
        if (!a.isRunning())
        {
            KeyboardHook kh;
            return a.exec();
        }
    }

    return 0;
}

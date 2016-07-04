#include "keyboardhook.h"
#include "appdetector.h"
#include <QApplication>
#include <QSettings>
#include <QProcess>

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

int main(int argc, char *argv[])
{
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
    AppDetector ad;

    return a.exec();
}

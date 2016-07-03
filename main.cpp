#include "keyboardhook.h"
#include "appdetector.h"
#include <QApplication>
#include <QSettings>

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
       if (arg.compare("/i", Qt::CaseInsensitive) == 0)
           setAutoStartEnabled(true);
       else if (arg.compare("/u", Qt::CaseInsensitive) == 0)
           setAutoStartEnabled(false);

       return 0;
    }

    KeyboardHook kh;
    AppDetector ad;

    return a.exec();
}

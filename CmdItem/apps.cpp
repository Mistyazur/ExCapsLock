#include "apps.h"
#include "applauncher.h"
#include <QApplication>
#include <QSettings>

Apps::Apps(const QString &text)  :
    CmdItem(text)
{
}

int Apps::exec()
{
    QSettings settings(qApp->applicationDirPath() + "/ExCapsLock.ini", QSettings::IniFormat);
    QStringList processList;
    for (int i = 0; ; ++i)
    {
        QString &key = QString("Apps/path%1").arg(i);
        QString &process = settings.value(key, "").toString();
        if (process.isEmpty())
            break;
        else
        {
//            processList += process;
            m_resModel->setItem(i, new AppLauncher(process));
        }
    }

    return 0;
}

#include "processretrieval.h"
#include "wmi.h"
#include <QApplication>
#include <QSettings>
#include <QFile>

ProcessRetrieval::ProcessRetrieval(QObject *parent) :
    QObject(parent)
{
    QSettings settings(qApp->applicationDirPath() + "/ExCapsLock.ini", QSettings::IniFormat);
    for (int i = 0; ; ++i)
    {
        QString &key = QString("Apps/path%1").arg(i);
        QString &process = settings.value(key, "").toString();
        if (process.isEmpty())
            break;
        else
        {
            // Check if this process exist or not
            QFile file(process);
            if (file.exists())
                m_processList += process;
        }
    }

    startTimer(10000);
}

void ProcessRetrieval::timerEvent(QTimerEvent *)
{
    QSettings settings(qApp->applicationDirPath() + "/ExCapsLock.ini", QSettings::IniFormat);
    QVariantList vlProcess;
    WmiInstance.get("SELECT * FROM Win32_Process", "ExecutablePath", vlProcess);
    foreach (QVariant v, vlProcess)
    {
        if (v.isValid())
        {
            QString &process = v.toString();
            process.replace("\\", "/");
            if (!m_processList.contains(process))
                m_processList += process;
        }
    }
    settings.remove("Apps");
    for (int i = 0; i < m_processList.count(); ++i)
    {
        settings.setValue(QString("Apps/path%1").arg(i), m_processList.at(i));
    }
}

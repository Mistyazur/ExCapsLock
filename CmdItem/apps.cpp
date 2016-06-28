#include "apps.h"
#include "applauncher.h"
#include "cmditemdelegate.h"
#include <QApplication>
#include <QSettings>

Apps::Apps(const QString &text)  :
    CmdItem(text)
{
}

bool Apps::exec()
{
    QSettings settings(qApp->applicationDirPath() + "/ExCapsLock.ini", QSettings::IniFormat);
    int index = 0;
    for (int i = 0; ; ++i)
    {
        QString &key = QString("Apps/path%1").arg(i);
        QString &process = settings.value(key, "").toString();
        if (process.isEmpty())
            break;
        else
        {
            QRegExp rx("/(.+)\\.exe", Qt::CaseInsensitive, QRegExp::RegExp);
            if (rx.lastIndexIn(process) != -1)
            {
                m_resModel->setItem(index, new AppLauncher(rx.cap(1)));
                m_resModel->setData(m_resModel->index(index, 0), process, CMD_PARAGRAPH);
                ++index;
            }
        }
    }

    return true;
}

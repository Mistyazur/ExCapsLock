#include "apps.h"
#include "applauncher.h"
#include "cmditemdelegate.h"
#include <QApplication>
#include <QSettings>

Apps::Apps(const QString &text)  :
    CmdItem(text)
{
}

int Apps::exec()
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
            QRegExp rx("/(\\w+)\\.exe", Qt::CaseInsensitive, QRegExp::RegExp);
            if (rx.indexIn(process) != -1)
            {
                m_resModel->setItem(i, new AppLauncher(S_CAPTION(rx.cap(1)) + S_PARAGRAPH(process)));
            }
            else
                m_resModel->setItem(i, new AppLauncher(S_CAPTION(process)));
        }
    }

    return 0;
}

#include "apps.h"
#include "appcapture.h"
#include "applauncher.h"
#include "cmditemdelegate.h"

extern AppCapture *g_appCapture;

Apps::Apps(const QString &text)  :
    CmdItem(text)
{
}

bool Apps::exec()
{
    QStringList processList = g_appCapture->processList();
    for (int index = 0; index < processList.count(); ++index)
    {
        const QString &process = processList.at(index);
        if (!process.isEmpty())
        {
            QRegExp rx("/(.+)\\.exe", Qt::CaseInsensitive, QRegExp::RegExp);
            if (rx.lastIndexIn(process) != -1)
            {
                m_resModel->setItem(index, new AppLauncher(rx.cap(1)));
                m_resModel->setData(m_resModel->index(index, 0), process, CMD_PARAGRAPH);
            }
        }
    }

    return true;
}

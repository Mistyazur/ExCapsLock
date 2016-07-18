#include "applister.h"
#include "applauncher.h"
#include "cmditemdelegate.h"
#include <QDebug>

AppLister::AppLister(const QString &text, QObject *parent) :
    CmdItem(text, parent)
{
}

bool AppLister::exec()
{
    for (int index = 0; index < m_appList.count(); ++index)
    {
        const QString &app = m_appList.at(index);
        if (!app.isEmpty())
        {
            QRegExp rx("/(.+)\\.exe", Qt::CaseInsensitive, QRegExp::RegExp);
            if (rx.lastIndexIn(app) != -1)
                m_resultModel->setItem(index, new AppLauncher(rx.cap(1), app));
        }
    }

    return true;
}

void AppLister::updateApps(const QStringList &apps)
{
    m_appList = apps;
}

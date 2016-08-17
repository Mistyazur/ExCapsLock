#include "config.h"
#include "applister.h"
#include "applauncher.h"
#include "jsettings.h"
#include "cmditemdelegate.h"
#include <QDebug>

AppLister::AppLister(const QString &text, QObject *parent) :
    CmdItem(text, parent)
{
}

bool AppLister::exec()
{
    JSettings settings(USER_SETTINGS);
    QStringList appList = settings.value(KEY_APP_PATH).toStringList();

    for (int index = 0; index < appList.count(); ++index)
    {
        const QString &app = appList.at(index);
        if (!app.isEmpty())
        {
            QRegExp rx("/(.+)\\.exe", Qt::CaseInsensitive, QRegExp::RegExp);
            if (rx.lastIndexIn(app) != -1)
                m_resultModel->setItem(index, new AppLauncher(rx.cap(1), app));
        }
    }

    return true;
}

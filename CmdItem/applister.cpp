#include "config.h"
#include "applister.h"
#include "applauncher.h"
#include "jsettings.h"
#include "cmditemdelegate.h"

AppLister::AppLister(const QString &text, bool runAsSystem, QObject *parent) :
    CmdItem(text, parent),
    m_runAsSystem(runAsSystem)
{
}

bool AppLister::exec()
{
    JSettings settings(USER_SETTINGS);
    QStringList appList = settings.value(KEY_APP_PATH).toStringList();

    for (int index = 0; index < appList.count(); ++index) {
        const QString &appPath = appList.at(index);
        if (!appPath.isEmpty()) {
            QRegExp rx("/(.+)\\.exe", Qt::CaseInsensitive, QRegExp::RegExp);
            if (rx.lastIndexIn(appPath) != -1) {
                m_resultModel->setItem(index, new AppLauncher(rx.cap(1), appPath, m_runAsSystem, this));
            }
        }
    }

    return true;
}

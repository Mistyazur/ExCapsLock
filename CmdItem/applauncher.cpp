#include "applauncher.h"
#include <QProcess>

AppLauncher::AppLauncher(const QString &text)  :
    CmdItem(text)
{

}

int AppLauncher::exec()
{
    QProcess::startDetached(text());
    return 0;
}

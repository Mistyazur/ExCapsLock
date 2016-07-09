#include "applauncher.h"
#include "cmditemdelegate.h"
#include <QProcess>
#include <Windows.h>


AppLauncher::AppLauncher(const QString &text, QObject *parent)  :
    CmdItem(text, parent)
{

}

bool AppLauncher::exec()
{
    PVOID OldValue = NULL;
    if( Wow64DisableWow64FsRedirection(&OldValue) )
    {
        QString &path = data(CMD_PARAGRAPH).toString();
        path = "\"" + path + "\"";
        if (!path.isEmpty())
            return QProcess::startDetached(path);

        Wow64RevertWow64FsRedirection(OldValue);
    }

    return false;
}

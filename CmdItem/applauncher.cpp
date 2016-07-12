#include "applauncher.h"
#include "cmditemdelegate.h"
#include <QProcess>
#include <Windows.h>

AppLauncher::AppLauncher(const QString &text, const QString &path, QObject *parent) :
    CmdItem(text, parent), m_path(path)
{

}

bool AppLauncher::exec()
{
    PVOID OldValue = NULL;
    if( Wow64DisableWow64FsRedirection(&OldValue) )
    {
        QString path = "\"" + m_path + "\"";
        if (!path.isEmpty())
            return QProcess::startDetached(path);

        Wow64RevertWow64FsRedirection(OldValue);
    }

    return false;
}

const QString AppLauncher::html(const QString &searchKeyword)
{
     return QString("<h6>%1</h6><p>%2</p>")
             .arg(highlight(text(), searchKeyword))
             .arg(m_path);
}

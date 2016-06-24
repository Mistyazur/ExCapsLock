#include "systemcmd.h"
#include <QProcess>

SystemCmd::SystemCmd(const QString &text, const QString &cmd) :
    CmdItem(text),
    m_cmd(cmd)
{
}

int SystemCmd::exec()
{
    return (QProcess::startDetached(m_cmd) ? CMD_SUCCEED : CMD_FAILED);
}

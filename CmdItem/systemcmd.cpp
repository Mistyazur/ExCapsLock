#include "systemcmd.h"
#include <QProcess>

SystemCmd::SystemCmd(const QString &text, const QString &cmd, QObject *parent) :
    CmdItem(text, parent),
    m_cmd(cmd)
{
}

bool SystemCmd::exec()
{
    return QProcess::startDetached(m_cmd);
}

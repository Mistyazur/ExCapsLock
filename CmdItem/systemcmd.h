#ifndef SYSTEMCMD_H
#define SYSTEMCMD_H

#include "cmditem.h"

class SystemCmd : public CmdItem
{
public:
    SystemCmd(const QString &text, const QString &cmd);
    int exec();
private:
    QString m_cmd;
};

#endif // SYSTEMCMD_H

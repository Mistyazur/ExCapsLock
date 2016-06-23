#ifndef POWER_H
#define POWER_H

#include "cmditem.h"

class SystemCmd : public CmdItem
{
public:
    SystemCmd(const QString &text, const QString &cmd);
    bool exec();
private:
    QString m_cmd;
};

#endif // POWER_H

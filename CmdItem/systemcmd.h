#ifndef SYSTEMCMD_H
#define SYSTEMCMD_H

#include "cmditem.h"

class SystemCmd : public CmdItem
{
    Q_OBJECT
public:
    SystemCmd(const QString &text, const QString &cmd, QObject *parent = 0);
    bool exec();
private:
    QString m_cmd;
};

#endif // SYSTEMCMD_H

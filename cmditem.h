#ifndef CMDS_H
#define CMDS_H

#include <QStandardItem>

class CmdItem : public QStandardItem
{
public:
    CmdItem(const QString &text);
    virtual bool exec() = 0;
};

#endif // CMDS_H

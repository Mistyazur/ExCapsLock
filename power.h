#ifndef POWER_H
#define POWER_H

#include "cmditem.h"

class Power : public CmdItem
{
public:
    Power(const QString &text, int type);
    int exec();
private:
    int m_type;
};

#endif // POWER_H

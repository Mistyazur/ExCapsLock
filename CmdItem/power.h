#ifndef POWER_H
#define POWER_H

#include "cmditem.h"

class Power : public CmdItem
{
    Q_OBJECT
public:
    Power(const QString &text, int type, QObject *parent = 0);
    bool exec();
private:
    int m_type;
};

#endif // POWER_H

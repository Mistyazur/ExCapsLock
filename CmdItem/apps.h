#ifndef APPS_H
#define APPS_H

#include "cmditem.h"

class Apps : public CmdItem
{
public:
    Apps(const QString &text);
    bool exec();
};

#endif // APPS_H

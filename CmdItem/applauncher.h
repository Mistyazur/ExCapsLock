#ifndef APPLAUNCHER_H
#define APPLAUNCHER_H

#include "cmditem.h"

class AppLauncher : public CmdItem
{
public:
    AppLauncher(const QString &text);
    int exec();
};

#endif // APPLAUNCHER_H

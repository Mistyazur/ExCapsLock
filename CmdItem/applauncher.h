#ifndef APPLAUNCHER_H
#define APPLAUNCHER_H

#include "cmditem.h"

class AppLauncher : public CmdItem
{
    Q_OBJECT
public:
    AppLauncher(const QString &text, QObject *parent = 0);
    bool exec();
};

#endif // APPLAUNCHER_H

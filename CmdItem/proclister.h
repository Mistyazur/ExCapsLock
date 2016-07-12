#ifndef PROCLISTER_H
#define PROCLISTER_H

#include "cmditem.h"

class ProcLister : public CmdItem
{
    Q_OBJECT
public:
    ProcLister(const QString &text, QObject *parent = 0);
    bool exec();
};

#endif // PROCLISTER_H

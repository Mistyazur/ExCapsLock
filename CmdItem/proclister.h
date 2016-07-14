#ifndef PROCLISTER_H
#define PROCLISTER_H

#include "cmditem.h"
#include <Windows.h>

class ProcLister : public CmdItem
{
    Q_OBJECT
public:
    ProcLister(const QString &text, QObject *parent = 0);
    bool exec();
protected:
    void timerEvent(QTimerEvent *);
private:

    LONG cpuUsage(HANDLE hProcess, LONG pid, LONGLONG timeDelta);
    LONGLONG memoryUsage(HANDLE hProcess);
};

#endif // PROCLISTER_H

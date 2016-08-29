#ifndef PROCLISTER_H
#define PROCLISTER_H

#include "cmditem.h"
#include <Windows.h>
#include <QTimer>

class ProcLister : public CmdItem
{
    Q_OBJECT
public:
    ProcLister(const QString &text, QObject *parent = 0);
    bool exec();
    void autoUpdate();
    bool isAutoUpdate();
public slots:
private:
    QString		getProcessUser(HANDLE hProcess);
    LONG 		getProcessCpu(HANDLE hProcess, LONGLONG timeDelta);
    LONGLONG 	getProcessMem(HANDLE hProcess);
};

#endif // PROCLISTER_H

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
    QString user(HANDLE hProcess);
    LONG cpuUsage(HANDLE hProcess, LONG pid, LONGLONG timeDelta);
    LONGLONG memoryUsage(HANDLE hProcess);
};

#endif // PROCLISTER_H

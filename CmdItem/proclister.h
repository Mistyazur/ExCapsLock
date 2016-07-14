#ifndef PROCLISTER_H
#define PROCLISTER_H

#include "cmditem.h"
#include <Windows.h>
#include <QHash>

class ProcessCpu : public QObject
{
    Q_OBJECT
public:
    ProcessCpu(int processorCount, QObject *parent = 0) :
        QObject(parent)
    {
        m_processorCount = processorCount;
        m_prevCpuTime = 0;
    }

    inline int cpu(HANDLE hProcess, LONGLONG timeDelta)
    {
        int nCpu = -1;

        // Get cpu time of process.

        FILETIME creationTime;
        FILETIME exitTime;
        FILETIME kernelTime;
        FILETIME userTime;
        LONGLONG cpuTime;
        LONGLONG cpuTimeDelta;

        if (!GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime)) {
            // We don't assert here because in some cases (such as in the Task Manager)
            // we may call this function on a process that has just exited but we have
            // not yet received the notification.
            return -1;
        }

        cpuTime = (((PLARGE_INTEGER)(&kernelTime))->QuadPart + ((PLARGE_INTEGER)(&userTime))->QuadPart) / m_processorCount;

        if (m_prevCpuTime == 0) {
            nCpu = -1;
        } else {
            cpuTimeDelta = cpuTime - m_prevCpuTime;
            nCpu = ((cpuTimeDelta * 100 + timeDelta / 2) / timeDelta);
        }

        m_prevCpuTime = cpuTime;

        return nCpu;
    }

private:
    int m_processorCount;
    LONGLONG m_prevCpuTime;
};

class ProcLister : public CmdItem
{
    Q_OBJECT
public:
    ProcLister(const QString &text, QObject *parent = 0);
    bool exec();
protected:
    void timerEvent(QTimerEvent *);
private:
    QHash<int, ProcessCpu*> m_processCpuHash;
};

#endif // PROCLISTER_H

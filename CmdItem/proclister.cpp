#include "proclister.h"
#include "prockiller.h"
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <QProcess>
#include <QHash>
#include <QDebug>



ProcLister::ProcLister(const QString &text, QObject *parent) :
    CmdItem(text, parent)
{

}

bool ProcLister::exec()
{
    timerEvent(nullptr);
    startTimer(1000);
    return true;
}

void ProcLister::timerEvent(QTimerEvent *)
{
    // Get time past since last time

    static LONGLONG prevTime = 0;
    LONGLONG currTime;
    LONGLONG timeDelta;
    FILETIME tNow;

    ::GetSystemTimeAsFileTime(&tNow);
    currTime = ((PLARGE_INTEGER)(&tNow))->QuadPart;

    if (prevTime == 0)
        timeDelta = 0;
    else
        timeDelta = currTime - prevTime;

    prevTime = currTime;

    // Take a snapshot of all processes in the system.

    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32 = { };
    pe32.dwSize = sizeof(PROCESSENTRY32);

    hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hProcessSnap)
        return;

    if (::Process32First(hProcessSnap, &pe32))
    {
        int index = 0;
        do {

            // Open process to query infomation.

            HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                            FALSE, pe32.th32ProcessID);
            if (NULL != hProcess) {

                QString &caption = QString::fromWCharArray(pe32.szExeFile);
                QStringList params;
                params += QString::number(pe32.th32ProcessID);
                params += QString::number(cpuUsage(hProcess, pe32.th32ProcessID, timeDelta));
                params += QString::number(memoryUsage(hProcess)/1024) + " K";
                m_resModel->setItem(index++, new ProcKiller(caption, params));

                ::CloseHandle(hProcess);
            }

        } while (::Process32Next(hProcessSnap, &pe32));
    }

    ::CloseHandle(hProcessSnap);
}

LONG ProcLister::cpuUsage(HANDLE hProcess, LONG pid, LONGLONG timeDelta)
{
    LONG nCpu = -1;

    // Get count of cpu.

    static int nProcessor = -1;

    if(nProcessor == -1) {
        SYSTEM_INFO info;
        ::GetSystemInfo(&info);
        nProcessor = info.dwNumberOfProcessors;
    }

    // Get previous cpu time of the specified process

    static QHash<LONG, LONGLONG> prevCpuTimeHash = QHash<LONG, LONGLONG>();
    LONGLONG prevCpuTime;

    if (prevCpuTimeHash.contains(pid)) {
        prevCpuTime = prevCpuTimeHash.take(pid);
    } else {
        prevCpuTime = 0;
    }

    // Get cpu time of process.

    FILETIME creationTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    FILETIME userTime;
    LONGLONG cpuTime;
    LONGLONG cpuTimeDelta;

    if (!::GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime)) {
        // We don't assert here because in some cases (such as in the Task Manager)
        // we may call this function on a process that has just exited but we have
        // not yet received the notification.
        return -1;
    }

    cpuTime = (((PLARGE_INTEGER)(&kernelTime))->QuadPart + ((PLARGE_INTEGER)(&userTime))->QuadPart) / nProcessor;

    if (prevCpuTime == 0) {
        nCpu = -1;
    } else {
        cpuTimeDelta = cpuTime - prevCpuTime;
        nCpu = ((cpuTimeDelta * 100 + timeDelta / 2) / timeDelta);
    }

    // Save as previous cpu time

    prevCpuTime = cpuTime;
    prevCpuTimeHash.insert(pid, prevCpuTime);

    return nCpu;
}

LONGLONG ProcLister::memoryUsage(HANDLE hProcess)
{
    LONGLONG workSetPrivate = 0;
    PBYTE pByte = NULL;

    PERFORMANCE_INFORMATION performanceInfo = {};
    if(!::GetPerformanceInfo(&performanceInfo, sizeof(performanceInfo)))
        return 0;

    PSAPI_WORKING_SET_INFORMATION workSetInfo = {};
    PSAPI_WORKING_SET_BLOCK *pWorkSetBlock = workSetInfo.WorkingSetInfo;
    if(!::QueryWorkingSet(hProcess, &workSetInfo, sizeof(PSAPI_WORKING_SET_INFORMATION))) {
        if(GetLastError() == ERROR_BAD_LENGTH) {
            DWORD requiredSize = sizeof(workSetInfo.NumberOfEntries) + workSetInfo.NumberOfEntries*sizeof(PSAPI_WORKING_SET_BLOCK);
            pByte = new BYTE[requiredSize]{0};
            pWorkSetBlock = (PSAPI_WORKING_SET_BLOCK *)(pByte + sizeof(workSetInfo.NumberOfEntries));
            if(!::QueryWorkingSet(hProcess, pByte, requiredSize))
            {
                goto CLEAN;
            }
        }
        else
        {
            goto CLEAN;
        }
    }

    for (ULONG_PTR i = 0; i < workSetInfo.NumberOfEntries; ++i)
    {
        if(!pWorkSetBlock[i].Shared)
            workSetPrivate += performanceInfo.PageSize;
    }

    CLEAN:
    if (pByte)
        delete[] pByte;
    CloseHandle(hProcess);

    return workSetPrivate;
}

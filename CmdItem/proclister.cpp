#include "proclister.h"
#include "prockiller.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <QWidget>
#include <QHash>
#include <QDebug>




ProcLister::ProcLister(const QString &text, QObject *parent) :
    CmdItem(text, parent)
{
}

bool ProcLister::exec()
{ 
    // Update immediately
    autoUpdate();

    return true;
}

bool ProcLister::isAutoUpdate()
{
    return true;
}

void ProcLister::autoUpdate()
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
                if (caption.endsWith(".exe", Qt::CaseInsensitive))
                    caption.chop(4);

                QStringList infoList;
                infoList += QString::number(pe32.th32ProcessID);
                infoList += user(hProcess);
                infoList += QString::number(cpuUsage(hProcess, pe32.th32ProcessID, timeDelta));
                infoList += QString::number(memoryUsage(hProcess)/1024) + " K";

                if (m_resultModel->item(index))
                {
                    m_resultModel->item(index)->setText(caption);
                    ((ProcKiller *)m_resultModel->item(index))->setInfo(infoList);
                }
                else
                {
                    m_resultModel ->setItem(index, new ProcKiller(caption));
                    ((ProcKiller *)m_resultModel->item(index))->setInfo(infoList);
                }
                ++index;

                ::CloseHandle(hProcess);
            }

        } while (::Process32Next(hProcessSnap, &pe32));
    }

    ::CloseHandle(hProcessSnap);
}

QString ProcLister::user(HANDLE hProcess)
{
    QString user;
    DWORD dwSize = 0;

    // Open process token

    HANDLE hToken = NULL;

    if(!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
        return user;
    }

    // Get infomation of token

    if (!GetTokenInformation(hToken, TokenUser, nullptr, 0, &dwSize)) {
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
            goto Cleanup;
    }

    PTOKEN_USER ptu = (PTOKEN_USER)LocalAlloc(LPTR, dwSize);
    if (ptu == NULL)
        goto Cleanup;

    if (!GetTokenInformation(hToken, TokenUser, (LPVOID)ptu, dwSize, &dwSize))
        goto Cleanup;

    // Look up account information

    SID_NAME_USE SidType;
    wchar_t lpName[MAX_PATH];
    wchar_t lpDomain[MAX_PATH];

    dwSize = MAX_PATH;
    if(!LookupAccountSid(NULL , ptu->User.Sid, lpName, &dwSize, lpDomain, &dwSize, &SidType))
        goto Cleanup;

    user = QString::fromWCharArray(lpName);
    user = user.split("/").last();

Cleanup:
    if (hToken)
        CloseHandle(hToken);

    if (ptu)
        LocalFree((HLOCAL)ptu);

    return user;
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
            SIZE_T requiredSize = sizeof(workSetInfo.NumberOfEntries) + workSetInfo.NumberOfEntries*sizeof(PSAPI_WORKING_SET_BLOCK);
            pByte = new BYTE[requiredSize]{0};
            pWorkSetBlock = (PSAPI_WORKING_SET_BLOCK *)(pByte + sizeof(workSetInfo.NumberOfEntries));
            if(!::QueryWorkingSet(hProcess, pByte, requiredSize))
            {
                goto Cleanup;
            }
        }
        else
        {
            goto Cleanup;
        }
    }

    for (ULONG_PTR i = 0; i < workSetInfo.NumberOfEntries; ++i)
    {
        if(!pWorkSetBlock[i].Shared)
            workSetPrivate += performanceInfo.PageSize;
    }

Cleanup:
    if (pByte)
        delete[] pByte;
    CloseHandle(hProcess);

    return workSetPrivate;
}

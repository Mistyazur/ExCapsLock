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

    QHash<int ,QString> processHash;
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
            processHash.insert(pe32.th32ProcessID, QString::fromWCharArray(pe32.szExeFile));
        } while (::Process32Next(hProcessSnap, &pe32));
    }

    ::CloseHandle(hProcessSnap);

    // Update process information

    ProcKiller *pProcKiller = NULL;
    HANDLE hProcess = NULL;
    DWORD dwPid;
    DWORD dwExitCode;

    for (int i = 0 ; i < m_resultModel->rowCount(); ++i) {
        pProcKiller = ((ProcKiller *)m_resultModel->item(i));
        if (pProcKiller) {
            dwPid = pProcKiller->pid();
            hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);
            if (NULL != hProcess) {
                if (::GetExitCodeProcess(hProcess, &dwExitCode)) {
                    if (dwExitCode == STILL_ACTIVE) {
                        pProcKiller->setCpu(getProcessCpu(hProcess, timeDelta));
//                        pProcKiller->setMem(getProcessMem(hProcess));
                        processHash.remove(dwPid);
                    } else {
                        m_resultModel->removeRow(i--);
                    }
                }

                ::CloseHandle(hProcess);
            } else {
                m_resultModel->removeRow(i--);
            }
        }
    }

    // Add new process infomation

    QHash<int, QString>::const_iterator it = processHash.constBegin();
    while (it != processHash.constEnd()) {
        hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, it.key());
        if (NULL != hProcess) {
            pProcKiller = new ProcKiller(it.value().left(it.value().lastIndexOf(".")), this);
            pProcKiller->setName(it.value());
            pProcKiller->setUser(getProcessUser(hProcess));
            pProcKiller->setPid(it.key());
            pProcKiller->setCpu(getProcessCpu(hProcess, timeDelta));
            pProcKiller->setMem(getProcessMem(hProcess));
            m_resultModel->appendRow(pProcKiller);

            ::CloseHandle(hProcess);
        }

        ++it;
    }
}

QString ProcLister::getProcessUser(HANDLE hProcess)
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

LONG ProcLister::getProcessCpu(HANDLE hProcess, LONGLONG timeDelta)
{
    LONG nCpu = -1;
    LONG pid = ::GetProcessId(hProcess);

    if (timeDelta == 0)
        return nCpu;

    // Get count of cpu.

    static int nProcessor = -1;

    if(nProcessor == -1) {
        SYSTEM_INFO info;
        ::GetSystemInfo(&info);
        nProcessor = info.dwNumberOfProcessors;
    }

    // Get previous cpu time of the specified process

    static QHash<LONG, LONGLONG> prevCpuTimeHash;
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
        return nCpu;
    }

    cpuTime = (((PLARGE_INTEGER)(&kernelTime))->QuadPart + ((PLARGE_INTEGER)(&userTime))->QuadPart) / nProcessor;

    if (prevCpuTime != 0) {
        cpuTimeDelta = cpuTime - prevCpuTime;
        nCpu = ((cpuTimeDelta * 100 + timeDelta / 2) / timeDelta);
    }

    // Save as previous cpu time

    prevCpuTime = cpuTime;
    prevCpuTimeHash.insert(pid, prevCpuTime);

    return nCpu;
}

LONGLONG ProcLister::getProcessMem(HANDLE hProcess)
{
    LONGLONG workSetPrivate = 0;
    PBYTE pByte = NULL;

    PERFORMANCE_INFORMATION performanceInfo = {};
    if(!::GetPerformanceInfo(&performanceInfo, sizeof(performanceInfo)))
        return -1;

    PSAPI_WORKING_SET_INFORMATION workSetInfo = {};
    PSAPI_WORKING_SET_BLOCK *pWorkSetBlock = workSetInfo.WorkingSetInfo;
    if(!::QueryWorkingSet(hProcess, &workSetInfo, sizeof(PSAPI_WORKING_SET_INFORMATION))) {
        if(GetLastError() == ERROR_BAD_LENGTH) {
            SIZE_T requiredSize = sizeof(workSetInfo.NumberOfEntries) + workSetInfo.NumberOfEntries*sizeof(PSAPI_WORKING_SET_BLOCK);
            pByte = new BYTE[requiredSize]{0};
            pWorkSetBlock = (PSAPI_WORKING_SET_BLOCK *)(pByte + sizeof(workSetInfo.NumberOfEntries));
            if(!::QueryWorkingSet(hProcess, pByte, requiredSize))
                goto Cleanup;
        } else
            goto Cleanup;
    }

    for (ULONG_PTR i = 0; i < workSetInfo.NumberOfEntries; ++i) {
        if(!pWorkSetBlock[i].Shared)
            workSetPrivate += performanceInfo.PageSize;
    }

Cleanup:
    if (pByte)
        delete[] pByte;

    CloseHandle(hProcess);

    return workSetPrivate;
}

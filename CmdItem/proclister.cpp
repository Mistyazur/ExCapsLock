#include "proclister.h"
#include "prockiller.h"
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <QProcess>
#include <QDebug>

DWORD GetProcessUseMemory(DWORD dwProcID)
{
    HANDLE hProcess = OpenProcess(
                PROCESS_QUERY_INFORMATION |
                PROCESS_VM_READ,
                FALSE, dwProcID );
    PERFORMANCE_INFORMATION performanceInfo;
    memset(&performanceInfo, 0, sizeof(performanceInfo));
    if(!::GetPerformanceInfo(&performanceInfo, sizeof(performanceInfo)))
        return 0;

    DWORD pageSize = performanceInfo.PageSize;

    BOOL bRet = TRUE;
    PSAPI_WORKING_SET_INFORMATION workSetInfo;
    PBYTE pByte = NULL;
    PSAPI_WORKING_SET_BLOCK *pWorkSetBlock = workSetInfo.WorkingSetInfo;
    memset(&workSetInfo, 0, sizeof(workSetInfo));
    // 要求操作进程的权限：PROCESS_QUERY_INFORMATION and PROCESS_VM_READ
    // 第一次调用获取实际缓冲区大小
    bRet = ::QueryWorkingSet(hProcess, &workSetInfo, sizeof(workSetInfo));
    if(!bRet) // 调用失败
    {
        if(GetLastError() == ERROR_BAD_LENGTH) // 需要重新分配缓冲区
        {
            DWORD realSize = sizeof(workSetInfo.NumberOfEntries)
                    + workSetInfo.NumberOfEntries*sizeof(PSAPI_WORKING_SET_BLOCK);
//            try
            {
                pByte = new BYTE[realSize];
                memset(pByte, 0, realSize);
                pWorkSetBlock = (PSAPI_WORKING_SET_BLOCK *)(pByte + sizeof(workSetInfo.NumberOfEntries));
                // 重新获取
                if(!::QueryWorkingSet(hProcess, pByte, realSize))
                {
                    delete[] pByte; // 清理内存
                    CloseHandle(hProcess);
                    return 0;
                }
            }
//            catch(CMemoryException *e) // 分配内存失败
//            {
//                e->Delete();
//                CloseHandle(hProcess);
//                return 0;
//            }

        }
        else // 其它错误，认为获取失败
        {
            CloseHandle(hProcess);
            return 0;
        }
    }
    SIZE_T workSetPrivate = 0;
    for (ULONG_PTR i = 0; i < workSetInfo.NumberOfEntries; ++i)
    {
        if(!pWorkSetBlock[i].Shared) // 如果不是共享页
            workSetPrivate += pageSize;
    }

    if(pByte)
        delete[] pByte;

    CloseHandle(hProcess);

    return workSetPrivate;
}

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
        QHash<int, ProcessCpu *> processCpuHash;

        do {
            HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                          FALSE, pe32.th32ProcessID);
            if (NULL != hProcess) {

                // Get count of cpu.

                static int nProcessor = -1;

                if(nProcessor == -1) {
                    SYSTEM_INFO info;
                    GetSystemInfo(&info);
                    nProcessor = info.dwNumberOfProcessors;
                }

                // Get cpu.
                ProcessCpu *pc = nullptr;
                if (m_processCpuHash.contains(pe32.th32ProcessID)) {
                    pc = m_processCpuHash.take(pe32.th32ProcessID);
                } else {
                    pc = new ProcessCpu(nProcessor);
                }

                processCpuHash.insert(pe32.th32ProcessID, pc);
                if (pe32.th32ProcessID == 26040)
                qDebug()<<QString::fromWCharArray(pe32.szExeFile)<<pc->cpu(hProcess, timeDelta);

                // Release the handle to the process.

                ::CloseHandle(hProcess);
            }

        } while (::Process32Next(hProcessSnap, &pe32));

        // Reset hash
        for(auto i : m_processCpuHash) {
            delete i;
        }
        m_processCpuHash = processCpuHash;
    }

    ::CloseHandle(hProcessSnap);
}

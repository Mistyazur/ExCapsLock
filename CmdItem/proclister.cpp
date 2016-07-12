#include "proclister.h"
#include "prockiller.h"
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

ProcLister::ProcLister(const QString &text, QObject *parent) :
    CmdItem(text, parent)
{

}

bool ProcLister::exec()
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32 = {sizeof(pe32), };

    // Take a snapshot of all processes in the system.
    hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hProcessSnap)
        return NULL;

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (::Process32First(hProcessSnap, &pe32))
    {
        int index = 0;
        do {
            QString &caption = QString::fromWCharArray(pe32.szExeFile);
            QStringList params;
            params += QString::number(pe32.th32ProcessID);

//            // Get other params
//            HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, false, pe32.th32ProcessID);
//            if (hProcess) {
//                // Get memory usage
//                PROCESS_MEMORY_COUNTERS procMem;
//                ::GetProcessMemoryInfo(hProcess, &procMem, sizeof(PROCESS_MEMORY_COUNTERS));
//                params += QString::number(procMem.WorkingSetSize/1024);
//                qDebug()<<params;
//                ::CloseHandle(hProcess);
//            }

            m_resModel->setItem(index, new ProcKiller(caption, params));
            ++index;
        } while (::Process32Next(hProcessSnap, &pe32));
    }

    ::CloseHandle(hProcessSnap);

    return true;
}

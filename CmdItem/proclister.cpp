#include "proclister.h"
#include "prockiller.h"
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <QProcess>
#include <QDebug>
#include "WMI/wmi.h"


#include <psapi.h>


/// 时间转换
static uint64_t file_time_2_utc(const FILETIME* ftime)
{
    LARGE_INTEGER li;

    assert(ftime);
    li.LowPart = ftime->dwLowDateTime;
    li.HighPart = ftime->dwHighDateTime;
    return li.QuadPart;
}


/// 获得CPU的核数
static int get_processor_number()
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return (int)info.dwNumberOfProcessors;
}




int get_cpu_usage()
{
    //cpu数量
    static int processor_count_ = -1;
    //上一次的时间
    static int64_t last_time_ = 0;
    static int64_t last_system_time_ = 0;


    FILETIME now;
    FILETIME creation_time;
    FILETIME exit_time;
    FILETIME kernel_time;
    FILETIME user_time;
    int64_t system_time;
    int64_t time;
    int64_t system_time_delta;
    int64_t time_delta;

    int cpu = -1;


    if(processor_count_ == -1)
    {
        processor_count_ = get_processor_number();
    }

    GetSystemTimeAsFileTime(&now);

    HANDLE h = ::OpenProcess(PROCESS_QUERY_INFORMATION, false, 14360);
    if (!GetProcessTimes(h, &creation_time, &exit_time,
        &kernel_time, &user_time))
    {
        // We don't assert here because in some cases (such as in the Task Manager)
        // we may call this function on a process that has just exited but we have
        // not yet received the notification.
        return -1;
    }
    ::CloseHandle(h);
    system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) / processor_count_;
    time = file_time_2_utc(&now);

    if ((last_system_time_ == 0) || (last_time_ == 0))
    {
        // First call, just set the last values.
        last_system_time_ = system_time;
        last_time_ = time;
        return -1;
    }

    system_time_delta = system_time - last_system_time_;
    time_delta = time - last_time_;

    assert(time_delta != 0);

    if (time_delta == 0)
        return -1;

    // We add time_delta / 2 so the result is rounded.
    cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
    last_system_time_ = system_time;
    last_time_ = time;
    return cpu;
}



int get_memory_usage(uint64_t* mem, uint64_t* vmem)
{
    PROCESS_MEMORY_COUNTERS pmc;
    if(GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        if(mem) *mem = pmc.WorkingSetSize;
        if(vmem) *vmem = pmc.PagefileUsage;
        return 0;
    }
    return -1;
}

ProcLister::ProcLister(const QString &text, QObject *parent) :
    CmdItem(text, parent)
{

}

bool ProcLister::exec()
{
//    HANDLE hProcessSnap;
//    PROCESSENTRY32 pe32 = {sizeof(pe32), };

//    // Take a snapshot of all processes in the system.
//    hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//    if (INVALID_HANDLE_VALUE == hProcessSnap)
//        return NULL;

//    // Retrieve information about the first process,
//    // and exit if unsuccessful
//    if (::Process32First(hProcessSnap, &pe32))
//    {
//        int index = 0;
//        do {
//            QString &caption = QString::fromWCharArray(pe32.szExeFile);
//            QStringList params;
//            params += QString::number(pe32.th32ProcessID);

////            // Get other params
////            HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, false, pe32.th32ProcessID);
////            if (hProcess) {
////                // Get memory usage
////                PROCESS_MEMORY_COUNTERS procMem;
////                ::GetProcessMemoryInfo(hProcess, &procMem, sizeof(PROCESS_MEMORY_COUNTERS));
////                params += QString::number(procMem.WorkingSetSize/1024);
////                qDebug()<<params;
////                ::CloseHandle(hProcess);
////            }

//            m_resModel->setItem(index, new ProcKiller(caption, params));
//            ++index;
//        } while (::Process32Next(hProcessSnap, &pe32));
//    }

//    ::CloseHandle(hProcessSnap);

//    QRegExp rx("(\\r\\n)+", Qt::CaseInsensitive, QRegExp::RegExp);
//    QProcess p;
//    p.start("TASKLIST /NH /FO CSV", QIODevice::ReadOnly);
//    p.waitForFinished();
//    QString processesInfo(p.readAllStandardOutput());
//    processesInfo.
//    QStringList processesInfoList = processesInfo.split(rx, QString::SkipEmptyParts);
//    qDebug()<<processesInfoList.count();
//    return true;
//    for (int i = 0; i < processesInfoList.count(); ++i) {
//        QString processInfo = processesInfoList.at(i);
//        rx.setPattern("^[^:]+:\\s+|\\r\\n[^:]+:\\s+");
//        QStringList processInfoList = processInfo.split(rx, QString::SkipEmptyParts);
//        if (processInfoList.at(2).trimmed() != "Services") {

//            qDebug()<<processInfoList;
//        }
//    }

//    QList<QVariantList> vlProcessInfo;
//    WmiInstance.get("SELECT * FROM Win32_Process WHERE (SessionId != '0')", QStringList({"Name","WorkingSetSize"}), vlProcessInfo);
//    qDebug()<<vlProcessInfo;
    QList<QVariantList> vlProcessInfo;
    WmiInstance.get("SELECT * FROM Win32_PerfRawData_PerfProc_Process", QStringList({"Name", "IDProcess", "PercentUserTime", "WorkingSetPrivate"}), vlProcessInfo);
    qDebug()<<vlProcessInfo;

    return true;
}

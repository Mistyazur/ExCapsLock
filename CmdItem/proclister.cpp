#include "proclister.h"
#include "prockiller.h"
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <QProcess>
#include <QDebug>
#include "WMI/wmi.h"

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

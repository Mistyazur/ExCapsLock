#include "proclister.h"
#include <windows.h>
#include <TlHelp32.h>
#include <QDebug>

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
//            QString info = QString::fromWCharArray(pe32.szExeFile).leftJustified(20, ' ', true)
//                    + QString::number(pe32.th32ProcessID).rightJustified(6, ' ');
            QString info = "<table width=\"100%\"><tr><td width=\"30%\">" + QString::fromWCharArray(pe32.szExeFile) + "</td>"
                    + "<td align=\"right\">" + QString::number(pe32.th32ProcessID) + "</td></tr></table> ";
            qDebug()<<info;
            m_resModel->setItem(index, new QStandardItem(info));
            ++index;
        } while (::Process32Next(hProcessSnap, &pe32));
    }

    ::CloseHandle(hProcessSnap);

    return true;
}

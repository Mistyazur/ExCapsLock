#include "winlister.h"
#include "wincontrollor.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <QDebug>


winLister::winLister(const QString &text, QObject *parent) :
    CmdItem(text, parent)
{

}

bool winLister::exec()
{
    int index = 0;

    // Get processes

    QHash<DWORD, QString> procIdNameHash;
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32 = {};

    hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hProcessSnap)
        return false;

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (::Process32First(hProcessSnap, &pe32))
    {
        do {

            procIdNameHash.insert(pe32.th32ProcessID, QString::fromWCharArray(pe32.szExeFile));

        } while (::Process32Next(hProcessSnap, &pe32));
    }

    ::CloseHandle(hProcessSnap);

    // Get windows

    WINDOWINFO winInfo = {sizeof(WINDOWINFO), };
    HWND hWnd = ::GetTopWindow(NULL);

    do
    {
        ::GetWindowInfo(hWnd, &winInfo);
        if (!(winInfo.dwStyle & WS_DISABLED)
                && (winInfo.dwStyle & WS_VISIBLE)
                && !(winInfo.dwExStyle & WS_EX_TRANSPARENT))
        {
            if ((winInfo.rcWindow.left != 0) &&
                    (winInfo.rcWindow.top != 0) &&
                    (winInfo.rcWindow.right != 0) &&
                    (winInfo.rcWindow.bottom != 0))
            {
                WCHAR szTitle[1024];
                ::GetWindowText(hWnd, szTitle, 1024);

                DWORD dwPid;
                ::GetWindowThreadProcessId(hWnd, &dwPid);

                QString processName = procIdNameHash.value(dwPid, "Unknown");
                QString &title = QString::fromWCharArray(szTitle);
                if (title.isEmpty())
                    title = "None";

                WinControllor *winCtrl = new WinControllor(processName.left(processName.lastIndexOf(".")), this);
                winCtrl->setTitle(title);
                winCtrl->setProcess(processName);
                winCtrl->setId(hWnd);
                m_resultModel->setItem(index, winCtrl);

                ++index;
            }
        }
    } while ((hWnd = ::GetWindow(hWnd, GW_HWNDNEXT)) != NULL);

    return true;
}




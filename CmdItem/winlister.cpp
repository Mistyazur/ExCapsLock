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
    if (::Process32First(hProcessSnap, &pe32)) {
        do {
            procIdNameHash.insert(pe32.th32ProcessID, QString::fromWCharArray(pe32.szExeFile));
        } while (::Process32Next(hProcessSnap, &pe32));
    }

    ::CloseHandle(hProcessSnap);

    // Get windows

    HWND hWnd = ::GetTopWindow(NULL);
    WCHAR szTitle[1024];
    DWORD dwPid;
    WINDOWINFO desktopInfo;
    desktopInfo.cbSize = sizeof(WINDOWINFO);
    WINDOWINFO winInfo;
    winInfo.cbSize = sizeof(WINDOWINFO);

    if ( !::GetWindowInfo(::GetDesktopWindow(), &desktopInfo) )
        return false;

    do {
        if ( !::GetWindowInfo(hWnd, &winInfo) )
            continue;

        if ( !(winInfo.dwStyle & WS_VISIBLE) ||
             (winInfo.dwExStyle & WS_EX_NOACTIVATE) )
            continue;

        if ( (winInfo.rcWindow.left == desktopInfo.rcWindow.left) &&
//             && (winInfo.rcWindow.top == desktopInfo.rcWindow.top)
//             && (winInfo.rcWindow.bottom == desktopInfo.rcWindow.bottom)
             (winInfo.rcWindow.right == desktopInfo.rcWindow.right) )
            continue;

        ::GetWindowThreadProcessId(hWnd, &dwPid);
        ::GetWindowText(hWnd, szTitle, 1024);

        QString processName = procIdNameHash.value(dwPid, "Unknown");
        QString &title = QString::fromWCharArray(szTitle);
        title = title.isEmpty() ? "None" : title;

        WinControllor *winCtrl = new WinControllor(processName.left(processName.lastIndexOf(".")), this);
        winCtrl->setTitle(title);
        winCtrl->setProcess(processName);
        winCtrl->setId(hWnd);
        m_resultModel->setItem(index, winCtrl);

        ++index;
    } while ((hWnd = ::GetWindow(hWnd, GW_HWNDNEXT)) != NULL);

    return true;
}




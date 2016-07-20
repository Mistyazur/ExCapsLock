#include "wincontrol.h"
#include <Windows.h>
#include <QDebug>

WinControl::WinControl(const QString &text, QObject *parent) :
    CmdItem(text, parent)
{

}

bool WinControl::exec()
{
    HWND hWnd = (HWND)m_infoList.last().toInt(nullptr, 16);

    // Restore minimized window

    WINDOWPLACEMENT placement;
    placement.length = sizeof(placement);
    if(GetWindowPlacement(hWnd, &placement))
    {
        if ((placement.showCmd & SW_SHOWMINIMIZED))
            ::SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
    }

    // Set window foreground

    ::SetForegroundWindow(hWnd);

    return true;
}

void WinControl::setInfo(const QStringList &infoList)
{
    m_infoList = infoList;
}

const QString WinControl::html(const QString &searchKeyword)
{
    return QString("<h6>%1</h6><p>%2</p>")
            .arg(m_infoList.first())
            .arg(highlight(text(), searchKeyword));
}

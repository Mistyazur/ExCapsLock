#include "wincontrollor.h"
#include <Windows.h>
#include <QDebug>

WinControllor::WinControllor(const QString &text, QObject *parent) :
    CmdItem(text, parent)
{

}

bool WinControllor::exec()
{
    bringToFront(m_id);

    return true;
}

const QString WinControllor::html(const QString &searchKeyword)
{
    return QString("<h6>%1</h6><p>%2</p>")
            .arg(m_title)
            .arg(highlight(text(), searchKeyword));
}

void WinControllor::bringToFront(HWND hWnd)
{
    // Restore minimized window

    if (::IsIconic(hWnd))
        ::OpenIcon(hWnd);

    // Set window foreground

    for (;;)
    {
        if (::SetForegroundWindow(hWnd))
            break;
    }
}

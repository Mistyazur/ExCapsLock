#include "wincontrollor.h"
#include <Windows.h>
#include <QDebug>

WinControllor::WinControllor(const QString &text, QObject *parent) :
    CmdItem(text, parent)
{

}

bool WinControllor::exec()
{
    HWND hWnd = (HWND)m_infoList.last().toLongLong(nullptr, 16);

    bringToFront(hWnd);

    return true;
}

void WinControllor::setInfo(const QStringList &infoList)
{
    m_infoList = infoList;
}

const QString WinControllor::html(const QString &searchKeyword)
{
    return QString("<h6>%1</h6><p>%2</p>")
            .arg(m_infoList.first())
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

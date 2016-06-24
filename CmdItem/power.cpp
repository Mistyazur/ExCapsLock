#include "power.h"
#include <windows.h>
#include <PowrProf.h>

Power::Power(const QString &text, int type) :
    CmdItem(text),
    m_type(type)
{
    HANDLE           hToken;
    TOKEN_PRIVILEGES tkp;

    ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    ::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1; // Set 1 privilege
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // Get the shutdown privilege for this process
    ::AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
}

int Power::exec()
{
    bool ret = false;

    switch (m_type)
    {
    case 0:
        ret = ::SetSuspendState(false, false, false);	// Sleep
        break;
    case 1:
        ret = ::SetSuspendState(true, false, false);	// hibernate
        break;
    case 2:
        ret = ::ExitWindowsEx(EWX_SHUTDOWN, 0);
        break;
    case 3:
        ret = ::ExitWindowsEx(EWX_REBOOT, 0);
        break;
    }

    return (ret ? CMD_SUCCEED : CMD_FAILED);
}

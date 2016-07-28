#include "applauncher.h"
#include "cmditemdelegate.h"
#include <Windows.h>
#include <Wtsapi32.h>
#include <UserEnv.h>
#include <QProcess>

AppLauncher::AppLauncher(const QString &text, const QString &path, QObject *parent) :
    CmdItem(text, parent), m_path(path)
{

}

bool AppLauncher::exec()
{
#ifndef _M_X64
    PVOID OldValue = NULL;
    if(Wow64DisableWow64FsRedirection(&OldValue))
    {
#endif
        QString path = "\"" + m_path + "\"";
        if (!path.isEmpty())
            return QProcess::startDetached(path);
#ifndef _M_X64
        Wow64RevertWow64FsRedirection(OldValue);
    }
#endif

    return false;
}

const QString AppLauncher::html(const QString &searchKeyword)
{
     return QString("<h6>%1</h6><p>%2</p>")
             .arg(highlight(text(), searchKeyword))
             .arg(m_path);
}

bool AppLauncher::executeAsActiveUser(const QString &process)
{
    // Get active user token.

    uint dwSessionId = WTSGetActiveConsoleSessionId();
    HANDLE hToken = NULL;
    if (!WTSQueryUserToken(dwSessionId, &hToken))
        return FALSE;

    // Security attibute structure used in DuplicateTokenEx and CreateProcessAsUser
    // I would prefer to not have to use a security attribute variable and to just
    // simply pass null and inherit (by default) the security attributes
    // of the existing token. However, in C# structures are value types and therefore
    // cannot be assigned the null value.

    SECURITY_ATTRIBUTES sa = {0};
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);

    // Copy the access token.
    // The newly created token will be a primary token.

    HANDLE hUserTokenDup = NULL;
    if (!DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, &sa, SecurityIdentification, TokenPrimary, &hUserTokenDup))
    {
        CloseHandle(hToken);
        return FALSE;
    }

    // Create environment block.

    LPVOID envBlock = NULL;
    if (!CreateEnvironmentBlock(&envBlock, hUserTokenDup, false))
    {
        CloseHandle(hToken);
        CloseHandle(hUserTokenDup);
        return FALSE;
    }

    // By default CreateProcessAsUser creates a process on a non-interactive window station, meaning
    // the window station has a desktop that is invisible and the process is incapable of receiving
    // user input. To remedy this we set the lpDesktop parameter to indicate we want to enable user
    // interaction with the new process.

    STARTUPINFO si = {0};
    si.cb = sizeof(STARTUPINFO);
    si.lpDesktop = (LPWSTR)TEXT("winsta0\\default"); // interactive window station parameter; basically this indicates that the process created can display a GUI on the desktop
    PROCESS_INFORMATION pi = {0};

    // Flags that specify the priority and creation method of the process.

    int dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT;

    // Create a new process in the current user's logon session.

    WCHAR szProcess[MAX_PATH] = {};
    process.toWCharArray(szProcess);
    bool result = CreateProcessAsUser(hUserTokenDup,
                                      NULL,
                                      szProcess,
                                      &sa,
                                      &sa,
                                      FALSE,
                                      dwCreationFlags,
                                      envBlock,
                                      NULL,
                                      &si,
                                      &pi
                                      );

    // Invalidate the handles.

    CloseHandle(hToken);
    CloseHandle(hUserTokenDup);
    DestroyEnvironmentBlock(envBlock);

    return TRUE;
}

#include "applauncher.h"
#include "cmditemdelegate.h"

#include <QProcess>

#include <Windows.h>
#include <Wtsapi32.h>
#include <UserEnv.h>

static BOOL RunAsActiveUser(LPCWSTR lpszProcess)
{
    BOOL bResult = false;
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    SECURITY_ATTRIBUTES sa = {0};
    HANDLE hActiveUserToken = NULL;
    HANDLE hActiveUserTokenDup = NULL;
    LPVOID hActiveUserEnvBlock = NULL;

    // Get active user token.
    if (!WTSQueryUserToken(WTSGetActiveConsoleSessionId(), &hActiveUserToken))
        goto CLEANUP;

    // Copy the access token.
    // The newly created token will be a primary token.
    if (!DuplicateTokenEx(hActiveUserToken, MAXIMUM_ALLOWED, &sa, SecurityIdentification, TokenPrimary, &hActiveUserTokenDup))
        goto CLEANUP;

    // Create environment block.
    if (!CreateEnvironmentBlock(&hActiveUserEnvBlock, hActiveUserTokenDup, false))
        goto CLEANUP;

    // By default CreateProcessAsUser creates a process on a non-interactive window station, meaning
    // the window station has a desktop that is invisible and the process is incapable of receiving
    // user input. To remedy this we set the lpDesktop parameter to indicate we want to enable user
    // interaction with the new process.

    si.cb = sizeof(STARTUPINFO);
    si.lpDesktop = (LPWSTR)TEXT("winsta0\\default"); // interactive window station parameter; basically this indicates that the process created can display a GUI on the desktop

    // Security attibute structure used in DuplicateTokenEx and CreateProcessAsUser
    // I would prefer to not have to use a security attribute variable and to just
    // simply pass null and inherit (by default) the security attributes
    // of the existing token. However, in C# structures are value types and therefore
    // cannot be assigned the null value.
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);

    // Flags that specify the priority and creation method of the process.
    int dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT;

    // Create a new process in the current user's logon session.

    WCHAR szCmd[MAX_PATH];
    wcscpy_s(szCmd, lpszProcess);
    bResult = CreateProcessAsUser(hActiveUserTokenDup,
                                      NULL,
                                      szCmd,
                                      &sa,
                                      &sa,
                                      FALSE,
                                      dwCreationFlags,
                                      hActiveUserEnvBlock,
                                      NULL,
                                      &si,
                                      &pi
                                      );
CLEANUP:
    if (hActiveUserToken != NULL)
        CloseHandle(hActiveUserToken);

    if (hActiveUserTokenDup != NULL)
        CloseHandle(hActiveUserTokenDup);

    if (hActiveUserEnvBlock != NULL)
        DestroyEnvironmentBlock(hActiveUserEnvBlock);

    return TRUE;
}

AppLauncher::AppLauncher(const QString &text, const QString &path, bool runAsSystem, QObject *parent) :
    CmdItem(text, parent),
    m_path(path),
    m_runAsSystem(runAsSystem)
{

}

bool AppLauncher::exec()
{
#ifndef _M_X64
    PVOID OldValue = NULL;
    if(Wow64DisableWow64FsRedirection(&OldValue))
    {
#endif
        if (!m_path.isEmpty())
        {
            QString cmd = "\"" + m_path + "\"";
            cmd.replace("/", "\\");
//            cmd = "cmd.exe /c start " + cmd;

            if (m_runAsSystem) {
                return QProcess::startDetached(cmd);
            } else {
                return RunAsActiveUser(cmd.toStdWString().c_str());
            }
        }
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

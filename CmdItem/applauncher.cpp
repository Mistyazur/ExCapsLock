#include "applauncher.h"
#include "cmditemdelegate.h"
#include <Windows.h>
#include <Wtsapi32.h>
#include <UserEnv.h>
#include <QProcess>
#include <QDebug>

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
        if (!m_path.isEmpty())
        {
            QString path = "\"" + m_path + "\"";
            return QProcess::startDetached(path);

//            QString path = "\"" + m_path + "\"";
//            path.replace("/", "\\");
//            WCHAR szProcess[MAX_PATH] = {};
//            path.toWCharArray(szProcess);
//            return executeAsActiveUser(szProcess);
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

bool AppLauncher::executeAsActiveUser(wchar_t *szProcess)
{
    bool bResult = false;
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};

    // Security attibute structure used in DuplicateTokenEx and CreateProcessAsUser
    // I would prefer to not have to use a security attribute variable and to just
    // simply pass null and inherit (by default) the security attributes
    // of the existing token. However, in C# structures are value types and therefore
    // cannot be assigned the null value.

    SECURITY_ATTRIBUTES sa = {0};
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);

    // Get active user token.

    uint dwSessionId = WTSGetActiveConsoleSessionId();
    HANDLE hActiveUserToken = NULL;
    if (!WTSQueryUserToken(dwSessionId, &hActiveUserToken))
        goto CLEANUP;

    // Copy the access token.
    // The newly created token will be a primary token.

    HANDLE hActiveUserTokenDup = NULL;
    if (!DuplicateTokenEx(hActiveUserToken, MAXIMUM_ALLOWED, &sa, SecurityIdentification, TokenPrimary, &hActiveUserTokenDup))
        goto CLEANUP;

    // Create environment block.

    LPVOID activeUserEnvBlock = NULL;
    if (!CreateEnvironmentBlock(&activeUserEnvBlock, hActiveUserTokenDup, false))
        goto CLEANUP;

    // By default CreateProcessAsUser creates a process on a non-interactive window station, meaning
    // the window station has a desktop that is invisible and the process is incapable of receiving
    // user input. To remedy this we set the lpDesktop parameter to indicate we want to enable user
    // interaction with the new process.

    si.cb = sizeof(STARTUPINFO);
    si.lpDesktop = (LPWSTR)TEXT("winsta0\\default"); // interactive window station parameter; basically this indicates that the process created can display a GUI on the desktop

    // Flags that specify the priority and creation method of the process.

    int dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT;

    // Create a new process in the current user's logon session.

    bResult = CreateProcessAsUser(hActiveUserTokenDup,
                                      NULL,
                                      szProcess,
                                      &sa,
                                      &sa,
                                      FALSE,
                                      dwCreationFlags,
                                      activeUserEnvBlock,
                                      NULL,
                                      &si,
                                      &pi
                                      );
CLEANUP:
    if (hActiveUserToken != NULL)
        CloseHandle(hActiveUserToken);

    if (hActiveUserTokenDup != NULL)
        CloseHandle(hActiveUserTokenDup);

    if (activeUserEnvBlock != NULL)
        DestroyEnvironmentBlock(activeUserEnvBlock);

    return TRUE;
}

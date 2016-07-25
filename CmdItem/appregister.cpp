#include "config.h"
#include "appregister.h"
#include "jsettings.h"
#include <Windows.h>
#include <Psapi.h>
#include <QCoreApplication>
#include <QFile>
#include <QTimer>
#include <QDebug>

#define KEY_APP_PATH	"run.app.path"

AppRegister::AppRegister(const QString &text, QObject *parent) :
    CmdItem(text, parent)
{
    QTimer::singleShot(100, this, &AppRegister::load);
}

bool AppRegister::exec()
{
    QTimer::singleShot(100, this, &AppRegister::capture);
    return true;
}

void AppRegister::load()
{
    JSettings settings(USER_SETTINGS);
    m_processList = settings.value(KEY_APP_PATH).toStringList();

#ifndef _M_X64
    PVOID OldValue = NULL;
    if(Wow64DisableWow64FsRedirection(&OldValue))
    {
#endif
        for (auto process : m_processList)
        {
            QFile file(process);
            if (!file.exists())
                m_processList.removeOne(process);
        }

#ifndef _M_X64
        Wow64RevertWow64FsRedirection(OldValue);
    }
#endif

    emit updateApps(m_processList);
}

bool AppRegister::capture()
{
    bool bResult = false;
    HWND hWnd = ::GetForegroundWindow();
    DWORD dwPid;
    WCHAR szProc[MAX_PATH];

    if (::GetWindowThreadProcessId(hWnd, &dwPid))
    {
        HANDLE hProc = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
        if (hProc != INVALID_HANDLE_VALUE)
        {
            if (::GetModuleFileNameEx(hProc, 0, szProc, MAX_PATH))
            {
                QString &process = QString::fromWCharArray(szProc);
                process.replace("\\", "/");
                if (!m_processList.contains(process))
                {
                    m_processList += process;
                    emit updateApps(m_processList);

                    JSettings settings(USER_SETTINGS);
                    settings.remove(KEY_APP_PATH);
                    settings.setValue(KEY_APP_PATH, QVariant::fromValue(m_processList));
                }

                bResult = true;
            }
            ::CloseHandle(hProc);
        }
    }

    return bResult;
}

#include "config.h"
#include "appcapture.h"
#include "jsettings.h"
#include <Windows.h>
#include <Psapi.h>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>


#define KEY_APP_PATH	"run.app.path"

AppCapture::AppCapture(QObject *parent) : QObject(parent)
{
    JSettings settings(USER_SETTINGS);
    m_processList = settings.value(KEY_APP_PATH).toStringList();
    PVOID OldValue = NULL;

    if( Wow64DisableWow64FsRedirection(&OldValue) ) {
        for (auto process : m_processList)
        {
            QFile file(process);
            if (!file.exists())
                m_processList.removeOne(process);
        }

        Wow64RevertWow64FsRedirection(OldValue);
    }
}

bool AppCapture::capture()
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

QStringList AppCapture::processList()
{
   return m_processList;
}

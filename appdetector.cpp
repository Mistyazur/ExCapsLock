#include "appdetector.h"
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <Windows.h>
#include <Psapi.h>


AppDetector::AppDetector(QObject *parent) : QObject(parent)
{
    QSettings settings(qApp->applicationDirPath() + "/ExCapsLock.ini", QSettings::IniFormat);

    PVOID OldValue = NULL;
    if( Wow64DisableWow64FsRedirection(&OldValue) )
    {
        for (int i = 0; ; ++i)
        {
            QString &key = QString("Apps/path%1").arg(i);
            QString &process = settings.value(key, "").toString();
            if (process.isEmpty())
                break;
            else
            {
                // Check if this process exist or not
                QFile file(process);
                if (file.exists())
                    m_processList += process;
            }
        }

        Wow64RevertWow64FsRedirection(OldValue);
    }

    startTimer(1000);
}

void AppDetector::timerEvent(QTimerEvent *)
{
    static HWND hPrevWnd = nullptr;
    HWND hWnd = ::GetForegroundWindow();
    DWORD dwPid;
    WCHAR szProc[MAX_PATH];

    if (hWnd && ::GetWindowThreadProcessId(hWnd, &dwPid) && hWnd != hPrevWnd)
    {
        HANDLE hProc = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
        if (hProc != INVALID_HANDLE_VALUE)
        {
            if (::GetModuleFileNameEx(hProc, 0, szProc, MAX_PATH))
            {
                QString &process = QString::fromWCharArray(szProc);
                process.replace("\\", "/");
                if (/*!process.startsWith("c:/windows", Qt::CaseInsensitive) && */!m_processList.contains(process))
                {
                    m_processList += process;

                    // Save to file
                    QSettings settings(qApp->applicationDirPath() + "/ExCapsLock.ini", QSettings::IniFormat);
                    settings.remove("Apps");
                    for (int i = 0; i < m_processList.count(); ++i)
                    {
                        settings.setValue(QString("Apps/path%1").arg(i), m_processList.at(i));
                    }
                }
            }
            ::CloseHandle(hProc);
        }

        hPrevWnd = hWnd;
    }
}

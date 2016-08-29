#include "prockiller.h"

#include <QLocale>

#include <Windows.h>

ProcKiller::ProcKiller(const QString &text, QObject *parent) :
    CmdItem(text, parent)
{
    m_cpu = 0;
    m_mem = 1;
}

bool ProcKiller::exec()
{
    HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, m_pid);
    if (NULL != hProcess) {
        ::TerminateProcess(hProcess, 0);

        ::CloseHandle(hProcess);
    }
    return true;
}

const QString ProcKiller::html(const QString &searchKeyword)
{
    QString html = QString("<table width=\"100%\"><tr>"		\
                           "<td width=\"50%\">%1</td>"		\
                           "<td width= \"10%\">%2</td>"	\
                           "<td width= \"15%\">%3</td>"	\
                           "<td width= \"5%\" align=\"right\">%4</td>"	\
                           "<td width= \"20%\" align=\"right\">%5</td>"	\
                           "</td></tr></table>")
            .arg(highlight(m_name, searchKeyword))
            .arg(QString::number(m_pid))
            .arg(m_user)
            .arg(QString("%1").arg(m_cpu, 2, 10, QChar('0')))
            .arg(QLocale::system().toString(m_mem/1024) + " K");
    return html;
}

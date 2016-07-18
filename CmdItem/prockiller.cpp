#include "prockiller.h"
#include <Windows.h>

ProcKiller::ProcKiller(const QString &text, const QStringList &params, QObject *parent) :
    CmdItem(text, parent), m_params(params)
{

}

bool ProcKiller::exec()
{
    HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, m_params.first().toInt());
    if (NULL != hProcess) {
        ::TerminateProcess(hProcess, 0);

        ::CloseHandle(hProcess);
    }
    return true;
}

void ProcKiller::setParams(const QStringList params)
{
    m_params = params;
}

const QString ProcKiller::html(const QString &searchKeyword)
{
    QString html = QString("<table width=\"100%\"><tr>"		\
                           "<td width=\"50%\">%1</td>"		\
                           "<td width= \"10%\">%2</td>"	\
                           "<td width= \"20%\">%3</td>"	\
                           "<td width= \"10%\" align=\"right\">%4</td>"	\
                           "<td width= \"10%\" align=\"right\">%5</td>"	\
                           "</td></tr></table>")
            .arg(highlight(text(), searchKeyword))
            .arg(m_params.at(0))
            .arg(m_params.at(1))
            .arg(m_params.at(2))
            .arg(m_params.at(3));
    return html;
}

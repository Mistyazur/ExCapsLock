#include "prockiller.h"
#include <Windows.h>

ProcKiller::ProcKiller(const QString &text, QObject *parent) :
    CmdItem(text, parent)
{

}

bool ProcKiller::exec()
{
    HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, m_infoList.first().toInt());
    if (NULL != hProcess) {
        ::TerminateProcess(hProcess, 0);

        ::CloseHandle(hProcess);
    }
    return true;
}

void ProcKiller::setInfo(const QStringList &infoList)
{
    m_infoList = infoList;
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
            .arg(m_infoList.at(0))
            .arg(m_infoList.at(1))
            .arg(m_infoList.at(2))
            .arg(m_infoList.at(3));
    return html;
}

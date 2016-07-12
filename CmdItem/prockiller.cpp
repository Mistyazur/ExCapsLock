#include "prockiller.h"

ProcKiller::ProcKiller(const QString &text, const QStringList &params, QObject *parent) :
    CmdItem(text, parent), m_params(params)
{

}

bool ProcKiller::exec()
{
    return true;
}

const QString ProcKiller::html(const QString &searchKeyword)
{
    QString html = QString("<table width=\"100%\"><tr>"		\
                           "<td width=\"30%\">%1</td>"		\
                           "<td align=\"right\">%2</td>"	\
                           "</td></tr></table>")
            .arg(highlight(text(), searchKeyword)).arg(m_params.at(0));
    return html;
}

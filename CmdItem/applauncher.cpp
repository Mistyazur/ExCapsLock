#include "applauncher.h"
#include "cmditemdelegate.h"
#include <QProcess>


AppLauncher::AppLauncher(const QString &text)  :
    CmdItem(text)
{

}

int AppLauncher::exec()
{
    QRegExp rx(S_PARAGRAPH("(.+)"), Qt::CaseInsensitive, QRegExp::RegExp);
    if (rx.indexIn(text()) != -1)
    {
        QProcess::startDetached(rx.cap(1));
    }

    return 0;
}

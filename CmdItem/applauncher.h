#ifndef APPLAUNCHER_H
#define APPLAUNCHER_H

#include "cmditem.h"

class AppLauncher : public CmdItem
{
    Q_OBJECT
public:
    AppLauncher(const QString &text, const QString &path, QObject *parent = 0);
    bool exec();
    const QString html(const QString &searchKeyword);
private:
    QString m_path;

    bool runAsActiveUser(wchar_t *szCmd);
};

#endif // APPLAUNCHER_H

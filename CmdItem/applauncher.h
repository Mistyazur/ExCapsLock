#ifndef APPLAUNCHER_H
#define APPLAUNCHER_H

#include "cmditem.h"

class AppLauncher : public CmdItem
{
    Q_OBJECT
public:
    AppLauncher(const QString &text, const QString &path, bool runAsSystem, QObject *parent = 0);
    bool exec();
    const QString html(const QString &searchKeyword);
private:
    QString m_path;
    bool m_runAsSystem;
};

#endif // APPLAUNCHER_H

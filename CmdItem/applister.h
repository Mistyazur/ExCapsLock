#ifndef APPLISTER_H
#define APPLISTER_H

#include "cmditem.h"

class AppLister : public CmdItem
{
    Q_OBJECT
public:
    AppLister(const QString &text, QObject *parent = 0);
    bool exec();
public slots:
    void updateApps(const QStringList &apps);
private:
    QStringList m_appList;
};

#endif // APPLISTER_H

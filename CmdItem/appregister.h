#ifndef APPREGISTER_H
#define APPREGISTER_H

#include "cmditem.h"

class AppRegister : public CmdItem
{
    Q_OBJECT
public:
    AppRegister(const QString &text , QObject *parent = nullptr);
    bool exec();
signals:
    void updateApps(const QStringList &apps);
private:
    QStringList m_processList;
    bool capture();
};

#endif // APPREGISTER_H

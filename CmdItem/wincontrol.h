#ifndef WINCONTROL_H
#define WINCONTROL_H

#include "cmditem.h"

class WinControl : public CmdItem
{
public:
    WinControl(const QString &text, QObject *parent = 0);
    bool exec();
    void setInfo(const QStringList &infoList);
    const QString html(const QString &searchKeyword);
private:
    QStringList m_infoList;
};

#endif // WINCONTROL_H

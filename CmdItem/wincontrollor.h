#ifndef WINCONTROLLOR_H
#define WINCONTROLLOR_H

#include "cmditem.h"

class WinControllor : public CmdItem
{
public:
    WinControllor(const QString &text, QObject *parent = 0);
    bool exec();
    void setInfo(const QStringList &infoList);
    const QString html(const QString &searchKeyword);

    static void bringToFront(HWND hWnd);
private:
    QStringList m_infoList;
};

#endif // WINCONTROLLOR_H

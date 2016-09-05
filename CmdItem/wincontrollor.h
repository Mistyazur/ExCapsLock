#ifndef WINCONTROLLOR_H
#define WINCONTROLLOR_H

#include "cmditem.h"

class WinControllor : public CmdItem
{
public:
    WinControllor(const QString &text, QObject *parent = 0);
    bool exec();

    HWND id() { return m_id; }
    void setId(HWND id) { m_id = id; }

    const QString title() { return m_title; }
    void setTitle(const QString &title) { m_title = title; }

    const QString process() { return m_process; }
    void setProcess(const QString &process) { m_process = process; }

    const QString html(const QString &searchKeyword);

    static void bringToFront(HWND hWnd);
private:
    HWND m_id;
    QString m_title;
    QString m_process;
};

#endif // WINCONTROLLOR_H

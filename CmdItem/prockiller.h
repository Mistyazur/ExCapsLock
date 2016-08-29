#ifndef PROCKILLER_H
#define PROCKILLER_H

#include "cmditem.h"

class ProcKiller : public CmdItem
{
    Q_OBJECT
public:
    ProcKiller(const QString &text, QObject *parent = 0);

    bool exec();

    int pid() { return m_pid; }
    void setPid(int pid) { m_pid = pid; }

    int cpu() { return m_cpu; }
    void setCpu(int cpu) {
        if (cpu != -1)
            m_cpu = cpu;
    }

    int mem() { return m_mem; }
    void setMem(int mem) {
        if (mem != -1 && mem != 0)
            m_mem = mem;
    }

    const QString name() { return m_name; }
    void setName(const QString &name) { m_name = name; }

    const QString user() { return m_user; }
    void setUser(const QString &user) { m_user = user; }

    const QString html(const QString &searchKeyword);
private:
    int 		m_pid;
    int			m_cpu;
    int			m_mem;
    QString		m_name;
    QString		m_user;
};

#endif // PROCKILLER_H

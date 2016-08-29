#ifndef PROCKILLER_H
#define PROCKILLER_H

#include "cmditem.h"

class ProcKiller : public CmdItem
{
    Q_OBJECT
public:
    ProcKiller(const QString &text, QObject *parent = 0);

    bool exec();

    QStringList getInfo() { return m_infoList; }
    void setInfo(const QStringList &infoList);

    const QString html(const QString &searchKeyword);
private:
    QStringList m_infoList;
};

#endif // PROCKILLER_H

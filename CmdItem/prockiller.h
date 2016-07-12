#ifndef PROCKILLER_H
#define PROCKILLER_H

#include "cmditem.h"

class ProcKiller : public CmdItem
{
    Q_OBJECT
public:
    ProcKiller(const QString &text, const QStringList &params, QObject *parent = 0);
    bool exec();
    const QString html(const QString &searchKeyword);
private:
    QStringList m_params;
};

#endif // PROCKILLER_H

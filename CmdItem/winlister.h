#ifndef WINLISTER_H
#define WINLISTER_H

#include "cmditem.h"

class winLister : public CmdItem
{
    Q_OBJECT
public:
    winLister(const QString &text, QObject *parent = 0);
    bool exec();
private:
};

#endif // WINLISTER_H

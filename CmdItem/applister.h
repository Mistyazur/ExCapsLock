#ifndef APPLISTER_H
#define APPLISTER_H

#include "cmditem.h"

class AppLister : public CmdItem
{
    Q_OBJECT
public:
    AppLister(const QString &text, QObject *parent = 0);
    bool exec();
private:
};

#endif // APPLISTER_H

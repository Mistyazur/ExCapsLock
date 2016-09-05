#ifndef APPLISTER_H
#define APPLISTER_H

#include "cmditem.h"

class AppLister : public CmdItem
{
    Q_OBJECT
public:
    AppLister(const QString &text, bool runAsSystem, QObject *parent = 0);
    bool exec();
private:
    bool m_runAsSystem;
};

#endif // APPLISTER_H

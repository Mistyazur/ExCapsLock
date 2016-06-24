#ifndef CMDITEM_H
#define CMDITEM_H

#include <QStandardItem>

class CmdItem : public QStandardItem
{
public:
    CmdItem(const QString &text);
    virtual bool exec() = 0;
};

#endif // CMDITEM_H

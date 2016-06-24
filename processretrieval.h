#ifndef PROCESSRETRIEVAL_H
#define PROCESSRETRIEVAL_H

#include <QObject>
#include <QStringList>

class ProcessRetrieval : public QObject
{
public:
    ProcessRetrieval(QObject *parent = 0);
protected:
    void timerEvent(QTimerEvent *);
private:
    QStringList m_processList;
};

#endif // PROCESSRETRIEVAL_H

#ifndef PROCESSRETRIEVER_H
#define PROCESSRETRIEVER_H

#include <QObject>
#include <QStringList>

class ProcessRetriever : public QObject
{
public:
    ProcessRetriever(QObject *parent = 0);
protected:
    void timerEvent(QTimerEvent *);
private:
    QStringList m_processList;
};

#endif // PROCESSRETRIEVER_H

#ifndef APPDETECTOR_H
#define APPDETECTOR_H

#include <QObject>

class AppDetector : public QObject
{
    Q_OBJECT
public:
    explicit AppDetector(QObject *parent = 0);

signals:

public slots:
protected:
    void timerEvent(QTimerEvent *);
private:
    QStringList m_processList;
};

#endif // APPDETECTOR_H

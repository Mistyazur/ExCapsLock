#ifndef APPCAPTURE_H
#define APPCAPTURE_H

#include <QObject>

class AppCapture : public QObject
{
    Q_OBJECT
public:
    explicit AppCapture(QObject *parent = 0);
    bool capture();
    QStringList processList();
signals:

public slots:
    
private:
    QStringList m_processList;
};

#endif // APPCAPTURE_H

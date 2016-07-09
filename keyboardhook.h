#ifndef KEYBOARDHOOK_H
#define KEYBOARDHOOK_H

#include <QObject>
#include <Windows.h>

class KeyboardHook : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardHook(QObject *parent = 0);
    ~KeyboardHook();
signals:
    void showCmd();

public slots:

private:
};

#endif // KEYBOARDHOOK_H

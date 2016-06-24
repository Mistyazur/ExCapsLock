#include <QApplication>
#include <QDebug>
#include <wmi.h>
#include "keyboardhook.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KeyboardHook kh;
    QVariantList vl;
    WmiInstance.get("SELECT * FROM Win32_Process WHERE Name = 'PotPlayerMini64.exe'", "ExecutablePath", vl);
    WmiInstance.get("SELECT * FROM Win32_Process WHERE Name = 'notepad.exe'", "ExecutablePath", vl);
    qDebug()<<vl;

    return a.exec();
}

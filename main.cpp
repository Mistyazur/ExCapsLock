#include <QApplication>
#include <wmi.h>
#include "keyboardhook.h"
#include "processretriever.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KeyboardHook kh;
    ProcessRetriever pr;

    return a.exec();
}

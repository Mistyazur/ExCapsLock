#include <QApplication>
#include <wmi.h>
#include "keyboardhook.h"
#include "processretrieval.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KeyboardHook kh;
    ProcessRetrieval pr;

    return a.exec();
}

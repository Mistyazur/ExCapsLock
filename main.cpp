#include "keyboardhook.h"
#include "processretriever.h"
#include "appdetector.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KeyboardHook kh;
//    ProcessRetriever pr;
    AppDetector ad;

    return a.exec();
}

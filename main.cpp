#include "keyboardhook.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KeyboardHook kh;

    return a.exec();
}

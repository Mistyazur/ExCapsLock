#-------------------------------------------------
#
# Project created by QtCreator 2016-06-15T19:47:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExCapsLock
TEMPLATE = app

SOURCES += main.cpp \
    keyboardhook.cpp \
    cmdpalette.cpp \
    shadowwidget.cpp \
    CmdItem/apps.cpp \
    CmdItem/cmditem.cpp \
    CmdItem/power.cpp \
    CmdItem/systemcmd.cpp \
    processretrieval.cpp

HEADERS  += \
    keyboardhook.h \
    cmdpalette.h \
    shadowwidget.h \
    CmdItem/apps.h \
    CmdItem/cmditem.h \
    CmdItem/power.h \
    CmdItem/systemcmd.h \
    processretrieval.h

FORMS    += cmd.ui

LIBS += -lUser32 -lAdvapi32 -lPowrProf

DEPENDPATH += Singleton \
        WMI
INCLUDEPATH += Singleton \
        WMI

include(Singleton/Singleton.pri)
include(WMI/WMI.pri)

RESOURCES += \
    src.qrc

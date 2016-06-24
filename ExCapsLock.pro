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
    cmditem.cpp \
    cmdpalette.cpp \
    systemcmd.cpp \
    power.cpp

HEADERS  += \
    keyboardhook.h \
    cmditem.h \
    cmdpalette.h \
    systemcmd.h \
    power.h

FORMS    += cmd.ui

LIBS += -lUser32 -lAdvapi32 -lPowrProf

DEPENDPATH += Singleton \
        WMI
INCLUDEPATH += Singleton \
        WMI

include(Singleton/Singleton.pri)
include(WMI/WMI.pri)

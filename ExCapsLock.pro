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
    processretriever.cpp \
    appdetector.cpp \
    CmdItem/applauncher.cpp \
    cmditemdelegate.cpp \
    cmditemsortfilterproxymodel.cpp

HEADERS  += \
    keyboardhook.h \
    cmdpalette.h \
    shadowwidget.h \
    CmdItem/apps.h \
    CmdItem/cmditem.h \
    CmdItem/power.h \
    CmdItem/systemcmd.h \
    processretriever.h \
    appdetector.h \
    CmdItem/applauncher.h \
    cmditemdelegate.h \
    cmditemsortfilterproxymodel.h

FORMS    += cmd.ui

LIBS += -lUser32 -lAdvapi32 -lPowrProf

include(Singleton/Singleton.pri)
include(WMI/WMI.pri)

RESOURCES += \
    resource.qrc

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
    CmdItem/cmditem.cpp \
    CmdItem/power.cpp \
    CmdItem/systemcmd.cpp \
    processretriever.cpp \
    CmdItem/applauncher.cpp \
    cmditemdelegate.cpp \
    cmditemsortfilterproxymodel.cpp \
    CmdItem/appregister.cpp \
    CmdItem/applister.cpp

HEADERS  += \
    keyboardhook.h \
    cmdpalette.h \
    shadowwidget.h \
    CmdItem/cmditem.h \
    CmdItem/power.h \
    CmdItem/systemcmd.h \
    processretriever.h \
    CmdItem/applauncher.h \
    cmditemdelegate.h \
    cmditemsortfilterproxymodel.h \
    config.h \
    CmdItem/appregister.h \
    CmdItem/applister.h \
    keysequence.h

FORMS    += cmd.ui

RESOURCES += \
    resource.qrc

LIBS += -lUser32 -lAdvapi32 -lPowrProf

include(JSettings/JSettings.pri)
include(Singleton/Singleton.pri)
include(WMI/WMI.pri)

win32 {
#QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
}

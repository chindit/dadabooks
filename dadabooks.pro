#-------------------------------------------------
#
# Project created by QtCreator 2014-08-20T16:10:33
#
#-------------------------------------------------

QT       += core gui sql xml network widgets printsupport

TARGET = dadabooks
TEMPLATE = app


SOURCES += main.cpp\
        dadabooks.cpp \
    settingsmanager.cpp \
    addbook.cpp \
    sitemanager.cpp \
    editbook.cpp \
    addauteur.cpp \
    addediteur.cpp \
    xmlmanager.cpp \
    sqlmanager.cpp \
    previewbook.cpp \
    settingsdialog.cpp \
    firstlaunch.cpp \
    toolsmanager.cpp \
    imageuploaddialog.cpp

HEADERS  += dadabooks.h \
    settingsmanager.h \
    addbook.h \
    sitemanager.h \
    PluginTemplate.h \
    editbook.h \
    addauteur.h \
    addediteur.h \
    xmlmanager.h \
    sqlmanager.h \
    previewbook.h \
    settingsdialog.h \
    firstlaunch.h \
    toolsmanager.h \
    imageuploaddialog.h

FORMS    += dadabooks.ui \
    addbook.ui \
    editbook.ui \
    addauteur.ui \
    addediteur.ui \
    previewbook.ui \
    settingsdialog.ui \
    firstlaunch.ui \
    imageuploaddialog.ui

RESOURCES += \
    images.qrc

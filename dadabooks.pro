#-------------------------------------------------
#
# Project created by QtCreator 2014-08-20T16:10:33
#
#-------------------------------------------------

QT       += core gui sql xml network widgets printsupport

TARGET = dadabooks
TEMPLATE = app

#For Linux
#LIBS += -lmysqld
#For Windows
#LIBS += "C:\Program Files\MariaDB 10.1\lib\libmysql.lib"

#QTPLUGIN += qsqlmysql

SOURCES += main.cpp\
        dadabooks.cpp \
    settingsmanager.cpp \
    addauteur.cpp \
    sitemanager.cpp \
    ui/editbook.cpp \
    addediteur.cpp \
    xmlmanager.cpp \
    sqlmanager.cpp \
    previewbook.cpp \
    settingsdialog.cpp \
    ui/firstlaunch.cpp \
    toolsmanager.cpp \
    ui/imageuploaddialog.cpp \
    lenddialog.cpp \
    updater.cpp \
    searchdialog.cpp \
    tools/settings.cpp \
    ui/apidialog.cpp \
    tools/logger.cpp \
    tools/pluginloader.cpp \
    entity/movieconverter.cpp \
    entity/movie.cpp \
    ui/storageselectiondialog.cpp \
    tools/storage.cpp \
    entity/baseentity.cpp \
    tools/stringtools.cpp \
    ui/additemdialog.cpp \
    tools/networkmanager.cpp

HEADERS  += dadabooks.h \
    settingsmanager.h \
    sitemanager.h \
    PluginTemplate.h \
    ui/editbook.h \
    addauteur.h \
    addediteur.h \
    xmlmanager.h \
    sqlmanager.h \
    previewbook.h \
    settingsdialog.h \
    ui/firstlaunch.h \
    toolsmanager.h \
    ui/imageuploaddialog.h \
    lenddialog.h \
    updater.h \
    searchdialog.h \
    tools/settings.h \
    config/config.h \
    ui/apidialog.h \
    tools/logger.h \
    tools/exceptioninterface.h \
    tools/pluginloader.h \
    entity/movieconverter.h \
    entity/movie.h \
    plugins/storageplugin.h \
    plugins/thirdpartyplugin.h \
    ui/storageselectiondialog.h \
    tools/storage.h \
    entity/baseentity.h \
    tools/stringtools.h \
    ui/additemdialog.h \
    tools/networkmanager.h

FORMS    += dadabooks.ui \
    ui/editbook.ui \
    addauteur.ui \
    addediteur.ui \
    previewbook.ui \
    settingsdialog.ui \
    ui/firstlaunch.ui \
    ui/imageuploaddialog.ui \
    lenddialog.ui \
    searchdialog.ui \
    ui/apidialog.ui \
    ui/storageselectiondialog.ui \
    ui/additemdialog.ui

RESOURCES += \
    images.qrc

TRANSLATIONS = languages/dadabooks_fr.ts

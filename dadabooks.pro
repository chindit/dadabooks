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
    addbook.cpp \
    sitemanager.cpp \
    editbook.cpp \
    addauteur.cpp \
    addediteur.cpp \
    xmlmanager.cpp \
    sqlmanager.cpp \
    previewbook.cpp \
    settingsdialog.cpp \
    ui/firstlaunch.cpp \
    toolsmanager.cpp \
    imageuploaddialog.cpp \
    lenddialog.cpp \
    updater.cpp \
    searchdialog.cpp \
    tools/settings.cpp \
    ui/apidialog.cpp \
    tools/logger.cpp \
    tools/pluginloader.cpp \
    storage/xmlstorage.cpp \
    entity/movieentity.cpp

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
    ui/firstlaunch.h \
    toolsmanager.h \
    imageuploaddialog.h \
    lenddialog.h \
    updater.h \
    searchdialog.h \
    tools/settings.h \
    config/config.h \
    ui/apidialog.h \
    tools/logger.h \
    tools/exceptioninterface.h \
    plugins/storagedefinition.h \
    plugins/thirdpartydefinition.h \
    tools/pluginloader.h \
    storage/xmlstorage.h \
    entity/movieentity.h

FORMS    += dadabooks.ui \
    addbook.ui \
    editbook.ui \
    addauteur.ui \
    addediteur.ui \
    previewbook.ui \
    settingsdialog.ui \
    ui/firstlaunch.ui \
    imageuploaddialog.ui \
    lenddialog.ui \
    searchdialog.ui \
    ui/apidialog.ui

RESOURCES += \
    images.qrc

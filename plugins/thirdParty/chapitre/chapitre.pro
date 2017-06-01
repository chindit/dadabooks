#-------------------------------------------------
#
# Project created by QtCreator 2013-08-26T19:57:32
#
#-------------------------------------------------

QT       += core network

TARGET = chapitre
TEMPLATE = lib
CONFIG += plugin

#DESTDIR = $$[QT_INSTALL_PLUGINS]/
DESTDIR = /home/david/Linux/david/Documents/Programmation/Compilations/dadabooks/Debug/plugins/

SOURCES += chapitre.cpp

HEADERS += chapitre.h
OTHER_FILES += \
    chapitre.json

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

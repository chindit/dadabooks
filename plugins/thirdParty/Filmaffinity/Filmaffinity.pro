#-------------------------------------------------
#
# Project created by QtCreator 2014-09-14T13:23:20
#
#-------------------------------------------------

QT       += core network

TARGET = filmaffinity
TEMPLATE = lib
CONFIG += plugin

DESTDIR = /home/david/Linux/david/Documents/Programmation/Compilations/dadabooks/Debug/plugins/

SOURCES += filmaffinity.cpp

HEADERS += filmaffinity.h
OTHER_FILES += \
    Filmaffinity.json

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

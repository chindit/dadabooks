#-------------------------------------------------
#
# Project created by QtCreator 2014-12-07T13:55:05
#
#-------------------------------------------------

QT       += core network

TARGET = moviecovers
TEMPLATE = lib
CONFIG += plugin

DESTDIR = /home/david/Linux/david/Documents/Programmation/Compilations/dadabooks/Debug/plugins/

SOURCES += moviecovers.cpp

HEADERS += moviecovers.h
OTHER_FILES += moviecovers.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}

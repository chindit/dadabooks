#-------------------------------------------------
#
# Project created by QtCreator 2014-11-18T21:42:54
#
#-------------------------------------------------

QT       += core network

TARGET = Allocine
TEMPLATE = lib
CONFIG += plugin

DESTDIR = /home/david/Linux/david/Documents/Programmation/Compilations/dadabooks/Debug/plugins/

SOURCES += allocine.cpp

HEADERS += allocine.h
OTHER_FILES += Allocine.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}

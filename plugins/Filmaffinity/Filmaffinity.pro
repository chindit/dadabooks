#-------------------------------------------------
#
# Project created by QtCreator 2014-09-14T13:23:20
#
#-------------------------------------------------

QT       += core network

TARGET = filmaffinity
TEMPLATE = lib
CONFIG += plugin

#DESTDIR = $$[QT_INSTALL_PLUGINS]/
DESTDIR = /tmp/dadabooks_Debug/

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

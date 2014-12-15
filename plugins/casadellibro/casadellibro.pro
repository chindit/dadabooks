#-------------------------------------------------
#
# Project created by QtCreator 2013-09-03T20:40:12
#
#-------------------------------------------------

QT       += core network

TARGET = casadellibro
TEMPLATE = lib
CONFIG += plugin

DESTDIR = /home/david/Linux/david/Documents/Programmation/Compilations/dadabooks/Debug/plugins/

SOURCES += \
    casadellibro.cpp

HEADERS += \
    casadellibro.h
OTHER_FILES += casadellibro.json

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

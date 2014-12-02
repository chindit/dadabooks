#-------------------------------------------------
#
# Project created by QtCreator 2013-08-30T11:37:57
#
#-------------------------------------------------

QT       += core network

TARGET = amazon
TEMPLATE = lib
CONFIG += plugin

DESTDIR = /home/david/Linux/david/Documents/Programmation/Compilations/dadabooks/Debug/plugins/

SOURCES += \
    amazon.cpp

HEADERS += \
    amazon.h
OTHER_FILES += amazon.json

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

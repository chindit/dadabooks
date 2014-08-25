#-------------------------------------------------
#
# Project created by QtCreator 2013-08-30T11:37:57
#
#-------------------------------------------------

QT       += core network

TARGET = amazon
TEMPLATE = lib
CONFIG += plugin

DESTDIR = /tmp/dadabooks_Debug/

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

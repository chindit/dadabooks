#-------------------------------------------------
#
# Project created by QtCreator 2013-09-03T20:40:12
#
#-------------------------------------------------

QT       += core sql

TARGET = casadellibro
TEMPLATE = lib
CONFIG += plugin

DESTDIR = /tmp/dadabooks_Debug/

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

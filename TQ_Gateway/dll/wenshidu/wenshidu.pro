#-------------------------------------------------
#
# Project created by QtCreator 2015-10-20T15:00:08
#
#-------------------------------------------------

QT       -= gui

TARGET = wenshidu
TEMPLATE = lib

DEFINES += WENSHIDU_LIBRARY

SOURCES += wenshidu.cpp

HEADERS += wenshidu.h\
        wenshidu_global.h \
    adaptorstrategy.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

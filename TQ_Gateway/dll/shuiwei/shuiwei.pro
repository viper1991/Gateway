#-------------------------------------------------
#
# Project created by QtCreator 2015-08-17T10:54:41
#
#-------------------------------------------------

QT       -= gui

TARGET = shuiwei
TEMPLATE = lib

DEFINES += SHUIWEI_LIBRARY

SOURCES += shuiwei.cpp

HEADERS += shuiwei.h\
        shuiwei_global.h \
    adaptorstrategy.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

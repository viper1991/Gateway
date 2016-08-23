#-------------------------------------------------
#
# Project created by QtCreator 2015-09-25T12:07:28
#
#-------------------------------------------------

QT       -= gui

TARGET = guangzhao
TEMPLATE = lib

DEFINES += GUANGZHAO_LIBRARY

SOURCES += guangzhao.cpp

HEADERS += guangzhao.h\
        guangzhao_global.h \
    adaptorstrategy.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

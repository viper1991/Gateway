#-------------------------------------------------
#
# Project created by QtCreator 2015-08-07T11:41:01
#
#-------------------------------------------------

QT       -= gui

TARGET = testConnAdaptor2
TEMPLATE = lib

DEFINES += TESTCONNADAPTOR2_LIBRARY

SOURCES += testconnadaptor2.cpp

HEADERS += testconnadaptor2.h\
        testconnadaptor2_global.h \
    adaptorstrategy.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

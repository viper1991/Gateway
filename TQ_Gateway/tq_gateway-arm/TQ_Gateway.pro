#-------------------------------------------------
#
# Project created by QtCreator 2015-07-22T09:42:21
#
#-------------------------------------------------

QT       += core network serialport xml

QT       -= gui

TARGET = TQ_Gateway
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Common/parameter.cpp \
    HardwareConnection/connections/tcpconnection.cpp \
    HardwareConnection/connections/udpconnection.cpp \
    DataUpload/mqtt/mqttconnection.cpp \
    interface/connection.cpp \
    interface/connectionadaptor.cpp \
    DataUpload/mqtt/mqttadaptor.cpp \
    mainthread.cpp \
    config/config.cpp \
    interface/factories/connectionfactory.cpp \
    DataUpload/mqtt/testupload.cpp \
    HardwareConnection/connections/serialconnection.cpp \
    HardwareConnection/connections/rs485connecction_.cpp \
    HardwareConnection/connections/rs485connection.cpp

HEADERS += \
    Common/parameter.h \
    Common/threadsafequeue.h \
    Common/commom.h \
    HardwareConnection/connections/tcpconnection.h \
    HardwareConnection/connections/udpconnection.h \
    DataUpload/mqtt/mqttconnection.h \
    interface/connection.h \
    interface/connectionadaptor.h \
    DataUpload/mqtt/mqttadaptor.h \
    mainthread.h \
    config/config.h \
    interface/factories/connectionfactory.h \
    adaptorstrategy.h \
    DataUpload/mqtt/testupload.h \
    HardwareConnection/connections/serialconnection.h \
    HardwareConnection/connections/rs485connecction_.h \
    HardwareConnection/connections/rs485connection.h \
    Common/singleton.h

INCLUDEPATH+="3rd/mqtt"

win32 {
    LIBS+="3rd/mqtt/mosquitto.lib"
    LIBS+="3rd/mqtt/mosquittopp.lib"
}

unix:!macx: LIBS += -L$$PWD/3rd/mqtt/ -lmosquitto

unix:!macx: LIBS += -L$$PWD/3rd/mqtt/ -lmosquittopp

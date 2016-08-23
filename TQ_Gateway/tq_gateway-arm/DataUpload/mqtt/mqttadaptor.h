
#ifndef MQTTCONNECTOR_H
#define MQTTCONNECTOR_H


#include "interface/connectionadaptor.h""
#include "mqttconnection.h"
#include "Common/commom.h"

class MQTTAdaptor:public ConnectionAdaptor
{
public:
    MQTTAdaptor(const Parameter &parameter);


    // ConnectionAdaptor interface
public:
    int readCmd(){return 1;}
    int procData(QByteArray &in, QString &out){return 1;}
    int poll();
    QString adaptorDesc();

protected:
    //Connection *onInitConnection(Parameter & para);
    void onError(int erorCode);
    void onStoped();
    void onStarted();

};

#endif

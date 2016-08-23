#include "mqttadaptor.h"


extern ThreadSafeQueue<QString> uploadData;
extern QString ip;

MQTTAdaptor::MQTTAdaptor(const Parameter &parameter):
    ConnectionAdaptor(parameter)
{

}

int MQTTAdaptor::poll()
{
    QString msg;
    uploadData.waitAndPop(msg);
    int i = msg.indexOf("}");
    msg.insert(i,"[IP:"+ip+"]");
    connection->sendData(msg.toLocal8Bit(),parameters);
    qDebug()<<"MQTT: "<<msg;
    return 1;
}

//Connection *MQTTAdaptor::onInitConnection(Parameter &para)
//{
//    Q_UNUSED(para);
//    QString mqttid=parameters.getValue("mqttId").toString();

//    if(mqttid.isEmpty()){
//        int intId=qrand()%10000;
//        mqttid=QString(intId);
//    }
//    char* charId=new char[mqttid.size()+1];
//    memcpy_s(charId,mqttid.size(),mqttid.toStdString().c_str(),mqttid.size());
//    charId[mqttid.size()]=0;
//    return new MQTTConnection(parameters,charId,this->parent());
//}

void MQTTAdaptor::onError(int erorCode)
{
    qDebug()<<this->getAdaptorID()<<" error :"<<erorCode;
}

void MQTTAdaptor::onStoped()
{
    qDebug()<<this->getAdaptorID()<<" connection stoped!";
}

void MQTTAdaptor::onStarted()
{
    qDebug()<<this->getAdaptorID()<<" connection started!";
}

QString MQTTAdaptor::adaptorDesc()
{
    return "Adaptor thread for MQTT connection.\nIn order to upload data.\nDO NOT STOP!";
}

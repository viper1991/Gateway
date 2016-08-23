#include "connectionfactory.h"
#include "Common/singleton.h"
#include "HardwareConnection/connections/udpconnection.h"
#include "HardwareConnection/connections/tcpconnection.h"
#include "HardwareConnection/connections/serialconnection.h"
#include "HardwareConnection/connections/rs485connection.h"
#include "DataUpload/mqtt/mqttconnection.h"

extern RS485Connecction_ * rs485;
extern Parameter rs485_conf;

ConnectionFactory::ConnectionFactory()
{

}

Connection *ConnectionFactory::createConnection(Parameter parameter)
{
    int i_type = parameter.getValue("connectionType").toInt();
    if(0==i_type){
        return NULL;
    }
    HardwareConnection::CONNECTION_TYPE type = (HardwareConnection::CONNECTION_TYPE)i_type;
    Connection* conn;
    switch (type) {
    case HardwareConnection::UDP:
        conn = new UDPConnection(parameter);
        break;
    case HardwareConnection::TCP:
        conn = new TCPConnection(parameter);
        break;
    case HardwareConnection::SERIAL_232:
        conn = new SerialConnection(parameter);
        break;
    case HardwareConnection::SERIAL_485:
        if(parameter.getValue("address").toString()=="p2p"){
            conn = new SerialConnection(parameter);
        }else{
            bool isnew = false;
            if(rs485==NULL){
                rs485=new RS485Connecction_();
                isnew = true;
            }
            if(isnew){
                if(!rs485->setParameter(rs485_conf)){
                      qDebug()<<"RS485 connection init error";
                return NULL;
                }
                qDebug()<<"RS485 connection created";
            }
            conn = new RS485Connection(parameter);
        }
        break;
    case HardwareConnection::MQTT:
    {
        QString mqttid;
        if(parameter.getValue("mqttid").isNull()){
            int intId=qrand()%10000;
            mqttid=QString(intId);
        }else{
            mqttid=parameter.getValue("mqttid").toString();
        }
        char* charId=new char[mqttid.size()+1];
        memcpy(charId,mqttid.toStdString().c_str(),mqttid.size());
        charId[mqttid.size()]=0;
        conn = new MQTTConnection(parameter,charId);
    }
        break;
    default:
        conn = NULL;
        break;
    }
    return conn;
}

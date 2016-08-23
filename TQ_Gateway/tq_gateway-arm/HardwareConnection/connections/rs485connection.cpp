#include "rs485connection.h"
#include "rs485connecction_.h"
#include "Common/singleton.h"

extern RS485Connecction_* rs485;

RS485Connection::RS485Connection(const Parameter &ps, QObject *parent):
    Connection(ps,parent)
{
    this->sharedConnection = rs485;
    QObject::connect(this,SIGNAL(connect_(char)),sharedConnection,SLOT(connect(char)));
    QObject::connect(this,SIGNAL(connectionDestory(char)),sharedConnection,SLOT(disConnect(char)));
    QObject::connect(this,SIGNAL(send(QByteArray)),sharedConnection,SLOT(sendData(QByteArray)));
    QObject::connect(sharedConnection,SIGNAL(newData(QByteArray,char)),this,SLOT(recvData(QByteArray,char)));
    QObject::connect(sharedConnection,SIGNAL(disConnected()),this,SLOT(connectionLost()));
}

RS485Connection::~RS485Connection()
{
}

int RS485Connection::connect()
{
    QString addr_string = this->parameters.getValue("address").toString();
    bool ok;
    this->addr = addr_string.toUShort(&ok,16);
    if(!ok){
        qDebug()<<"Address "<<addr_string<<" error";
        return HC_CONNECTION_ERROR;
    }
    qDebug()<<"Address "<<addr_string<<" registint";
    rs485->registLoginConnection(addr,&recvMsgQueue);
    qDebug()<<"Address "<<addr_string<<" registed";
    emit connect_(addr);
    return HC_CONNECTION_SUCCESS;
}

int RS485Connection::sendData(const QByteArray &bytes, const Parameter &aim)
{
    Q_UNUSED(aim);
    //emit send(bytes);
    qDebug()<<"request to write";
    //rs485SendData.push(bytes);
    this->sharedConnection->sendData(bytes);
    return 0;
}

void RS485Connection::disConnect()
{
    this->isConnected=false;
    emit connectionDestory(this->addr);
}

void RS485Connection::recvData(QByteArray data, char addr)
{
    if(addr!=this->addr){
        return;
    }
    this->recvMsgQueue.push(data);
}

void RS485Connection::connectionLost()
{
    emit conn_disconnected();
}


#include "udpconnection.h"

UDPConnection::UDPConnection(const Parameter &ps, QObject *parent):
    Connection(ps,parent)
{
    this->socket = new QUdpSocket(this->parent());
    bool ok = true;
    int p = ps.getValue("bindport").toInt(&ok);
    if(ok){
        socket->bind(QHostAddress::LocalHost,p);
    }
    else{
        socket->bind(QHostAddress::LocalHost,8888);
    }
}

UDPConnection::~UDPConnection()
{
    if(NULL!=this->socket){
        socket->deleteLater();
    }
}

int UDPConnection::connect()
{
   QObject::connect(socket,SIGNAL(readyRead()),this,SLOT(dataIncome()));
   qDebug()<<"UDP: ready";
   this->isConnected=true;
   return 1;
}

int UDPConnection::sendData(const QByteArray &bytes, const Parameter &aim)
{
    QString ip = aim.getValue("IP").toString();
    int port = aim.getValue("port").toInt();
    if(ip.isEmpty()||(0==port)){
        return -1;
    }
    return this->socket->writeDatagram(bytes,QHostAddress(ip),port);
}

void UDPConnection::readData()
{
    if(socket->hasPendingDatagrams()==true)
    {
        QByteArray datagram;  //接收字节数组
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size());
        this->recvMsgQueue.push(datagram);
    }
}

//void UDPConnection::dataIncome()
//{
//    readData();
//}


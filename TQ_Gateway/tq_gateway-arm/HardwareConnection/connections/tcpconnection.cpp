#include "tcpconnection.h"
#include <QHostAddress>

static QByteArray temp;

TCPConnection::TCPConnection(const Parameter &ps, QObject *parent):
    Connection(ps,parent)
{
    this->IP=ps.getValue("IP").toString();
    this->port=ps.getValue("port").toInt();
    this->tcpSocket = new QTcpSocket();
    bool ok = true;
    int p = ps.getValue("bindport").toInt(&ok);
    if(ok){
        tcpSocket->bind(QHostAddress::LocalHost,p);
    }
    else{
        tcpSocket->bind(QHostAddress::LocalHost,9999);
    }
}

TCPConnection::~TCPConnection()
{
    if(NULL != this->tcpSocket){
        tcpSocket->deleteLater();
    }
}

int TCPConnection::connect()
{
    QObject::connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataIncome()));
    QObject::connect(tcpSocket,SIGNAL(connected()),this,SLOT(tcpConnected()));
    QObject::connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(tcpDisconnected()));
    qDebug()<<"TCP: connecting...";
    this->tcpSocket->connectToHost(this->IP,this->port);
    if(this->tcpSocket->waitForConnected(5000)){
        this->isConnected=true;
        return 1;
    }
    this->isConnected=false;
    return 0;
}

void TCPConnection::disConnect()
{
    this->tcpSocket->disconnectFromHost();
    this->isConnected=false;
    qDebug()<<"TCP: disconnect called";
}

int TCPConnection::sendData(const QByteArray &bytes, const Parameter &aim)
{
    Q_UNUSED(aim);
    if(isConnected){
        tcpSocket->waitForBytesWritten();
        return tcpSocket->write(bytes);
    }
    else{
        return -1;
    }
}

void TCPConnection::readData()
{
    QObject::disconnect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataIncome()));
    temp.append(tcpSocket->readAll());
    while(tcpSocket->waitForReadyRead(100)){
        temp.append(tcpSocket->readAll());
    }
    qDebug()<<"Read data:"<<temp;
    recvMsgQueue.push(temp);
    temp.clear();
    QObject::connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataIncome()));
}

void TCPConnection::dataIncome()
{
    readData();
}

void TCPConnection::tcpConnected()
{
    qDebug()<<"TCP: connected signal";
    this->isConnected=true;
}

void TCPConnection::tcpDisconnected()
{
    qDebug()<<"TCP: disconnected signal";
    this->isConnected=false;
    emit conn_disconnected();
}



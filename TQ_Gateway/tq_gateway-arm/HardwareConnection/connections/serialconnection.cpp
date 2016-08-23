#include "serialconnection.h"

static QByteArray temp;

SerialConnection::SerialConnection(const Parameter &ps, QObject *parent):
    Connection(ps,parent)
{
    QString serial_name = ps.getValue("port").toString();
    this->serial = new QSerialPort(serial_name);
    int bondrate = ps.getValue("boundrate").toInt();
    int databit = ps.getValue("databit").toInt();
    int stopbit = ps.getValue("stopbit").toInt();
    int parity = ps.getValue("parity").toInt();
    int flowcontrol = ps.getValue("flowcontrol").toInt();
    serial->setBaudRate((QSerialPort::BaudRate)bondrate);
    serial->setDataBits((QSerialPort::DataBits)databit);
    serial->setStopBits((QSerialPort::StopBits)stopbit);
    serial->setParity((QSerialPort::Parity)parity);
    serial->setFlowControl((QSerialPort::FlowControl)flowcontrol);
    QObject::connect(serial,SIGNAL(readyRead()),this,SLOT(dataIncome()));
    temp.clear();
}

SerialConnection::~SerialConnection()
{
    if(NULL!=serial){
        serial->deleteLater();
    }
}

int SerialConnection::connect()
{
    if(serial->open(QSerialPort::ReadWrite)){
        this->isConnected = true;
        return HC_CONNECTION_SUCCESS;
    }else{
        this->isConnected = false;
        return HC_SERIAL_INIT_ERROR;
    }
}

void SerialConnection::disConnect()
{
    serial->clear();
    serial->close();
    this->isConnected = false;
}

int SerialConnection::sendData(const QByteArray &bytes, const Parameter &aim)
{
    Q_UNUSED(aim);
    int rs = serial->write(bytes);
    serial->waitForBytesWritten(-1);
    return rs;
}

void SerialConnection::readData()
{
    QObject::disconnect(serial,SIGNAL(readyRead()),this,SLOT(dataIncome()));
    temp.append(serial->readAll());
    while(serial->waitForReadyRead(100)){
        temp.append(serial->readAll());
    }
    qDebug()<<"Read data:"<<temp;
    recvMsgQueue.push(temp);
    temp.clear();
    QObject::connect(serial,SIGNAL(readyRead()),this,SLOT(dataIncome()));
}

void SerialConnection::dataIncome()
{
    readData();
}

#include "rs485connecction_.h"
#include <QTimer>

static QByteArray temp;

bool RS485Connecction_::setParameter(const Parameter &ps)
{
    this->isConnected = false;
    this->ps=ps;
    QString serial_name = ps.getValue("port").toString();
    this->serial = new QSerialPort(serial_name);
    int bondrate = ps.getValue("boundrate").toInt();
    int databit = ps.getValue("databit").toInt();
    int stopbit = ps.getValue("stopbit").toInt();
    int parity = ps.getValue("parity").toInt();
    int flowcontrol = ps.getValue("flowcontrol").toInt();
    int interval = ps.getValue("interval").toInt();
    serial->setBaudRate((QSerialPort::BaudRate)bondrate);
    serial->setDataBits((QSerialPort::DataBits)databit);
    serial->setStopBits((QSerialPort::StopBits)stopbit);
    serial->setParity((QSerialPort::Parity)parity);
    serial->setFlowControl((QSerialPort::FlowControl)flowcontrol);
    QObject::connect(serial,SIGNAL(readyRead()),this,SLOT(dataIncome()));
    temp.clear();
    if(HC_CONNECTION_SUCCESS!= connect_()){
        emit disConnected();
        return false;
    }
    this->timer = new QTimer(this);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(trySend()));
    if(0==interval){
        interval=1000;
    }
    timer->start(interval);
    return true;
}

RS485Connecction_::~RS485Connecction_()
{
    if(NULL!=serial){
        serial->deleteLater();
    }
}

void RS485Connecction_::readData()
{
    //暂时断开信号槽关联，避免循环触发槽函数导致接收数据不全
    QObject::disconnect(serial,SIGNAL(readyRead()),this,SLOT(dataIncome()));

    serial->waitForReadyRead(200);
    temp.append(serial->readAll());
    int size = temp.at(2)+5;
    qDebug()<<"size:"<<size<<" R1:"<<temp;
    int count=1;
    bool flag = true;
    while(temp.size()<size){
        serial->waitForReadyRead(200);
        temp.append(serial->readAll());
        qDebug()<<"Rn:"<<temp<<" size:"<<temp.size();
        count++;
        if(count>=10){
            flag = false;
            break;
        }
    }
    if(size != temp.size()){
        flag=false;
    }
    if(flag){
        char addr = temp.at(0);
        if(map.contains(addr))
        {
            map.value(addr)->push(temp);
        }
    }else{
        mutex.lock();
        map.clear();
        sdata.clear();
        mutex.unlock();
        emit disConnected();
    }

    //清临时变量，准备下次接收
    temp.clear();

    QObject::connect(serial,SIGNAL(readyRead()),this,SLOT(dataIncome()));
}

int RS485Connecction_::sendData(const QByteArray &bytes)
{
    sdata.push(bytes);
    return 1;
}

int RS485Connecction_::sendData_(const QByteArray &bytes)
{
    serial->write(bytes);
    serial->waitForBytesWritten(-1);
    return 1;
}

void RS485Connecction_::disConnect(char addr)
{
    mutex.lock();
    map.remove(addr);
    mutex.unlock();
//    if(map.isEmpty()){
//        allDisConnect();
//    }
}

void RS485Connecction_::connect(char addr)
{
    Q_UNUSED(addr);
    mutex.lock();
    if((!isConnected) && (!map.isEmpty())){
        this->connect_();
    }
    mutex.unlock();
}

void RS485Connecction_::dataIncome()
{
    readData();
}

void RS485Connecction_::trySend()
{
    QByteArray data;
    if(sdata.tryPop(data)){
        sendData_(data);
    }
}

int RS485Connecction_::connect_()
{
    qDebug()<<"rs485 physis connecting..";
    if(serial->open(QSerialPort::ReadWrite)){
        this->isConnected = true;
        return HC_CONNECTION_SUCCESS;
    }else{
        this->isConnected = false;
        return HC_SERIAL_INIT_ERROR;
    }
    qDebug()<<"timer start";
}

void RS485Connecction_::allDisConnect()
{
    mutex.lock();
    map.clear();
    mutex.unlock();
    sdata.clear();
    serial->close();
    this->isConnected = false;
    this->serial->deleteLater();
    timer->stop();
    timer->deleteLater();
    this->deleteLater();
}


#ifndef CONNECTION_H
#define CONNECTION_H

#include "Common/commom.h"

#include <QObject>
#include <QQueue>
#include <QByteArray>
#include <QDebug>
#include <QThread>

class Connection:public QObject
{
    Q_OBJECT
public:

    Connection(const Parameter & para,QObject * parent=0);
    virtual ~Connection(){}

    //建立通信连接，失败后会返回错误码
    virtual int connect()=0;

    //数据发送
    virtual int sendData(const QByteArray & bytes,const Parameter & aim)=0;

    //获取数据，阻塞函数，默认实现：直接从缓冲队列取数据
    virtual int getData(QByteArray & bytes, long time);

    //链接类型
    virtual HardwareConnection::CONNECTION_TYPE connectionType()=0;

    virtual bool waitForConnected();

    bool is_Connected(){return isConnected;}


protected:
    //根据具体连接，确定如何读取原始数据，读到的数据放入缓冲队列
    virtual void readData()=0;

    //链接标识
    bool isConnected;

    //接收数据缓存队列
    ThreadSafeQueue<QByteArray> recvMsgQueue;

    //连接参数
    Parameter parameters;

signals:
    void conn_disconnected();
    void restart();

public slots:
    virtual void dataIncome(){readData();}
    virtual void tcpConnected(){}
    virtual void tcpDisconnected(){}
    //断开连接
    virtual void disConnect()=0;

protected slots:
     //消息发送槽函数
     virtual void onSend(const QByteArray & bytes,const Parameter & aim){
        this->sendData(bytes,aim);
    }
};

#endif // CONNECTION_H

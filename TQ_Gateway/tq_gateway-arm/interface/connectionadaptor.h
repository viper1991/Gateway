#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>

#include "connection.h"
#include "adaptorstrategy.h"

#define DATA_READ_ERROR -2
#define CRC_CHECK_ERROR -1
#define DATA_PROC_ERROR -4
#define POLL_SUCCESS 1
#define POLL_NEED_MORE_DATA -5
#define DATA_READ_CMD_ERROR -3
#define POLL_TIME_OUT 9

//全局变量定义
extern ThreadSafeQueue<QString> uploadData;

class ConnectionAdaptor:public QThread
{
    Q_OBJECT
public:
    ConnectionAdaptor(const Parameter & para, int pollTimeOut=5000,QObject * obj=0);
    virtual ~ConnectionAdaptor();

    // 发送字符串消息
    int send(const QString & strMsg,const Parameter & aim);
    //发送字节流消息
    int send(const QByteArray & bytes,const Parameter & aim);

    //开启工作线程
    virtual void startWork();
    //关闭工作线程
    void stopWork();

    //循环处理数据函数
    virtual int poll();

    //发送读取指令，若程序被动接收，则添加空实现
    virtual int readCmd(){
        emit sendData(stgy->readCmd(),parameters);
        return 1;
    }
    //处理数据
    virtual int procData(QByteArray & in,QString & out){return stgy->procData(in,out);}

    //获取连接器id
    QString getAdaptorID() const;
    //获取连接类型
    HardwareConnection::CONNECTION_TYPE connectionType();

    QDateTime getStartTime() const;

    virtual QString adaptorDesc(){return stgy->adaptorDesc();}

    void setStrategy(AdaptorStrategy* as);

signals:
    //连接成功信号
    void started(QString id);
    //连接断开信号
    void stoped(QString id);
    //发送消息
    void sendData(QByteArray data,Parameter aim);
    //上报连接断开
    void conn_disconnected(QString id);
    void conn_init_error(QString id);
    void force_disconnect();

public slots:
    void run();
    void forceStop();
    void conn_disconnected();

    void restart(QString id){
        if(id==this->getAdaptorID()){
             qDebug()<<id<<" restarting";
             this->startWork();
        }
    }

protected:
    bool isRuning;   //标志工作线程是否开启

    //链接抽象类指针，具体实例通过 onInitConnection 返回
    Connection * connection;
    Parameter parameters; //连接参数

    //初始化连接,返回实例化的Connection指针
    Connection* onInitConnection(Parameter & para);
    //错误处理
    virtual void onError(int erorCode){return stgy->onError(erorCode);}
    //数据接收已经停止
    virtual void onStoped(){return stgy->onStoped();}
    //数据接收已经开始
    virtual void onStarted(){return stgy->onStarted();}
    //数据发送完毕
    virtual int onSended(int rs){
        Q_UNUSED(rs);
        return 0;
    }

private:
    QDateTime startTime;
    QString adaptorID;
    int  pollTimeOut;  // 消息获取的超时时间,ms
    QMutex mutex;

    AdaptorStrategy * stgy;
};

#endif // CONNECTOR_H

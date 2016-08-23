#ifndef MQTTCONNECTION_H
#define MQTTCONNECTION_H

#include "interface/connection.h"

#include "mosquittopp.h"

#include <QObject>
#include <QMutex>
#include <QMutexLocker>

//消息内容的最大长度
#define EC_MQTTCONNECTION_DEF_BUFF_LENGTH 1024

//消息topic的最大长度
#define EC_MQTTCONNECTION_TOPICNAME_LENGTH 64

//topic与内容的分割符号
#define EC_MQTTCONNECTION_T_C_SPLIT "@"

typedef mosqpp::mosquittopp QMosquitto ;


class MQTTConnection : public Connection,public QMosquitto
{
    Q_OBJECT
public:
    explicit MQTTConnection(const Parameter & para, char * id, QObject * parent = 0);

    //重写Connection的方法

    //建立连接
    int connect();

    //断开连接
    void disConnect();

    //发送数据
    int sendData(const QByteArray &bytes,const Parameter &aim);

    //连接类型
    HardwareConnection::CONNECTION_TYPE connectionType(){
        return  HardwareConnection::MQTT;
    }

protected:
    void readData();

    //初始化主题订阅
    void initSub();

private:
    typedef struct mosquitto_message QMosquittoMessage;

    //重写mosquittopp的方法

    //连接成功
    void on_connect(int rc);

    //获取到消息
    void on_message(const QMosquittoMessage *message);

    //连接断开
    void on_disconnect(int rc);

    //日志输出
    void on_log(int level, const char *str);

    //出错
    void on_error();

    bool loop_started;
    bool loop_start_support;

};



#endif // MQTTCONNECTION_H




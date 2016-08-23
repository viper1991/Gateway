#include "mqttconnection.h"

#include <QDebug>
#include <QThread>

static QMutex mosMutex;
static int mosquittoInit=false;

/**
 * @brief 初始化mosquoitto运行库,运行过程中只需要初始化一次
 */
void initMosquittoLib(){

    QMutexLocker locker(&mosMutex);

    if(mosquittoInit==true){
        return;
    }

    mosqpp::lib_init();
    mosquittoInit=true;

}

MQTTConnection::MQTTConnection(const Parameter &para, char*id, QObject *parent) :
    Connection(para,parent),
    QMosquitto(id),
    loop_started(false),
    loop_start_support(true)
{
    initMosquittoLib();
}

int MQTTConnection::connect()
{
    //从配置中获取远程主机地址通信端口
    QString host=parameters.getValue("host").toString();
    int port=parameters.getValue("port").toInt();
    int keepalive=parameters.getValue("keepalive").toInt();

    //建立MQTT通信连接
    const char* c_host = host.toStdString().c_str();
    int rs=QMosquitto::connect(c_host,port,keepalive);

    qDebug("[MQTT] connect to %s:%d",host.toStdString().c_str(),port);

    if(MOSQ_ERR_SUCCESS == rs){
        return HC_CONNECTION_SUCCESS;
    }
    else{
        return HC_CONNECTION_ERROR;
    }
//    return 1;
}

void MQTTConnection::disConnect()
{
    if(loop_start_support && loop_started){
        QMosquitto::loop_stop(true);
    }
    QMosquitto::disconnect();
}

int MQTTConnection::sendData(const QByteArray &bytes, const Parameter &aim)
{
    //从发送参数中获取消息的主题
    QString topic=aim.getValue("topic").toString();

    //发布消息
    int result=QMosquitto::publish(NULL,topic.toStdString().c_str(),bytes.size(),bytes.data());

    this->readData();
    //返回发布结果，如果发布成功，result等于发送的字节数
    return result;
}

void MQTTConnection::initSub()
{
    if(parameters.getValue("subTopics").isNull()){
        return;
    }

    //从配置参数中获取主题订阅列表
    QString subTopics=parameters.getValue("subTopics").toString();

    //多个主题之间使用|分割
    QStringList topicList=subTopics.split("|");

    //遍历主题
    foreach (QString topic, topicList) {

        //调用主题订阅接口
        int rs=QMosquitto::subscribe(NULL,topic.toStdString().c_str());
        qDebug()<<"[MQTT] sub Topic:"<<topic<<" sub result:"<<rs;
    }

}

void MQTTConnection::on_connect(int rc)
{

    qDebug()<<"[MQTT] on_connect:"<<rc;
    if(isConnected==true){
        return;
    }

    initSub();
    isConnected=true;

}

void MQTTConnection::on_message(const MQTTConnection::QMosquittoMessage *message)
{

    if(message->payloadlen==0){
        return;
    }

    //主题存储buff
    char topicBuff[EC_MQTTCONNECTION_TOPICNAME_LENGTH];
    memset(topicBuff, 0, sizeof(topicBuff));
    strcat(topicBuff,message->topic);

    //内容存储buff
    char conBuff[EC_MQTTCONNECTION_DEF_BUFF_LENGTH];
    memset(conBuff, 0, sizeof(conBuff));
    strcat(conBuff,(char*)message->payload);

    //存入消息内容
    recvMsgQueue.push(QByteArray(conBuff));

    qDebug("[MQTT]get message %s@%s",
           message->topic,
           (char*)message->payload
           );

}

void MQTTConnection::on_disconnect(int rc)
{
    Q_UNUSED(rc);
    qDebug()<<"[MQTT]on_disconnect";
    emit conn_disconnected();
}

void MQTTConnection::on_log(int level, const char *str)
{
    if(level==4||level==8){
        qDebug()<<"[MQTT]LOG: level:"<<level<<" str:"<<str;
    }
}

void MQTTConnection::on_error()
{
    qDebug()<<"[MQTT]on_error";
}

void MQTTConnection::readData()
{
    //等待数据50毫秒
    int rs=QMosquitto::loop(50);
    //如果连接已经断开，则从新连接
    if(rs!=MOSQ_ERR_SUCCESS){
        QMosquitto::reconnect();
    }
}

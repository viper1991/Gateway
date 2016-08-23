#ifndef RS485CONNECCTION__H
#define RS485CONNECCTION__H
#include "interface/connection.h"
#include <QThread>
#include <QTimer>
#include <QSerialPort>
#include <QMutex>
#include <QList>
/**
 * @brief The RS485Connecction_ class
 * 485物理连接，单例模式，处理485链路上的数据
 */
class RS485Connecction_ : public QObject
{
    Q_OBJECT
public:
    explicit RS485Connecction_(){}
    bool setParameter(const Parameter & ps);
    ~RS485Connecction_();
    void registLoginConnection(char addr,ThreadSafeQueue<QByteArray>* dataQueue){
        map.insert(addr,dataQueue);
    }

protected:
    void readData();

public slots:
    //发送数据槽函数
    int sendData(const QByteArray &bytes);
    //逻辑连接断开槽函数
    void disConnect(char addr);
    //逻辑连接建立槽函数
    void connect(char addr);
    //处理QSerialPort信号槽
    void dataIncome();
    void trySend();

signals:
    //收到新数据后通知各逻辑连接
    void newData(QByteArray data,char addr);
    //物理连接断开后通知各逻辑连接
    void disConnected();
    void dataError();

private:
    //建立物理连接
    int connect_();
    //断开物理连接
    void allDisConnect();
    int sendData_(const QByteArray &bytes);
    bool isConnected;
    QSerialPort * serial;
    QMutex mutex;
    QWaitCondition  condition;
    Parameter ps;
    QMap<char,ThreadSafeQueue<QByteArray>*> map;
    ThreadSafeQueue<QByteArray> sdata;
    QTimer * timer;
};

#endif // RS485CONNECCTION__H

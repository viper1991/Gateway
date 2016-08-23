#ifndef RS485CONNECTION_H
#define RS485CONNECTION_H
#include "interface/connection.h"
#include "rs485connecction_.h"

/**
 * @brief The RS485Connection class
 * 485逻辑连接
 */
class RS485Connection : public Connection
{
    Q_OBJECT
public:
    explicit RS485Connection(const Parameter & ps, QObject *parent=0);
    ~RS485Connection();
    // Connection interface
public:
    int connect();
    HardwareConnection::CONNECTION_TYPE connectionType(){
        return HardwareConnection::SERIAL_485;
    }

protected:
    void readData(){}

public slots:
    int sendData(const QByteArray &bytes, const Parameter &aim);
    void disConnect();
    void recvData(QByteArray data,char addr);
    void connectionLost();

signals:
    void connectionDestory(char addr);
    void connect_(char addr);
    void send(QByteArray bytes);

private:
    RS485Connecction_ * sharedConnection;
    char addr;
};

#endif // RS485CONNECTION_H

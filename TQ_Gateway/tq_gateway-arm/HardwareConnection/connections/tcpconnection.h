#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include "interface/connection.h"
#include <QTcpSocket>

class TCPConnection :public Connection
{
public:
    explicit TCPConnection(const Parameter & ps, QObject *parent=0);
    ~TCPConnection();
    // Connection interface
public:
    int connect();
    void disConnect();
    int sendData(const QByteArray &bytes, const Parameter &aim);
    HardwareConnection::CONNECTION_TYPE connectionType(){
        return HardwareConnection::TCP;
    }

protected:
    void readData();

private:
    QString IP;
    int port;
    QTcpSocket * tcpSocket;

public slots:
    void dataIncome();
    void tcpConnected();
    void tcpDisconnected();
};

#endif // TCPCONNECTION_H

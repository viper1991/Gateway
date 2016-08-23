#ifndef UDPCONNECTION_H
#define UDPCONNECTION_H

#include "interface/connection.h"
#include <QUdpSocket>

class UDPConnection : public Connection
{
public:
    explicit UDPConnection(const Parameter & ps, QObject *parent=0);
    ~UDPConnection();

    // Connection interface
public:
    int connect();
    void disConnect(){this->isConnected=false;}
    int sendData(const QByteArray &bytes, const Parameter &aim);
    HardwareConnection::CONNECTION_TYPE connectionType(){
        return HardwareConnection::UDP;
    }

protected:
    void readData();

//public slots:
//    void dataIncome();

private:
    QUdpSocket * socket;
};

#endif // UDPCONNECTION_H

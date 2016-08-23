#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H
#include "interface/connection.h"
#include <QSerialPort>

class SerialConnection : public Connection
{
public:
    explicit SerialConnection(const Parameter & ps, QObject *parent=0);
    ~SerialConnection();

    // Connection interface
public:
    int connect();
    void disConnect();
    int sendData(const QByteArray &bytes, const Parameter &aim);
    HardwareConnection::CONNECTION_TYPE connectionType(){
        return HardwareConnection::SERIAL_232;
    }

protected:
    void readData();

public slots:
    void dataIncome();

private:
    QSerialPort * serial;
};

#endif // SERIALCONNECTION_H

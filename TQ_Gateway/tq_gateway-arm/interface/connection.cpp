#include "connection.h"
#include "QTime"

Connection::Connection(const Parameter & para, QObject * parent):
    QObject(parent),
    parameters(para),
    isConnected(false)
{
}

int Connection::getData(QByteArray &bytes, long time)
{
    if(recvMsgQueue.waitAndPop(bytes,time))
    {
        return HC_CONNECTION_SUCCESS;
    }else{
        return HC_WAIT_TIMEOUT;
    }
}

bool Connection::waitForConnected()
{
    while(!isConnected){}
    return true;
}


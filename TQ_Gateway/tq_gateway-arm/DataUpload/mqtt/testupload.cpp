#include "testupload.h"

TestUpload::TestUpload(const Parameter &parameter):
    ConnectionAdaptor(parameter)
{

}

int TestUpload::poll()
{
    QString msg;
    uploadData.waitAndPop(msg);
    //connection->sendData(msg.toLocal8Bit(),parameters);
    qDebug()<<msg;
    return 1;
}

void TestUpload::startWork()
{
    start();
}


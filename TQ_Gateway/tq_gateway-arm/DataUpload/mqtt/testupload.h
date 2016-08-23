#ifndef TESTUPLOAD_H
#define TESTUPLOAD_H

#include "interface/connectionadaptor.h"

class TestUpload : public ConnectionAdaptor
{
public:
    TestUpload(const Parameter &parameter);

    // ConnectionAdaptor interface
public:
    int poll();
    int readCmd(){return 1;}
    int procData(QByteArray &in, QString &out){return 1;}
    QString adaptorDesc(){return "Test Upload";}

protected:
    void onError(int erorCode){}
    void onStoped(){}
    void onStarted(){}

    // ConnectionAdaptor interface
public:
    void startWork();
};

#endif // TESTUPLOAD_H

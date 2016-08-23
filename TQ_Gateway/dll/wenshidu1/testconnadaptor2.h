#ifndef TESTCONNADAPTOR2_H
#define TESTCONNADAPTOR2_H

#include "testconnadaptor2_global.h"
#include "adaptorstrategy.h"

extern "C" TESTCONNADAPTOR2SHARED_EXPORT int createObj(AdaptorStrategy *& st);
class TestConnAdaptor2 : public AdaptorStrategy
{

public:
    TestConnAdaptor2(){}

    // AdaptorStrategy interface
public:
    QByteArray readCmd();
    int procData(QByteArray &in, QString &out);
    QString adaptorDesc(){
        return "Test connection adaptor strategy 2 load from dll file";
    }
    void onError(int erorCode){Q_UNUSED(erorCode)}
    void onStoped(){}
    void onStarted(){}
};

#endif // TESTCONNADAPTOR2_H

#ifndef SHUIWEI_H
#define SHUIWEI_H

#include "adaptorstrategy.h"
#include "shuiwei_global.h"
#include <QList>

extern "C" SHUIWEISHARED_EXPORT int createObj(AdaptorStrategy *& st);
class Shuiwei : public AdaptorStrategy
{

public:
    Shuiwei();

    // AdaptorStrategy interface
public:
    QByteArray readCmd();
    int procData(QByteArray &in, QString &out);
    QString adaptorDesc();
    void onError(int erorCode);
    void onStoped(){}
    void onStarted(){}

private:
    double current_Val;
    double max;
    double min;

    double getReal(double in);
};

#endif // SHUIWEI_H

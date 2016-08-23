#ifndef GUANGZHAO_H
#define GUANGZHAO_H

#include "guangzhao_global.h"
#include "adaptorstrategy.h"

extern "C" GUANGZHAOSHARED_EXPORT int createObj(AdaptorStrategy *& st);
class Guangzhao : public AdaptorStrategy
{

public:
    Guangzhao(){}

    // AdaptorStrategy interface
public:
    QByteArray readCmd();
    int procData(QByteArray &in, QString &out);
    QString adaptorDesc();
    void onError(int erorCode);
    void onStoped(){}
    void onStarted(){}
};

#endif // GUANGZHAO_H

#ifndef WENSHIDU_H
#define WENSHIDU_H

#include "wenshidu_global.h"
#include "adaptorstrategy.h"

extern "C" WENSHIDUSHARED_EXPORT int createObj(AdaptorStrategy *& st);

class Wenshidu : public AdaptorStrategy
{

public:
    Wenshidu();

    // AdaptorStrategy interface
public:
    QByteArray readCmd();
    int procData(QByteArray &in, QString &out);
    QString adaptorDesc(){
        return "temp with modbus prototal";
    }
    void onError(int erorCode){Q_UNUSED(erorCode);}
    void onStoped(){}
    void onStarted(){}
};

#endif // WENSHIDU_H

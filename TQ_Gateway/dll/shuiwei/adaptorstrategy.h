#ifndef ADAPTORSTRATEGY_H
#define ADAPTORSTRATEGY_H
#include <QByteArray>
#include <QString>

class AdaptorStrategy
{
public:
    AdaptorStrategy(){}
    //发送读取指令，若程序被动接收，则添加空实现
    virtual QByteArray readCmd()=0;
    //处理数据
    virtual int procData(QByteArray & in,QString & out)=0;
    //适配器描述
    virtual QString adaptorDesc()=0;
    //错误处理
    virtual void onError(int erorCode)=0;
    //数据接收已经停止
    virtual void onStoped()=0;
    //数据接收已经开始
    virtual void onStarted()=0;
    //设置设备ID
    virtual void setId(QString id){
        this->id=id;
    }
    virtual QString getId(){
        return this->id;
    }

private:
    QString id;
};

#endif // ADAPTORSTRATEGY_H

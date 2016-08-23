#include <QCoreApplication>
#include "Common/threadsafequeue.h"
#include <QByteArray>
#include "mainthread.h"
#include <QNetworkInterface>

//全局变量定义
ThreadSafeQueue<QString> uploadData;
RS485Connecction_* rs485;
Parameter rs485_conf;
QString ip;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ip="0.0.0.0";
    QString i_name="eth0";
    QString c_name="/home/config.xml";
    for(int i = 0;i<argc;i++){
        QString t(argv[i]);
        if(t.startsWith("-i")){
            i_name=t.right(t.size()-2);
        }
        if(t.startsWith("-c")){
            c_name=t.right(t.size()-2);
        }
    }

    QNetworkInterface interface = QNetworkInterface::interfaceFromName(i_name);
    if(interface.isValid()){
        if(!interface.addressEntries().empty()){
            ip=interface.addressEntries().at(0).ip().toString();
        }
    }
    MainThread mt;
    mt.init(c_name);
    return a.exec();
}

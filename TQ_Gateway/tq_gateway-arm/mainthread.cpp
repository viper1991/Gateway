#include "mainthread.h"
#include "DataUpload/mqtt/mqttadaptor.h"

#include <QDebug>
#include <QLibrary>

extern QString ip;
extern RS485Connecction_* rs485;
extern Parameter rs485_conf;

MainThread::MainThread(QObject *parent) : QThread(parent)
{
}

MainThread::~MainThread()
{

}

void MainThread::init(QString configPath)
{
    //读取配置文件
    QString errMsg;int line;int col;
    if(FILE_PROC_SUCESS != config.readConfig(configPath,this->para,errMsg,line,col)){
        qDebug()<<"Config file read error : "<<errMsg<<" at line "<<line<<" colume "<<col;
        return;
    }
    //加载连接适配器
    QStringList keys = para.getKeys();
    bool isMQTTLoad = false;
    for(int i=0;i<keys.size();i++){
        Parameter m;
        m.setVarMap(para.getValue(keys.at(i)).toMap());
        paras.insert(keys.at(i),m);
        //加载MQTT连接
        if(keys.at(i)=="MQTT"){
            qDebug()<<"Loading adaptor "<<keys.at(i);
            paras["MQTT"].setValue("id","MQTT");
            paras["MQTT"].setValue("timeout",0);
            ConnectionAdaptor * mqtt = new MQTTAdaptor(paras.value("MQTT"));
            //ConnectionAdaptor * mqtt = new TestUpload(paras.value("MQTT"));
            connect(mqtt,SIGNAL(started(QString)),this,SLOT(adaptorStarted(QString)));
            connect(mqtt,SIGNAL(stoped(QString)),this,SLOT(adaptorStoped(QString)));
            connect(mqtt,SIGNAL(conn_disconnected(QString)),this,SLOT(conn_disconnected(QString)));
            connect(mqtt,SIGNAL(conn_init_error(QString)),this,SLOT(conn_init_error(QString)));
            connect(this,SIGNAL(restart(QString)),mqtt,SLOT(restart(QString)));
            this->adaptors.insert("MQTT",mqtt);
            isMQTTLoad=true;
        }
        //如果配置了rs485连接
        else if(keys.at(i)=="rs485"){
            rs485_conf = paras.value("rs485");
        }
        else{
            //加载其他连接
            qDebug()<<"Loading adaptor "<<keys.at(i);
            ConnectionAdaptor * a = loadAdaptor(paras.value(keys.at(i)));
            if(NULL!=a){
                this->adaptors.insert(keys.at(i),a);
                connect(a,SIGNAL(started(QString)),this,SLOT(adaptorStarted(QString)));
                connect(a,SIGNAL(stoped(QString)),this,SLOT(adaptorStoped(QString)));
                connect(a,SIGNAL(conn_disconnected(QString)),this,SLOT(conn_disconnected(QString)));
                connect(a,SIGNAL(conn_init_error(QString)),this,SLOT(conn_init_error(QString)));
                connect(this,SIGNAL(restart(QString)),a,SLOT(restart(QString)));
            }else{
                qDebug()<<"Adaptor "<<keys.at(i)<<" load ERROR!";
                QMap<QString,ConnectionAdaptor*>::const_iterator it = adaptors.cbegin();
                while(it!=adaptors.cend()){
                    it.value()->deleteLater();
                    ++it;
                }
                return;
            }
        }
    }
    //MQTT连接适配器未加载直接退出
    if(!isMQTTLoad){
        qDebug()<<"No MQTT adaptor load!";
        QMap<QString,ConnectionAdaptor*>::const_iterator it = adaptors.cbegin();
        while(it!=adaptors.cend()){
            it.value()->deleteLater();
            ++it;
        }
        return;
    }
    adaptors.value("MQTT")->startWork();
    start();
}

void MainThread::adaptorStarted(QString id)
{
    qDebug()<<"Adaptor "<<id<<" started at "<<this->adaptors[id]->getStartTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    if("MQTT"==id){
        //启动全部连接适配器
        QMap<QString,ConnectionAdaptor*>::const_iterator it = adaptors.cbegin();
        while(it!=adaptors.cend()){
            if(it.key()!="MQTT"){
                it.value()->startWork();
            }
            ++it;
        }
    }
}

void MainThread::adaptorStoped(QString id)
{
    //qDebug()<<"Adaptor "<<id<<" stoped";
}

void MainThread::conn_disconnected(QString id)
{
    qDebug()<<"Main thread: "<<id<<" conn_disconnected slot";
    if(adaptors[id]->isRunning()){
        qDebug()<<"Main thread: waiting for "<<id;
        this->adaptors[id]->wait();
        qDebug()<<"Adaptor "<<id<<" stoped";
    }
    if("MQTT"==id){
        //关闭全部连接适配器
        QMap<QString,ConnectionAdaptor*>::const_iterator it = adaptors.cbegin();
        while(it!=adaptors.cend()){
            if(it.key()!="MQTT"){
                it.value()->stopWork();
            }
            ++it;
        }
        errorAdaptors.clear();
    }
    qDebug()<<"Main thread: "<<id<<" connection lost try again later";
    this->errorAdaptors.push(id);
}

void MainThread::conn_init_error(QString id)
{
    qDebug()<<"Main thread: "<<id<<" connection init_error slot";
    this->errorAdaptors.push(id);
}

ConnectionAdaptor *MainThread::loadAdaptor(Parameter para)
{
    ConnectionAdaptor * ca = new ConnectionAdaptor(para);
    QString path = para.getValue("adaptorClass").toString();
    if(path.isEmpty()){
        ca->deleteLater();
        return NULL;
    }
    QLibrary lib(path);
    if(!lib.load()){
        qDebug()<<"Library "<<path<<" can't load";
        ca->deleteLater();
        return NULL;
    }
    FunPtr createObj = (FunPtr)lib.resolve("createObj");
    AdaptorStrategy *s;
    if(0!=createObj(s)){
        qDebug()<<path<<"Library "<<path<<" create error";
        ca->deleteLater();
        return NULL;
    }
    s->setId(ca->getAdaptorID());
    ca->setStrategy(s);
    return ca;
}

void MainThread::run()
{
    while(1){
        QThread::msleep(5000);
        QString id;
        this->errorAdaptors.waitAndPop(id);
        emit restart(id);
    }
}

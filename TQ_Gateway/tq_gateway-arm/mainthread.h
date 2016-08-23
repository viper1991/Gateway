#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include <QThread>
#include "Common/commom.h"
#include "config/config.h"
#include "interface/connectionadaptor.h"
#include "HardwareConnection/connections/rs485connecction_.h"

typedef int (*FunPtr)(AdaptorStrategy *& stgy);

class MainThread :public QThread
{
    Q_OBJECT
public:
    explicit MainThread(QObject *parent = 0);
    ~MainThread();
    void init(QString configPath);

signals:
    void stopAdaptor(QString id);
    void msg(QString msg);

    void restart(QString id);

public slots:
    void adaptorStarted(QString id);
    void adaptorStoped(QString id);

    void conn_disconnected(QString id);
    void conn_init_error(QString id);
private:
    //从配置文件中加载的全部配置参数
    Parameter para;
    //保存所有连接适配器指针
    QMap<QString,ConnectionAdaptor*> adaptors;
    Config config;
    ConnectionAdaptor * loadAdaptor(Parameter para);
    //每个连接适配器单独的配置参数（从para中读取出来）
    QMap<QString,Parameter> paras;
    //保存异常的适配器
    ThreadSafeQueue<QString> errorAdaptors;

    // QThread interface
protected:
    void run();
};

#endif // MAINTHREAD_H

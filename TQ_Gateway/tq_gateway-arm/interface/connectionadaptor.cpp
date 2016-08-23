#include "connectionadaptor.h"
#include "factories/connectionfactory.h"

ConnectionAdaptor::ConnectionAdaptor(const Parameter &para, int pollTimeOut, QObject *obj):
    QThread(obj),
    pollTimeOut(pollTimeOut),
    isRuning(false),
    parameters(para)
{
    adaptorID=para.getValue("id").toString();
    if(!para.getValue("timeout").isNull()){
        this->pollTimeOut = para.getValue("timeout").toInt();
    }
    this->connection=NULL;
}

ConnectionAdaptor::~ConnectionAdaptor()
{
    if(NULL!=connection)
    {
        connection->deleteLater();
    }
}

int ConnectionAdaptor::send(const QString &strMsg,const Parameter &aim)
{
    return send(strMsg.toLocal8Bit(),aim);
}

int ConnectionAdaptor::send(const QByteArray &bytes, const Parameter &aim)
{
    this->connection->sendData(bytes,aim);
    return 0;

}

void ConnectionAdaptor::startWork()
{
    //创建并且初始化连接
    if(NULL == connection){
        connection=onInitConnection(parameters);
        if(NULL==connection){
            qDebug()<<this->adaptorID<<" connection create fail!";
            emit conn_init_error(this->getAdaptorID());
            return;
        }
        //关联消息发送的信号槽
        qRegisterMetaType<Parameter>("Parameter");
        QObject::connect(this,SIGNAL(sendData(QByteArray,Parameter)),connection,SLOT(onSend(QByteArray,Parameter)));
        QObject::connect(connection,SIGNAL(conn_disconnected()),this,SLOT(conn_disconnected()));
        QObject::connect(this,SIGNAL(force_disconnect()),connection,SLOT(disConnect()));

    }
    qDebug()<<this->adaptorID<<" connection created";
    int rs=connection->connect();
    qDebug()<<this->adaptorID<<" connecting...";
    //没有成功连接则退出
    if(rs!=HC_CONNECTION_SUCCESS){
        onError(rs);//处理错误
        qDebug()<<this->adaptorID<<" connection init error";
        if(NULL!=connection){
            connection->deleteLater();
            qDebug()<<this->adaptorID<<" connection deleteLater called";
        }
        if(HC_SERIAL_INIT_ERROR == rs){
            qDebug()<<this->adaptorID<<" serial init error";
            return;
        }
        emit conn_init_error(this->getAdaptorID());
        qDebug()<<this->adaptorID<<" conn_init_error emited";
        return;
    }
    start(); //开启线程
}

void ConnectionAdaptor::stopWork()
{
    QMutexLocker locker(&mutex);
    this->isRuning=false;
}

int ConnectionAdaptor::poll()
{
    QByteArray data;
    QString rs;
    if(readCmd()<0){
        return DATA_READ_CMD_ERROR;
    }
    //阻塞方式获取数据
    if(HC_WAIT_TIMEOUT==connection->getData(data,3*pollTimeOut)){
        return POLL_TIME_OUT;
    }
    int prs = procData(data,rs);
    if(prs<0){
        return prs;
    }
    uploadData.push(rs);
    return POLL_SUCCESS;
}

QString ConnectionAdaptor::getAdaptorID() const
{
    return this->adaptorID;
}

HardwareConnection::CONNECTION_TYPE ConnectionAdaptor::connectionType()
{
    return connection->connectionType();
}

void ConnectionAdaptor::run()
{
    startTime=QDateTime::currentDateTime();
    isRuning=true;  //修改运行标志
    onStarted();  //处理开启后的一些工作

    //释放工作开启信号
    emit started(getAdaptorID());

    while(true){

        //同步结束标志
        QMutexLocker locker(&mutex);
        if(isRuning==false){
            break;
        }
        locker.unlock();

        //判断结果
        int result=poll();

        //此时存在异常状况
        if(result<0){
            onError(result);
            switch (result) {
            case CRC_CHECK_ERROR:
                qDebug()<<this->getAdaptorID()<<": CRC check error";
                break;
            case DATA_PROC_ERROR:
                qDebug()<<this->getAdaptorID()<<": data process error";
                break;
            case POLL_NEED_MORE_DATA:
                QThread::msleep(1000);
                continue;
            default:
                break;
            }
            continue;
        }else if(result == POLL_TIME_OUT){
            qDebug()<<this->getAdaptorID()<<" POLL time out";
            //{[devNo:12345][ID:123456][IP:128.1.1.1][TIME:20121111000000][PTYPE:DEVEX]}[EXTYPE:1/2]
            //uploadData.push(this->getAdaptorID()+" POLL time out");
            QString out;
            out="{[devNo:";
            out.append(getAdaptorID()).append("]").append("[PTYPE:DEVEX]}");
            out.append("[EXTYPE:2]");
            uploadData.push(out);
            continue;
        }

        if(pollTimeOut>0){
            try{
                QThread::msleep(pollTimeOut);
            }catch(...){
            }
        }
    }

    //关闭连接
    if(connection->is_Connected())
    {
        emit force_disconnect();
    }
    onStoped(); //处理关闭后的一些工作

    //释放关闭信号
    emit stoped(getAdaptorID());

}

void ConnectionAdaptor::forceStop()
{
    this->stopWork();
}

void ConnectionAdaptor::conn_disconnected()
{
    this->stopWork();
    emit conn_disconnected(this->getAdaptorID());
    qDebug()<<this->adaptorID<<" conn_disconnected emited";
}

Connection *ConnectionAdaptor::onInitConnection(Parameter & para)
{
    return ConnectionFactory::createConnection(para);
}

QDateTime ConnectionAdaptor::getStartTime() const
{
    return startTime;
}

void ConnectionAdaptor::setStrategy(AdaptorStrategy *as)
{
    this->stgy = as;
}




#include "restartthread.h"
#include "Common/commom.h"
#include <QDebug>

extern ThreadSafeQueue<QString> errorAdaptors;

RestartThread::RestartThread(QObject *parent):QThread(parent)
{

}

void RestartThread::run()
{
    while(1){
        QString id;
        errorAdaptors.waitAndPop(id);
        qDebug()<<id<<" restarting";
        emit restart(id);
    }
}

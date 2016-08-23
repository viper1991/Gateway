#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H


#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
#include <QMutexLocker>

template<typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue(void){}
    ~ThreadSafeQueue(void){}

    //放入数据
    void push( T const&t);

    //取数据
    bool tryPop(T&t);

    //是否为空
    bool empty() const;

    //等待获取数据
    bool waitAndPop(T&t,long time);

    //等待获取数据
    void waitAndPop(T&t);

    //清除队列
    void clear();

    //队列长度
    int size();

private:
    QMutex mutex; //线程安全锁
    QWaitCondition  condition;
    QQueue<T> tQueue; //数据队列

};

template<typename T>
void ThreadSafeQueue<T>::clear()
{
    QMutexLocker lock(&mutex);

    while(tQueue.empty()==false){
        tQueue.pop_front();
    }
}

template<typename T>
int ThreadSafeQueue<T>::size()
{
    QMutexLocker lock(&mutex);
    return tQueue.size();
}

template<typename T>
bool ThreadSafeQueue<T>::waitAndPop(T&t, long time)
{
    QMutexLocker lock(&mutex);
    //队列为空时等待
    while(tQueue.empty()==true){
        if(!condition.wait(&mutex,time)){
            return false;
        }
    }
    t=tQueue.first();
    tQueue.pop_front();
    return true;
}

template<typename T>
void ThreadSafeQueue<T>::waitAndPop(T &t)
{
    QMutexLocker lock(&mutex);
    //队列为空时等待
    while(tQueue.empty()==true){
        condition.wait(&mutex);
    }
    t=tQueue.first();
    tQueue.pop_front();
}

template<typename T>
bool ThreadSafeQueue<T>::empty() const
{
    QMutexLocker lock(&mutex);
    return tQueue.empty();
}

template<typename T>
bool ThreadSafeQueue<T>::tryPop(T&t)
{
    QMutexLocker lock(&mutex);
    if(tQueue.empty()==true){
        return false;
    }
    t=tQueue.first();
    tQueue.pop_front();
    return true;
}

template<typename T>
void ThreadSafeQueue<T>::push(T const&t)
{
    QMutexLocker lock(&mutex);
    tQueue.push_back(t);
    lock.unlock();
    condition.wakeOne();
}

#endif // THREADSAFEQUEUE_H

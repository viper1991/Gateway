#ifndef SINGLETON_H
#define SINGLETON_H

#include <QMutex>
#include <QMutexLocker>
#include <QScopedPointer>

static QMutex mutex;//实例互斥锁。

/**
 * 单例模板类
 */
template<typename T>
class Singleton
{
public:
    ~Singleton(){}

    static T* getInstance(bool* isnew){
        QMutexLocker locker(&mutex);//加互斥锁
        *isnew = false;
        if(!instance){      // 线程不安全，这里需要同步
             instance=new T;
             *isnew = true;
        }
        return instance;
    }

    static void delInstance(){
        QMutexLocker locker(&mutex);//加互斥锁
        instance=NULL;
    }

protected:
    Singleton(){}
    Singleton(const Singleton<T>&);
    Singleton<T>& operator=(const Singleton<T>);
private:

    static T* instance;
};

template<typename T>
T* Singleton<T>::instance;


#endif // SINGLETON_H

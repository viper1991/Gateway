#ifndef RESTARTTHREAD_H
#define RESTARTTHREAD_H
#include <QThread>
#include <QString>

class RestartThread : public QThread
{
    Q_OBJECT
public:
     explicit RestartThread(QObject *parent = 0);

signals:
    void restart(QString id);

protected:
    void run();
};

#endif // RESTARTTHREAD_H

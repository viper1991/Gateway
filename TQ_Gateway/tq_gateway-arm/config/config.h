#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>
#include <QDomDocument>
#include <QFile>
#include "Common/commom.h"

#define FILE_OPEN_ERROR -1
#define FILE_OPEN_SUCESS 1
#define FILE_READ_ERROR -2
#define FILE_READ_SUCESS 2
#define FILE_PROC_ERROR -3
#define FILE_PROC_SUCESS 3

class Config : QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = 0);
    int readConfig(QString path, Parameter & para, QString &errMsg, int &line, int &col);

private:
    QFile *file;
    QDomDocument dom;
    int xml_to_parameter(Parameter & para);

};

#endif // CONFIG_H

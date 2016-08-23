#include "config.h"


Config::Config(QObject *parent) : QObject(parent)
{

}

int Config::readConfig(QString path, Parameter &para, QString& errMsg, int& line,int& col)
{
    this->file = new QFile(path);
    if(!file->open(QFile::ReadOnly | QFile::Text)){
        return FILE_OPEN_ERROR;
    }
    if(!dom.setContent(file,&errMsg,&line,&col)){
        return FILE_OPEN_ERROR;
        file->close();
    }
    return xml_to_parameter(para);
}

int Config::xml_to_parameter(Parameter &para)
{
    QDomElement root =  dom.documentElement();
    QDomNode mqtt = root.firstChild();
    //首先加载mqtt配置
    if(!mqtt.isNull()){
        QDomElement e = mqtt.toElement();
        if(e.tagName()!="MQTT"){
            return FILE_PROC_ERROR;
        }
        QMap<QString,QVariant> varMap;
        QDomNodeList list = e.childNodes(); //获得元素e的所有子节点的列表
        for(int i=0; i<list.count(); i++){
            QDomNode node = list.at(i);
            if(node.isElement()){
                varMap.insert(node.toElement().tagName(),node.toElement().text());
            }
        }
        para.setValue("MQTT",varMap);
    }else{
        return FILE_PROC_ERROR;
    }
    QDomNode adaptor = mqtt.nextSibling();
    //遍历Adaptors除MQTT外其他
    while(!adaptor.isNull()){
        QDomElement e = adaptor.toElement();
        if(e.tagName()=="rs485"){
            QMap<QString,QVariant> varMap;
            QDomNodeList list = e.childNodes(); //获得元素e的所有子节点的列表
            for(int i=0; i<list.count(); i++){
                QDomNode node = list.at(i);
                if(node.isElement()){
                    varMap.insert(node.toElement().tagName(),node.toElement().text());
                }
            }
            para.setValue("rs485",varMap);
        }
        else if(e.tagName()=="adaptor"){
            QString a_id = e.attribute("id");
            QDomNodeList list = e.childNodes();
            QMap<QString,QVariant> a;
            a.insert("id",a_id);
            //遍历adaptor
            for(int i=0;i<list.count();i++){
                QDomNode node = list.at(i);
                if(node.isElement()){
                    QDomElement e=node.toElement();
                    a.insert(e.tagName(),e.text());
                }
            }
            para.setValue(a_id,a);
        }
        else {
            return FILE_PROC_ERROR;
        }
        adaptor=adaptor.nextSibling();
    }
    file->close();
    return FILE_PROC_SUCESS;
}

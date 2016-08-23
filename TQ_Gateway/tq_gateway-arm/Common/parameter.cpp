#include "parameter.h"

Parameter::Parameter()
{

}

void Parameter::setVarMap(const QMap<QString, QVariant> &newMap)
{
    varMap=newMap;
}

//QString Parameter::toString() const
//{

//    QString str;

//    QMap<QString,QVariant>::ConstIterator it=varMap.cbegin();

//    str.append("{");
//    while(it!=varMap.cend()){

//        str.append(it.key());
//        str.append(":");
//        str.append(it.value().toString());
//        str.append(",");
//        ++it;

//    }

//    int lastF= str.lastIndexOf(",");
//    if(lastF!=-1){
//        str=str.left(str.length()-1);
//    }

//    str.append("}");

//    return str;

//}

QStringList Parameter::getKeys() const
{
    QStringList keys;
    QMap<QString,QVariant>::ConstIterator it=varMap.cbegin();
    while(it!=varMap.cend()){
        keys.append(it.key());
        ++it;
    }
    return keys;
}


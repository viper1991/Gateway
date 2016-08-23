#ifndef PARAMETER_H
#define PARAMETER_H


#include <QVariant>
#include <QMap>
#include <QString>

class Parameter
{
public:
    Parameter();

    //取值
    QVariant getValue(const QString & key) const{

        return varMap[key];

    }

    //设定值
    void setValue(const QString & key,const QVariant & value){
        if(varMap.find(key)!=varMap.end()){
            varMap[key]=value;
        }else{
            varMap.insert(key,value);
        }
    }
    void setVarMap(const QMap<QString,QVariant> & newMap);
    QStringList getKeys() const;
private:
    QMap<QString,QVariant> varMap;

};

#endif // PARAMETER_H

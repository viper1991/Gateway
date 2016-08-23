#include "testconnadaptor2.h"
#include <QStringList>
#include <QDateTime>

int createObj(AdaptorStrategy *&st)
{
    st = new TestConnAdaptor2();
    if(NULL!=st)
    {
        return 0;
    } else{
        return -1;
    }
}

QByteArray TestConnAdaptor2::readCmd()
{
    return QString("READ").toLocal8Bit();
}

int TestConnAdaptor2::procData(QByteArray &in, QString &out)
{
    //{[devNo:12345][ID:123456][IP:128.1.1.1][TIME:20121111000000][PTYPE:DATA]}
    //[Temp:28][Humi:78%]
    QString msg(in);
    QStringList m = msg.split(",");
    out="{[devNo:";
    out.append(getId()).append("]").append("[PTYPE:DATA]}");
    out.append("[airtempertaiji:").append(m[0].trimmed()).append("]");
    out.append("[airhumidloit:").append(m[1].trimmed().left(m[1].trimmed().size()-1)).append("]");
    return 1;
}

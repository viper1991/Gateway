#ifndef TCPFACTORY_H
#define TCPFACTORY_H
#include "interface/connection.h"

class ConnectionFactory
{
public:
    ConnectionFactory();

    static Connection *createConnection(Parameter parameter);
};

#endif // TCPFACTORY_H


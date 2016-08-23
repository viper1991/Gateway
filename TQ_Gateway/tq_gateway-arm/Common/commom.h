#ifndef COMMOM
#define COMMOM
#include "parameter.h"
#include "threadsafequeue.h"

#define HC_CONNECTION_SUCCESS 1
#define HC_CONNECTION_ERROR -1
#define HC_CONNECTION_TIMEOUT -2
#define HC_CONNECTION_RECV_QUEUE_NO_DATA 2
#define HC_WAIT_TIMEOUT 0
#define HC_SERIAL_INIT_ERROR -3

namespace HardwareConnection{
    enum CONNECTION_TYPE{
        UDP=1,
        TCP,
        SERIAL_232,
        SERIAL_485,
        MQTT,
        OTHER
    };
}


#endif // COMMOM


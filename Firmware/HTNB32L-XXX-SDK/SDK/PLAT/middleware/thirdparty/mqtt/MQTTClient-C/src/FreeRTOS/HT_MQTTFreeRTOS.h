#ifndef __HT_MQTTFREERTOS_H__
#define __HT_MQTTFREERTOS_H__

#include "MQTTFreeRTOS.h"

void HT_NetworkInit(Network *n);
int HT_NetworkConnect(Network *n, char *addr, int port);
int HT_NetworkSetConnTimeout(Network *n, int send_timeout, int recv_timeout);

#endif /* __HT_MQTTFREERTOS_H__ */

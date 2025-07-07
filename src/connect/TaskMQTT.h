#ifndef INC_TASKMQTT_H_
#define INC_TASKMQTT_H_

#include "globals.h"
extern String IO_USERNAME;
extern String IO_KEY;
void initMQTT();
void reconnectMQTT(void *pvParameter);
void publishData(String feed, String data);

#endif /* INC_TASKMQTT_H_ */
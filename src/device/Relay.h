#ifndef INC_RELAY_H_
#define INC_RELAY_H_
#include <globals.h>
extern String relayState;
extern void initRelay();
void relayTask(void *pvParameters);
#endif // INC_RELAY_H_
#ifndef INC_LED_H_
#define INC_LED_H_
#include "globals.h"
extern String ledState;
extern void initLed();
void ledTask(void *pvParameters);
#endif
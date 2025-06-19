#ifndef INC_LIGHTSENSOR_H_
#define INC_LIGHTSENSOR_H_
#include <globals.h>
extern String light; 
extern void initLightSensor();
void lightSensorTask(void *pvParameters);
#endif
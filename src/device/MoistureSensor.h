#ifndef INC_MOISTURESENSOR_H
#define INC_MOISTURESENSOR_H
#include <globals.h>
extern String moistureValue;
extern void initMoistureSensor();
void moistureSensorTask(void *pvParameters);
#endif
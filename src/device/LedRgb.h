#ifndef INC_LEDRGB_H_
#define INC_LEDRGB_H_
#include <globals.h>
extern String colorValue;
extern String ledMode;
extern void initLedRgb();
void ledRgbTask(void *pvParameters);
#endif
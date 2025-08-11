#ifndef INC_NEOPIXEL_H
#define INC_NEOPIXEL_H
#include "globals.h"
// extern bool state;
// extern Adafruit_NeoPixel neo_pixel;

extern void initNeoPixel();
void NeoPixelTask(void *pvParameter);
#endif
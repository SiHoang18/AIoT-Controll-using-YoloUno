#ifndef INC_LCD_H
#define INC_LCD_H
#include <globals.h>
extern LiquidCrystal_I2C lcd; 
void initLCD();
void lcdPrint(const String &message);
void lcdTask(void *pvParameters);
#endif
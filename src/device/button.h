#ifndef INC_BUTTON_H
#define INC_BUTTON_H
#include "globals.h"
extern int buttonFlag1;
extern int buttonFlag2;
void initButton();
void buttonTask(void *pvParameters);
#endif
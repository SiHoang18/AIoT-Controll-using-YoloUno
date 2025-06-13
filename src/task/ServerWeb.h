#ifndef INC_SERVERWEB_H_
#define INC_SERVERWEB_H_
#include <globals.h>  

extern void initServerWeb();
void handleRoot();
void handleGPIO26On();
void handleGPIO26Off();
void handleGPIO27On();
void handleGPIO27Off();
void webServerTask(void *pvParameters);

#endif

#ifndef INC_INFO_H
#define INC_INFO_H
#include "globals.h"
extern void initWiFiACP();
extern bool check_config();
void load_config();
void reset_esp();
void task_reset_esp(void *pvParameter);
#endif 
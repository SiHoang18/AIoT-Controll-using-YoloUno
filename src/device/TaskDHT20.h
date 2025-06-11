#ifndef INC_TASKDHT20_H_
#define INC_TASKDHT20_H_

#include "globals.h"

extern void initDHT20();
extern void getValueDHT20(void *pvParameters);
extern int getTemperatureDHT20();
extern int getHumidityDHT20();
#endif /* INC_TASKDHT20_H_ */
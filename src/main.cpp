
#include "globals.h"
#define MY_SCL 11
#define MY_SDA 12

void setup()
{
  if(!LittleFS.begin()){
    Serial.println("Fail to mount LittleFS");
    return;
  }
  Serial.begin(115200);
  Wire.begin(MY_SCL, MY_SDA);
  if(check_config()){
    init_connect();
    // init_device();
  }
}

void loop()
{
}
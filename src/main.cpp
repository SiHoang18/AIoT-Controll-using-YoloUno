
#include "globals.h"
void setup()
{
  Serial.begin(115200);
#ifdef M5_CORE2
  M5.begin(true, false, true); 
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE); 
#else 
  Wire.begin(MY_SDA, MY_SCL);
#endif
  if(!LittleFS.begin()){
    Serial.println("Fail to mount LittleFS");
    return;
  }
  if(check_config()){
    init_connect();
    init_device();
  }
}

void loop()
{
  
}
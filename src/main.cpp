
#include "globals.h"
#define MY_SCL 11
#define MY_SDA 12

void setup()
{
  Serial.begin(115200);
  Wire.begin(MY_SCL, MY_SDA);
  InitWiFi();
  initMQTT();
  initServerWeb();
  initDHT20();
  initRelay();
  initMiniFan();
  initLedRgb();
  initLed();
  initLightSensor();
  initMoistureSensor();
  initUltrasonic();
  initTask();
  initLCD();
 
}

void loop()
{
  // Serial.println("Hello");
  if (!Wifi_reconnect())
  {
    return;
  }
  reconnectMQTT();
}
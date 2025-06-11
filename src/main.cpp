
#include "globals.h"

void setup()
{
  Serial.begin(115200);
  initDHT20();
  initMQTT();
  initUltrasonic();
}

void loop()
{
  if (!Wifi_reconnect())
  {
    return;
  }
  reconnectMQTT();
  // getValueDHT20();
  // // publishData("humidity", String(getHumidityDHT20()));
  // // delay(1000);
  // // publishData("tempurature", String(getTemperatureDHT20()));
  // delay(2000);
}
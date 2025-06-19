#include "TaskDHT20.h"
#define DelayTime 3000
DHT20 dht20;
String humidity = "0";
String temperature = "0";
void getValueDHT20(void *pvParameters)
{
    while(1){
        if (dht20.read() == DHT20_OK)
        {
            humidity = String(dht20.getHumidity());
            temperature = String(dht20.getTemperature());
            publishData("humidity", String(dht20.getHumidity()));
            delay(10);
            publishData("temperature", String(dht20.getTemperature()));
            delay(10);
            Serial.println(String(dht20.getTemperature()) + "-" + String(dht20.getHumidity()));
        }
        else
        {
            Serial.println("Failed to read DHT20 sensor.");
        }
        vTaskDelay(DelayTime / portTICK_PERIOD_MS);
    }
}
void initDHT20()
{
    dht20.begin();
    xTaskCreate(getValueDHT20, "getValueDHT20", 2048, NULL, 1, NULL);
}

#include "TaskDHT20.h"

#define MY_SCL 11
#define MY_SDA 12
#define DelayTime 5000
DHT20 dht20;

void getValueDHT20(void *pvParameters)
{
    while(1){
        publishData("humidity", String(dht20.getHumidity()));
        delay(100);
        publishData("temperature", String(dht20.getTemperature()));
        delay(100);
        if (dht20.read() == DHT20_OK)
        {
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
    Wire.begin(MY_SCL, MY_SDA);
    dht20.begin();
    xTaskCreate(getValueDHT20, "getValueDHT20", 2048, NULL, 1, NULL);
}

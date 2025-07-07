#include "../src/device/LightSensor.h"

#define DelayTime 3000
String light = "0";
void initLightSensor() {
    pinMode(LIGHT_PORT, INPUT); 
    xTaskCreate(lightSensorTask, "lightSensorTask", 2048, NULL, 1, NULL);
}
void lightSensorTask(void *pvParameters) {
    while (true) {
        int lightValue = analogRead(LIGHT_PORT); 
        Serial.print("Light Sensor Value: ");
        Serial.println(lightValue);
        light = String(lightValue);
        publishData("light_sensor", String(lightValue));
        
        vTaskDelay(DelayTime / portTICK_PERIOD_MS); 
    }
}
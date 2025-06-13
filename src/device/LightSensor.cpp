#include "../src/device/LightSensor.h"
#define PIN_MODE 10

void initLightSensor() {
    pinMode(PIN_MODE, INPUT); 
    xTaskCreate(lightSensorTask, "lightSensorTask", 2048, NULL, 1, NULL);
}
void lightSensorTask(void *pvParameters) {
    while (true) {
        int lightValue = analogRead(PIN_MODE); 
        Serial.print("Light Sensor Value: ");
        Serial.println(lightValue);

        publishData("light_sensor", String(lightValue));
        
        vTaskDelay(2000 / portTICK_PERIOD_MS); 
    }
}
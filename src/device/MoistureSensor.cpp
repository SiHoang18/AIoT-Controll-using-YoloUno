#include "../src/device/MoistureSensor.h"
#define DelayTime 3000
String moistureValue = "0";

void initMoistureSensor() {
    pinMode(MOISTURE_PORT, INPUT); 
    xTaskCreate(moistureSensorTask, "moistureSensorTask", 2048, NULL, 1, NULL);
}
void moistureSensorTask(void *pvParameters) {
    while (true) {
        int moistureValueInt = analogRead(MOISTURE_PORT); 
        moistureValue = String(moistureValueInt);
        Serial.print("Moisture Sensor Value: ");
        Serial.println(moistureValue);
        
        publishData("moisture_sensor", moistureValue);
        
        vTaskDelay(DelayTime / portTICK_PERIOD_MS); 
    }
}
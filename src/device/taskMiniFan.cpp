#include<../src/device/taskMiniFan.h>
String fanState = "UNACTIVATE";

void initMiniFan() {
    pinMode(FAN_PORT, OUTPUT); 
    digitalWrite(FAN_PORT, LOW); 
    xTaskCreate(taskMiniFan, "taskMiniFan", 2048, NULL, 1, NULL);
}
void taskMiniFan(void *pvParameters) {
    while (true) {
        if (fanState == "UNACTIVATE") {
            digitalWrite(FAN_PORT,LOW);
        } else if (fanState == "ACTIVATE") {
            digitalWrite(FAN_PORT,HIGH);
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
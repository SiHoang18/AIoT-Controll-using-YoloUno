#include<../src/device/taskMiniFan.h>
int flag = false;
void initMiniFan() {
    pinMode(6, OUTPUT); 
    digitalWrite(6, LOW); 
    xTaskCreate(taskMiniFan, "taskMiniFan", 2048, NULL, 1, NULL);
}
void taskMiniFan(void *pvParameters) {
    while (true) {
        if (flag) {
            digitalWrite(6,HIGH);
            flag = false;
            publishData("fan_state", "on");
            Serial.println("Fan is ON");
            vTaskDelay(1000 / portTICK_PERIOD_MS); 
        } else {
            digitalWrite(6,LOW);
            flag = true;
            publishData("fan_state", "off");
            Serial.println("Fan is OFF");
            vTaskDelay(5000 / portTICK_PERIOD_MS);
        }
    }
}
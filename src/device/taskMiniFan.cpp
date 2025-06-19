#include<../src/device/taskMiniFan.h>
String fanState = "UNACTIVATE";
int fanOutput = 10;
void initMiniFan() {
    pinMode(fanOutput, OUTPUT); 
    digitalWrite(fanOutput, LOW); 
    xTaskCreate(taskMiniFan, "taskMiniFan", 2048, NULL, 1, NULL);
}
void taskMiniFan(void *pvParameters) {
    while (true) {
        if (fanState == "UNACTIVATE") {
            digitalWrite(fanOutput,LOW);
        } else if (fanState == "ACTIVATE") {
            digitalWrite(fanOutput,HIGH);
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
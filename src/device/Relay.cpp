#include<../src/device/Relay.h>
String relayState = "UNACTIVATE";
int relayOutput = 8;
void initRelay() {
    pinMode(relayOutput, OUTPUT);
    digitalWrite(relayOutput, LOW); 
    xTaskCreate(relayTask, "relayTask", 2048, NULL, 1, NULL);
}
void relayTask(void *pvParameters) {
    while (true) {
        if (relayState == "UNACTIVATE") {
            digitalWrite(relayOutput, LOW);
        } else if (relayState == "ACTIVATE") {
            digitalWrite(relayOutput, HIGH);
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
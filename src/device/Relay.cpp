#include<../src/device/Relay.h>
String relayState = "ACTIVATE";

void initRelay() {
    pinMode(4, OUTPUT);
    digitalWrite(4, LOW); 
    xTaskCreate(relayTask, "relayTask", 2048, NULL, 1, NULL);
}
void relayTask(void *pvParameters) {
    while (true) {
        if (relayState == "UNACTIVATE") {
            digitalWrite(4, LOW);
        } else if (relayState == "ACTIVATE") {
            digitalWrite(4, HIGH);
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
#include<../src/device/Relay.h>
String relayState = "UNACTIVATE";

void initRelay() {
    pinMode(RELAY_PORT, OUTPUT);
    digitalWrite(RELAY_PORT, LOW); 
    xTaskCreate(relayTask, "relayTask", 2048, NULL, 1, NULL);
}
void relayTask(void *pvParameters) {
    while (true) {
        if (relayState == "UNACTIVATE") {
            digitalWrite(RELAY_PORT, LOW);
        } else if (relayState == "ACTIVATE") {
            digitalWrite(RELAY_PORT, HIGH);
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
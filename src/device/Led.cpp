#include <../src/device/Led.h>
String ledState = "UNACTIVATE";

void initLed() {
    pinMode(LED_PORT, LED_PORT);
    digitalWrite(LED_PORT, LOW); 
    xTaskCreate(ledTask, "ledTask", 2048, NULL, 1, NULL);
}

void ledTask(void *pvParameters) {
    while (true) {
        if (ledState == "UNACTIVATE") {
            digitalWrite(LED_PORT, LOW);
        } else if (ledState == "ACTIVATE") {
            digitalWrite(LED_PORT, HIGH);
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
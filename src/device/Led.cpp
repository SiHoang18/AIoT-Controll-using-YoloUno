#include <../src/device/Led.h>
String ledState = "UNACTIVATE";
int output = 48;
void initLed() {
    pinMode(output, OUTPUT);
    digitalWrite(output, LOW); 
    xTaskCreate(ledTask, "ledTask", 2048, NULL, 1, NULL);
}

void ledTask(void *pvParameters) {
    while (true) {
        if (ledState == "UNACTIVATE") {
            digitalWrite(output, LOW);
        } else if (ledState == "ACTIVATE") {
            digitalWrite(output, HIGH);
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
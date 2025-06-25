#include "../src/task/rgbTask.h"
const char* colorHex[] = {
    "#FF0000", 
    "#FFA500", 
    "#FFFF00", 
    "#00FF00", 
    "#0000FF", 
    "#4B0082", 
    "#9400D3",
    "#000000", 
    "#FFFFFF"  
};
int colorIndex = 0;
void initLedRgbTask() {
    xTaskCreate(ledRgbTaskCustom, "ledRgbTask", 2048, NULL, 1, NULL); 
}
void ledRgbTaskCustom(void *pvParameters) {
    int colorCount = sizeof(colorHex) / sizeof(*colorHex);
    while (true) {
        if (ledMode == "AUTOMATIC") {
            colorValue = colorHex[colorIndex];
            colorIndex = (colorIndex + 1) % colorCount;
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        } 
    }
}   
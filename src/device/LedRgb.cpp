#include "../src/device/LedRgb.h"
const int output = 10; 
int i = 0;
Adafruit_NeoPixel Pixel(4, output , NEO_GRB + NEO_KHZ800); 
void initLedRgb() {
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    Pixel.begin(); 
    Pixel.clear(); 
    Pixel.show();
    xTaskCreate(ledRgbTask, "ledRgbTask", 2048, NULL, 1, NULL); 
}
void ledRgbTask(void *pvParameters) {
    while (true) {
        if (i % 8 < 4){
            Pixel.setPixelColor((i++) % 4, Pixel.Color(255, 0, 0));
        }else{
            Pixel.setPixelColor((i++) % 4, Pixel.Color(0, 0, 0));
        }
        Pixel.show();
        if(i % 8 == 0) Pixel.clear();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
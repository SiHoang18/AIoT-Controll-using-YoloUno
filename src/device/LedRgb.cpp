#include "../src/device/LedRgb.h"
String colorValue = "#ffffff";
String ledMode = "UNACTIVATE";

Adafruit_NeoPixel Pixel(4, RGB_PORT , NEO_GRB + NEO_KHZ800); 
void initLedRgb() {
    pinMode(RGB_PORT, OUTPUT);
    digitalWrite(RGB_PORT, LOW);
    Pixel.begin(); 
    Pixel.clear(); 
    Pixel.show();
    xTaskCreate(ledRgbTask, "ledRgbTask", 2048, NULL, 1, NULL); 
}
void ledRgbTask(void *pvParameters) {
    while (true){
        if (ledMode == "UNACTIVATE"){
            Pixel.clear();
            Pixel.show();
            digitalWrite(RGB_PORT, LOW);
        }else {
            int r = strtol(colorValue.substring(1, 3).c_str(), NULL, 16);
            int g = strtol(colorValue.substring(3, 5).c_str(), NULL, 16);
            int b = strtol(colorValue.substring(5, 7).c_str(), NULL, 16);
            Pixel.clear();
            for(int i = 0; i < Pixel.numPixels(); i++) {
                Pixel.setPixelColor(i, Pixel.Color(r, g, b));
            }
            Pixel.show();
            digitalWrite(RGB_PORT, HIGH);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);

    }
}

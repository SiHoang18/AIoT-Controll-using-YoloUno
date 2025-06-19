#include "../src/device/LedRgb.h"
String colorValue = "#ffffff";
String ledMode = "UNACTIVATE";
int colorIndex = 0;
const int rgbOutput = 6; 
Adafruit_NeoPixel Pixel(4, rgbOutput , NEO_GRB + NEO_KHZ800); 

uint32_t color[] = {
    Pixel.Color(255, 0, 0),    
    Pixel.Color(255, 165, 0),  
    Pixel.Color(255, 255, 0),  
    Pixel.Color(0, 255, 0),     
    Pixel.Color(0, 0, 255),   
    Pixel.Color(75, 0, 130),   
    Pixel.Color(148, 0, 211),  
    Pixel.Color(0, 0, 0),      
    Pixel.Color(255, 255, 255) 
};

int colorCount = sizeof(color) / sizeof(*color);

void initLedRgb() {
    pinMode(rgbOutput, OUTPUT);
    digitalWrite(rgbOutput, LOW);
    Pixel.begin(); 
    Pixel.clear(); 
    Pixel.show();
    xTaskCreate(ledRgbTask, "ledRgbTask", 2048, NULL, 1, NULL); 
}
void ledRgbTask(void *pvParameters) {
    while (true) {
        if (ledMode == "UNACTIVATE"){
            Pixel.clear();
            Pixel.show();
            digitalWrite(rgbOutput, LOW);
            vTaskDelay(200 / portTICK_PERIOD_MS);
        } else if(ledMode == "AUTOMATIC"){
            Pixel.clear();
            for(int i = 0; i < 4; i++) {
                Pixel.setPixelColor(i, color[colorIndex]);
            }
            colorIndex = (colorIndex + 1) % colorCount;
            Pixel.show();
            digitalWrite(rgbOutput, HIGH);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        } else if(ledMode == "MANUAL"){
            int r = strtol(colorValue.substring(1, 3).c_str(), NULL, 16);
            int g = strtol(colorValue.substring(3, 5).c_str(), NULL, 16);
            int b = strtol(colorValue.substring(5, 7).c_str(), NULL, 16);
            for(int i = 0; i < Pixel.numPixels(); i++) {
                Pixel.setPixelColor(i, Pixel.Color(r, g, b));
            }
            Pixel.show();
            digitalWrite(rgbOutput, HIGH);
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }
    }
}

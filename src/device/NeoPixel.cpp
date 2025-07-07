#include "../src/device/NeoPixel.h"
Adafruit_NeoPixel neo_pixel(1, NEOPIXEL_PORT, NEO_GRB + NEO_KHZ800);
void initNeoPixel(){
    neo_pixel.begin();
    neo_pixel.setBrightness(50);
    neo_pixel.show();
    xTaskCreate(NeoPixelTask,"Neo_Pixel",4096,NULL,1,NULL);
}
void NeoPixelTask(void *pvParameter){
    bool state = false;
    while (1)
    {
        if (state){
            neo_pixel.setPixelColor(0, neo_pixel.Color(255,255,255)); 
            neo_pixel.show();
            state = false;
        }else{
            neo_pixel.setPixelColor(0, neo_pixel.Color(0,0,0)); 
            neo_pixel.show();
            state = true;

        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
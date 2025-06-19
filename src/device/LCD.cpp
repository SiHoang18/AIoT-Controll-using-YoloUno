#include "../src/device/LCD.h"

LiquidCrystal_I2C lcd(0x21,16,2); 
int flag = 0;
void initLCD() {
    lcd.init(); 
    lcd.backlight();
    lcd.clear();
    xTaskCreate(lcdTask, "lcdTask", 2048, NULL, 1, NULL);
}

void lcdPrint(const String &message, uint8_t row, uint8_t col) {
    lcd.setCursor(col, row);
    lcd.print(message);
}

void lcdTask(void *pvParameters) {
    while (true) {
        if(flag == 0){
            String msg = "Humidity: " + humidity + "%";
            lcd.clear();
            lcdPrint(msg, 0, 0);
            msg = "Temp:" + temperature + "C";
            lcdPrint(msg, 1, 0);
            ++flag;
        }else if(flag == 1){
            String msg = "Light: " + light;
            lcd.clear();
            lcdPrint(msg, 0, 0);
            msg = "Moisture: " + moistureValue;
            lcdPrint(msg, 1, 0);
            ++flag;
        }else if(flag == 2){
            String msg = "Distance: " + distance + "cm";
            lcd.clear();
            lcdPrint(msg, 0, 0);
            flag = 0; 
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
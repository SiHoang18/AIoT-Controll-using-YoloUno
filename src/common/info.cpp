#include "../src/common/info.h"

#ifdef M5_CORE2
void printOnM5(const char* text, uint16_t color, uint8_t textSize, uint8_t x, uint8_t y) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(x,y);
    M5.Lcd.setTextSize(textSize);
    M5.Lcd.setTextColor(color);
    M5.Lcd.println(text);
}
#endif
void load_config(){
    if(!LittleFS.exists("/config.dat")){
        return;
    }
    File configFile = LittleFS.open("/config.dat","r",false);
    if (!configFile) {
        Serial.println("Failed to open config file");
        return;
    }
    size_t size = configFile.size();
    if (size == 0) {
        Serial.println("Config file is empty");
        configFile.close();
        return;
    }
    std::unique_ptr<char[]> buf(new char[size + 1]);
    configFile.readBytes(buf.get(), size);
    buf[size] = '\0';

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, buf.get());
    if (error) {
        Serial.print("Failed to parse config file: ");
        Serial.println(error.c_str());
        configFile.close();
        return;
    }
    WIFI_SSID = doc["WiFi SSID"].as<String>();
    WIFI_PASSWORD = doc["WiFi Password"].as<String>();
    IO_USERNAME = doc["MQTT SSID"].as<String>();
    IO_KEY = doc["MQTT Password"].as<String>();

    LED_PORT = doc["LED_PORT"].as<int>();
    NEOPIXEL_PORT = doc["NEOPIXEL_PORT"].as<int>();
    RGB_PORT = doc["RGB_PORT"].as<int>();
    LIGHT_PORT = doc["LIGHT_PORT"].as<int>();
    MOISTURE_PORT = doc["MOISTURE_PORT"].as<int>();
    RELAY_PORT = doc["RELAY_PORT"].as<int>();
    FAN_PORT = doc["FAN_PORT"].as<int>();
    ULTRASONIC_TRIG_PORT = doc["ULTRASONIC_TRIG_PORT"].as<int>();
    ULTRASONIC_ECHO_PORT = doc["ULTRASONIC_ECHO_PORT"].as<int>();
    MY_SCL = doc["MY_SCL"].as<int>();
    MY_SDA = doc["MY_SDA"].as<int>();
    configFile.close();
}
void initWiFiACP(){
#ifndef M5_CORE2
    initNeoPixel();
#endif
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_ACP_NAME);
    Serial.print("Wifi ACP Address: ");
    Serial.println(WiFi.softAPIP().toString());
    initConfigServer();
#ifdef M5_CORE2
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.println("Please connect to:");
    int screenWidth = M5.Lcd.width();
    int textWidth = M5.Lcd.textWidth(String(WIFI_ACP_NAME));
    M5.Lcd.setTextColor(CYAN);
    M5.Lcd.setCursor((screenWidth - textWidth) / 2, 50);
    M5.Lcd.println(WIFI_ACP_NAME);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(0, 80);
    M5.Lcd.println("Then go to:");
    String ipAddress = WiFi.softAPIP().toString();
    textWidth = M5.Lcd.textWidth(ipAddress);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setCursor((screenWidth - textWidth) / 2, 120);
    M5.Lcd.println(ipAddress);
#endif
}

void task_reset_esp(void *pvParameter) {
    unsigned long buttonPressStart = 0;
    // pinMode(BOOT,INPUT);

    while (true) {
    #ifdef M5_CORE2
        M5.update();
    #endif
        if (digitalRead(BOOT) == LOW)
        {
            if (buttonPressStart == 0)
            {
                buttonPressStart = millis();
            }
            else if (millis() - buttonPressStart > 5000)
            {
                Serial.println("Start cleaning");
                if (LittleFS.exists("/config.dat"))
                {
                    LittleFS.remove("/config.dat");
                }
                Serial.println("Clean Completed");
                ESP.restart();
            }
        }
        #ifdef M5_CORE2
        else if(M5.BtnB.wasPressed()){
            if (buttonPressStart == 0){
                buttonPressStart = 0;
            }else if (millis() - buttonPressStart > 5000){
                if (LittleFS.exists("/config.dat"))
                {
                    LittleFS.remove("/config.dat");
                }
                printOnM5("M5Core2 Restart",WHITE,10,10,60);
                ESP.restart();
            }
        }
        #endif
        else{
            buttonPressStart = 0;
        }
        vTaskDelay(RESET_DELAY_TIME);
    }
}
void reset_esp(){
    // pinMode(BOOT,INPUT);
    xTaskCreate(task_reset_esp,"reset_task",4096,NULL,1,NULL);
}

bool check_config(){
    // state = false;
    load_config();
    reset_esp();
    if(WIFI_SSID.isEmpty() || WIFI_PASSWORD.isEmpty() || IO_USERNAME.isEmpty() || IO_KEY.isEmpty()){
        initWiFiACP();
        return false;
    }
    return true;
}

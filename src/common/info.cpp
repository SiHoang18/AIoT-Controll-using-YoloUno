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
DynamicJsonDocument json_decode(File file){
    if(!file) return NULL;
    size_t size = file.size();
   if (size == 0) {
        Serial.println("Config file is empty");
        return NULL;
    }
    std::unique_ptr<char[]> buf(new char[size + 1]);
    file.readBytes(buf.get(), size);
    buf[size] = '\0';

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, buf.get());
    if (error) {
        Serial.print("Failed to parse config file: ");
        Serial.println(error.c_str());
        return NULL;
    }
    return doc;
}
void load_config(){
    if(!LittleFS.exists("/config.dat")){
        return;
    }
    File configFile = LittleFS.open("/config.dat","r",false);
    if (!configFile) {
        Serial.println("Failed to open config file");
        return;
    }
    DynamicJsonDocument doc(1024);
    doc = json_decode(configFile);
    if(doc == NULL){
        Serial.println("File is empty");
        configFile.close();
        // return;
    }
    WIFI_SSID = doc["WiFi SSID"].as<String>();
    WIFI_PASSWORD = doc["WiFi Password"].as<String>();
    IO_USERNAME = doc["MQTT SSID"].as<String>();
    IO_KEY = doc["MQTT Password"].as<String>();
    File portFile = LittleFS.open("/webserver/ports.dat","r",false);
    if (portFile){
        doc = json_decode(portFile);
        Serial.println("Decoded JSON:");
        serializeJsonPretty(doc, Serial);  
        Serial.println();
        if(doc != NULL){
            // pinMode(doc["data"]["0"]["port"],doc["data"]["0"]["mode"]);
            JsonObject data = doc["data"].as<JsonObject>();
            for (JsonPair kv : data) {
                JsonObject widget = kv.value().as<JsonObject>();
                if(widget["device"] == "RGB_PORT")
                    RGB_PORT = widget["port"];
                else if(widget["device"] == "LED_PORT")
                    LED_PORT = widget["port"];
                else if(widget["device"] == "LIGHT_PORT")
                    LIGHT_PORT = widget["port"];
                else if(widget["device"] == "MOISTURE_PORT")
                    MOISTURE_PORT = widget["port"];
                else if(widget["device"] == "RELAY_PORT")
                    RELAY_PORT = widget["port"];
                else if(widget["device"] == "ULTRASONIC_TRIG_PORT")
                    ULTRASONIC_TRIG_PORT = widget["port"];
                else if(widget["device"] == "ULTRASONIC_ECHO_PORT")
                    ULTRASONIC_ECHO_PORT = widget["port"];
                else if(widget["device"] == "MY_SCL")
                    MY_SCL = widget["port"];
                else if(widget["device"] == "MY_SDA")
                    MY_SDA = widget["port"];
            }
        }
        portFile.close();
    }
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
                if(LittleFS.exists("/webserver/ports.dat")){
                    LittleFS.remove("/webserver/ports.dat");
                }
                if(LittleFS.exists("/webserver/boards.dat")){
                    LittleFS.remove("/webserver/ports.dat");
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
    xTaskCreate(task_reset_esp,"reset_task",4096,NULL,1,NULL);
}

bool check_config(){
    load_config();
    reset_esp();
    if(WIFI_SSID.isEmpty() || IO_USERNAME.isEmpty() || IO_KEY.isEmpty()){
        initWiFiACP();
        return false;
    }
    return true;
}

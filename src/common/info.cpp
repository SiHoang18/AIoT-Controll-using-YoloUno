#include "../src/common/info.h"
constexpr char SSID_ACP[] = "HELLO";

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
    configFile.close();
}
void initWiFiACP(){
    initNeoPixel();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID_ACP);
    Serial.print("Wifi ACP Address: ");
    Serial.println(WiFi.softAPIP().toString());
    initConfigServer();
}

void task_reset_esp(void *pvParameter) {
    unsigned long buttonPressStart = 0;
    // pinMode(BOOT,INPUT);

    while (true) {
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
        }else{
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
    load_config();
    reset_esp();
    if(WIFI_SSID.isEmpty() || WIFI_PASSWORD.isEmpty() || IO_USERNAME.isEmpty() || IO_KEY.isEmpty()){
        initWiFiACP();
        return false;
    }
    return true;
}

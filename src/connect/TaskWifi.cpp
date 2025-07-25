#include "TaskWifi.h"
bool reconnect = false;
String WIFI_SSID = "";
String WIFI_PASSWORD = "";
void InitWiFi()
{
    xTaskCreate(wifi_task,"wifi_sta",4096,NULL,1,NULL);
}
void wifi_task(void *pvParameter){
 if (WIFI_SSID.isEmpty() && WIFI_PASSWORD.isEmpty())
    {
        vTaskDelete(NULL);
    }

    WiFi.mode(WIFI_STA);
    if(WIFI_PASSWORD.isEmpty())
        WiFi.begin(WIFI_SSID);
    else
        WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
    #ifdef M5_CORE2
        M5.update();
    #endif
        attempts++;
        if (WiFi.status() == WL_CONNECT_FAILED) {
            Serial.println("WiFi connect failed: wrong password or AP not found.");
            vTaskDelete(NULL);
            return;
        }
        vTaskDelay(WIFI_DELAY_TIME / portTICK_PERIOD_MS);
    }
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Failed to connect to WiFi. Please check your credentials.");
        vTaskDelete(NULL);
        return;
    }
    String ipAddress = WiFi.localIP().toString();
    Serial.println("Connected to WiFi");
    Serial.println(ipAddress);

    uint8_t mac[6];
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, mac);
    if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  mac[0], mac[1], mac[2],
                  mac[3], mac[4], mac[5]);
    } else {
        Serial.println("Failed to read MAC address");
    }
    while (true)
    {
    #ifdef M5_CORE2
        M5.update();
    #endif
        if (WiFi.status() != WL_CONNECTED)
        {
            if (!reconnect)
            {
                reconnect = true;
                WiFi.disconnect();
                WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());
                Serial.println("Reconnecting to WiFi...");
            }
        }
        else if (reconnect)
        {
            reconnect = false;
        }
        vTaskDelay(WIFI_DELAY_TIME / portTICK_PERIOD_MS);
    }
}
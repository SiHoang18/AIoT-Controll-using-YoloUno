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
        attempts++;
        if (WiFi.status() == WL_CONNECT_FAILED) {
            Serial.println("WiFi connect failed: wrong password or AP not found.");
            vTaskDelete(NULL);
            return;
        }
        vTaskDelay(WIFI_DELAY_TIME / portTICK_PERIOD_MS);
    }

    String ipAddress = WiFi.localIP().toString();
    Serial.println("Connected to WiFi");
    Serial.println(ipAddress);


    while (true)
    {
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
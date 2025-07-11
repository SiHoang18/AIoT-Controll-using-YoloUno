
#include "TaskMQTT.h"

#define MQTT_SERVER "io.adafruit.com"
#define MQTT_PORT 1883

String IO_USERNAME = ""; // Replace with your Adafruit IO username
String IO_KEY = ""; // Replace with your Adafruit IO key

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
    String message;
    for (unsigned int i = 0; i < length; i++)
    {
        message += (char)payload[i];
    }
    if (strcmp(topic, (String(IO_USERNAME) + "/feeds/feed_2").c_str()) == 0)
    {
        Serial.println(message);
    }
    else if (strcmp(topic, (String(IO_USERNAME) + "/feeds/feed_3").c_str()) == 0)
    {
        Serial.println(message);
    }
}

void publishData(String feed, String data)
{
    String topic = String(IO_USERNAME) + "/feeds/" + feed;
    if (client.connected())
    {
        client.publish(topic.c_str(), data.c_str());
    }
}

void InitMQTT()
{
    Serial.println("Connecting to MQTT...");
    String clientId = "ESP32Client" + String(random(0, 1000));
    if (client.connect(clientId.c_str(), IO_USERNAME.c_str(), IO_KEY.c_str()))
    {
        Serial.println("MQTT Connected");
        client.subscribe((String(IO_USERNAME) + "/feeds/feed_2").c_str());
        client.subscribe((String(IO_USERNAME) + "/feeds/feed_3").c_str());

        String data = "hello";
        publishData("feed_100", data);
        Serial.println("Start");
        publishData("wifi",WiFi.localIP().toString());
    }
    else
    {
        Serial.print("MQTT connection failed, rc=");
        Serial.println(client.state());
        delay(1000);
    }
}

void reconnectMQTT(void *pvParameter)
{
    while ((1))
    {
        if (client.connected())
        {
            client.loop();
        }
        else
        {
            InitMQTT();
        }
        vTaskDelay(MQTT_DELAY_TIME);
    }
    
}

void initMQTT()
{
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(callback);
    xTaskCreate(reconnectMQTT,"mqtt_reconnect",4096,NULL,1,NULL);
}
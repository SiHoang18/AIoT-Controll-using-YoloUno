#include "../src/connect/ServerWeb.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT) {
    Serial.printf("WebSocket client connected: %u\n", client->id());
  } else if(type == WS_EVT_DISCONNECT) {
    Serial.printf("WebSocket client disconnected: %u\n", client->id());
  } else if(type == WS_EVT_DATA) {
    // Serial.printf("WebSocket data received from client %u: ", client->id());
    receiveData(arg, data, len);
  } else if(type == WS_EVT_ERROR) {
    Serial.printf("WebSocket error: %s\n", (char*)data);
  }
}
void sendSensorData(void *pvParameters)
{
  while(1){
    String json = "{";
    json += "\"temperature\": " + temperature + ",";
    json += "\"humidity\": " + humidity + ",";
    json += "\"distance\": " + distance + ",";
    json += "\"moisture\": " + moistureValue + ",";
    json += "\"light\": " + light;
    json += "}";
    ws.textAll(json);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}
void receiveData(void *arg, uint8_t *data, size_t len)
{
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, data, len);
  if (error) {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return;
  }
  String title = doc["title"];
  String mode = doc["mode"];
  String state = doc["state"];
  // Serial.printf("Received data: title=%s, mode=%s, state=%s\n", title.c_str(), mode.c_str(), state.c_str());
  if(title == "LED RGB"){
    if(mode == "UNACTIVATE"){
      ledMode = "UNACTIVATE";
    }else if(mode == "AUTOMATIC MODE"){
      ledMode = "AUTOMATIC";
    } else if(mode == "MANUAL MODE"){
      ledMode = "MANUAL";
      String color = doc["color"];
      colorValue = color;
    }
  }else if(title == "LED"){
    if(mode == "UNACTIVATE"){
      ledState = "UNACTIVATE";
    }else if(mode == "ACTIVATE"){
      ledState = "ACTIVATE";
    }
  }
}
void initServerWeb()
{
  Serial.println("Starting web server...");  

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/styles.css", "text/css");
  });
  server.on("/server.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/server.js", "application/javascript");
  });
  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.js", "application/javascript");
  });
  server.begin();
  Serial.println("Web server started at IP: ");

  xTaskCreate(sendSensorData, "sendSensorData", 2048, NULL, 1, NULL);
}
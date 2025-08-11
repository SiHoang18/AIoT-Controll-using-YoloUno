#include "../src/connect/ServerWeb.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// WebSocket Event Handler
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client connected: %u\n", client->id());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client disconnected: %u\n", client->id());
      break;
    case WS_EVT_DATA:
      receiveData(arg, data, len);
      break;
    case WS_EVT_ERROR:
      Serial.printf("WebSocket error: %s\n", (char*)data);
      break;
  }
}

// Periodic Sensor Data Broadcast
void sendSensorData(void *pvParameters) {
  while (true) {
    String json = "{";
    json += "\"led\": \"" + ledState + "\",";
    json += "\"ledrgb\": \"" + ledMode + "\",";
    json += "\"temperature\": " + String(temperature) + ",";
    json += "\"humidity\": " + String(humidity) + ",";
    json += "\"distance\": " + String(distance) + ",";
    json += "\"moisture\": " + String(moistureValue) + ",";
    json += "\"light\": " + String(light);
    json += "}";

    ws.textAll(json);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

// WebSocket Data Receiver
void receiveData(void *arg, uint8_t *data, size_t len) {
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, data, len);

  if (error) {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return;
  }
  String msg = "";
  for (size_t i = 0; i < len; i++) {
    msg += (char)data[i];
  }
  Serial.println("WebSocket message: " + msg);

  String title = doc["title"] | "";
  String mode  = doc["mode"] | "";
  String state = doc["state"] | "";

  if (title == "LED_RGB") {
    if (mode == "UNACTIVATE") {
      ledMode = "UNACTIVATE";
    } else if (mode == "AUTOMATIC MODE") {
      ledMode = "AUTOMATIC";
    } else if (mode == "MANUAL MODE") {
      ledMode = "MANUAL";
      colorValue = doc["color"] | "";
    }
  } 
  else if (title == "LED") {
    if(doc["mac_address"].as<String>() == "")
      ledState = (doc["mode"] == "ACTIVATE") ? "ACTIVATE" : "UNACTIVATE";
    else{
      uint8_t mac[6];
      const char* macStr = doc["mac_address"].as<const char*>();
      if (!parseMacAddress(macStr, mac)) {
        Serial.print("Invalid MAC address format: ");
        Serial.println(macStr);
        return;
      }
      if (!esp_now_is_peer_exist(mac)) {
        addESPBoard(mac);
      }
      struct_message msg;
      msg.msg = doc["state"].as<String>();
      ESPNowSendTask(mac,msg);
    }
  } 
  else if (title == "ports") {
    File file = LittleFS.open("/webserver/ports.dat", "w", true);
    if (!file) {
      Serial.println("Failed to open ports.dat for writing");
      return;
    }
    serializeJson(doc, file);
    file.close();
    ESP.restart();
  } 
  else if (title == "update-board") {
    File file = LittleFS.open("/webserver/boards.dat", "w", true);
    if (!file) {
      Serial.println("Failed to open boards.dat for writing");
      return;
    }
    serializeJson(doc["data"], file);
    file.close();
    JsonArray boards = doc["data"].as<JsonArray>();
    for (JsonObject board : boards) {
      const char* macStr = board["mac_address"];
      if (macStr == nullptr || strlen(macStr) == 0) {
        continue;
      }
      uint8_t mac[6];
      if (!parseMacAddress(macStr, mac)) {
        Serial.print("Invalid MAC address format: ");
        Serial.println(macStr);
        continue;
      }
      if (!esp_now_is_peer_exist(mac)) {
        addESPBoard(mac);
      }
    }
  }
}

void initServerWeb() {
  Serial.println("Starting web server...");

  ws.onEvent(onEvent);
  server.addHandler(&ws);

  server.serveStatic("/", LittleFS, "/webserver/").setDefaultFile("index.html");
  server.serveStatic("/ports.dat", LittleFS, "/webserver/ports.dat");

  ElegantOTA.begin(&server);

  server.begin();
  Serial.println("Web server started at IP: ");
  
  // xTaskCreate(sendSensorData, "sendSensorData", 2048, NULL, 1, NULL);
}

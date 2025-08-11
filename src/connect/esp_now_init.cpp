#include "../src/connect/esp_now_init.h"
#define SEND_MODE
// #define RECV_MODE  
String success;
// uint8_t receive_mac[6] = {0x9c,0x9e,0x6e,0x15,0x95,0xb4};

#ifdef RECV_MODE
struct_message recv_msg;
String recv_temp;
String recv_humidity;
#endif 

#ifdef SEND_MODE
struct_message send_msg;
String send_temp;
String send_humidity;
String msg;
#endif

esp_now_peer_info_t peerInfo;

#ifdef SEND_MODE
volatile esp_now_send_status_t lastSendStatus = ESP_NOW_SEND_FAIL;
volatile bool sendCompleted = false;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  lastSendStatus = status;
  sendCompleted = true;
}

bool ESPNowSendTask(uint8_t *receive_mac, struct_message msg) {
  const int MAX_RETRIES = 20;
  int attempt = 0;

  sendCompleted = false;  
  esp_now_register_send_cb(OnDataSent); 

  while (attempt < MAX_RETRIES) {
    sendCompleted = false;
    esp_err_t result = esp_now_send(receive_mac, (uint8_t *)&msg, sizeof(msg));

    if (result != ESP_OK) {
      Serial.println("❌ esp_now_send() failed to queue message");
      return false;  
    }

    unsigned long start = millis();
    while (!sendCompleted && millis() - start < 100);

    if (lastSendStatus == ESP_NOW_SEND_SUCCESS) {
      Serial.printf("✅ Send succeeded on attempt #%d\n", attempt + 1);
      return true;
    } else {
      Serial.printf("⚠️ Send failed on attempt #%d\n", attempt + 1);
      attempt++;
      delay(50); 
    }
  }

  Serial.println("❌ Failed to send after 20 attempts.");
  return false;
}

bool parseMacAddress(const char* macStr, uint8_t* mac) {
  return sscanf(macStr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                &mac[0], &mac[1], &mac[2],
                &mac[3], &mac[4], &mac[5]) == 6;
}

bool addESPBoard(uint8_t* receive_mac) {
  if (esp_now_is_peer_exist(receive_mac)) {
    Serial.println("Peer already exists");
    return true;
  }
  memcpy(peerInfo.peer_addr, receive_mac, 6);
  peerInfo.channel = (WiFi.status() == WL_CONNECTED) ? WiFi.channel() : 1;  

  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return false;
  }

  Serial.println("Peer added successfully");
  return true;
}

#endif
#ifdef RECV_MODE
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&recv_msg, incomingData, sizeof(recv_msg));
  Serial.print("Bytes received: ");
  Serial.println(len);
  recv_temp = recv_msg.temperature;
  recv_humidity = recv_msg.humidity;
  Serial.println(WiFi.channel());
}
#endif

void initESPNow(){
    WiFi.mode(WIFI_AP_STA);
    if(esp_now_init() != ESP_OK){
        Serial.println("Error initializing ESP-NOW");
        return;
    }
#ifdef SEND_MODE
    esp_now_register_send_cb(OnDataSent);
#endif
    // memcpy(peerInfo.peer_addr, receive_mac, 6);
    // peerInfo.channel = (WiFi.status() == WL_CONNECTED) ? WiFi.channel() : 1;  
    // peerInfo.encrypt = false;
         
    // if (esp_now_add_peer(&peerInfo) != ESP_OK){
    //     Serial.println("Failed to add peer");
    //     return;
    // }
#ifdef RECV_MODE
    esp_now_register_recv_cb(OnDataRecv);
#endif
#ifdef SEND_MODE
    // xTaskCreate(ESPNowSendTask,"send_task",4096,NULL,1,NULL);
#endif
}
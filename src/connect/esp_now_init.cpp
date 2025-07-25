#include "../src/connect/esp_now_init.h"
#define SEND_MODE
// #define RECV_MODE  
String success;
uint8_t receive_mac[6] = {0x48, 0xCA, 0x43, 0xE5, 0xC2, 0xF4};

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
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}
void ESPNowSendTask(void *pvParameters){
  while (1)
  {
    send_humidity = (String)(rand() % 100 + 60);
    send_temp = (String)(rand() % 100 + 30);
    msg = (msg == "ON") ? "OFF" : "ON";
    send_msg.temperature = send_temp;
    send_msg.humidity = send_humidity;
    send_msg.msg = msg;
    esp_err_t result = esp_now_send(receive_mac, (uint8_t *) &send_msg, sizeof(send_msg));
    if (result == ESP_OK) {
        Serial.println("Sent with success");
    }
    else {
        Serial.println("Error sending the data");
    }
    vTaskDelay(1000);
  }
}
#endif
#ifdef RECV_MODE
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&recv_msg, incomingData, sizeof(recv_msg));
  Serial.print("Bytes received: ");
  Serial.println(len);
  recv_temp = recv_msg.temperature;
  recv_humidity = recv_msg.humidity;
}
#endif

void initESPNow(){
    WiFi.mode(WIFI_STA);
    if(esp_now_init() != ESP_OK){
        Serial.println("Error initializing ESP-NOW");
        return;
    }
#ifdef SEND_MODE
    esp_now_register_send_cb(OnDataSent);
#endif
    memcpy(peerInfo.peer_addr, receive_mac, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
         
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
#ifdef RECV_MODE
    esp_now_register_recv_cb(OnDataRecv);
#endif
#ifdef SEND_MODE
    xTaskCreate(ESPNowSendTask,"send_task",4096,NULL,1,NULL);
#endif
}
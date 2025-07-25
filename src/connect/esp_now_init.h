#ifndef INC_ESP_NOW_H
#define INC_ESP_NOW_H
#include <globals.h>
typedef struct struct_message{
    int id;
    String temperature;
    String humidity;
    String msg;
} struct_message;

extern uint8_t receive_mac[6];

extern void initESPNow();
void ESPNowSendTask(void *pvParameters);
void ESPNowRecvTask(void *pvParameters);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
#endif
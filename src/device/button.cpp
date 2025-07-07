#include "button.h"
#define BUTTON_PORT_1 6
#define BUTTON_PORT_2 7
int buttonFlag1 = 0;
int buttonFlag2 = 0;

void initButton(){
    pinMode(BUTTON_PORT_1, INPUT); 
    pinMode(BUTTON_PORT_2,INPUT);
    xTaskCreate(buttonTask, "Button Task", 1024, NULL, 1, NULL);
}
void buttonTask(void *pvParameters){
    while(1){
        if(digitalRead(BUTTON_PORT_1) == LOW){
            buttonFlag1 = 1;
        } else if(digitalRead(BUTTON_PORT_2) == LOW){
            buttonFlag2 = 1;
        }
        vTaskDelay(50); 
    }
} 
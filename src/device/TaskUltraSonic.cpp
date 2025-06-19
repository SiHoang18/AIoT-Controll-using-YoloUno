#include "TaskUltraSonic.h"
#define TRIGGER_PIN 18
#define ECHO_PIN 21
#define DelayTime 3000
String distance = "0";
void initUltrasonic()
{
    pinMode(TRIGGER_PIN, OUTPUT); 
    pinMode(ECHO_PIN, INPUT); 
    xTaskCreate(getValueUltrasonic, "getValueUltrasonic", 4096, NULL, 1, NULL);
}
void getValueUltrasonic(void *pvParameters){
    while (1)
    {
        unsigned long duration; 
        int dist;           
        
        digitalWrite(TRIGGER_PIN,0);  
        delayMicroseconds(2);
        digitalWrite(TRIGGER_PIN,1);   
        delayMicroseconds(5);   
        digitalWrite(TRIGGER_PIN,0);   
        duration = pulseIn(ECHO_PIN,HIGH);  
       
        dist = int(duration/2/29.412);
        Serial.print(dist);
        Serial.println("cm");
        distance = String(dist);
        publishData("ultrasonic", String(dist));
        vTaskDelay( DelayTime / portTICK_PERIOD_MS);
    }
}

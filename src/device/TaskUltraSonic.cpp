#include "TaskUltraSonic.h"

#define DelayTime 3000
String distance = "0";
void initUltrasonic()
{
    pinMode(ULTRASONIC_TRIG_PORT, OUTPUT); 
    pinMode(ULTRASONIC_ECHO_PORT, INPUT); 
    xTaskCreate(getValueUltrasonic, "getValueUltrasonic", 4096, NULL, 1, NULL);
}
void getValueUltrasonic(void *pvParameters){
    while (1)
    {
        unsigned long duration; 
        int dist;           
        
        digitalWrite(ULTRASONIC_TRIG_PORT,0);  
        delayMicroseconds(2);
        digitalWrite(ULTRASONIC_TRIG_PORT,1);   
        delayMicroseconds(5);   
        digitalWrite(ULTRASONIC_TRIG_PORT,0);   
        duration = pulseIn(ULTRASONIC_ECHO_PORT,HIGH);  
       
        dist = int(duration/2/29.412);
        Serial.print(dist);
        Serial.println("cm");
        distance = String(dist);
        publishData("ultrasonic", String(dist));
        vTaskDelay( DelayTime / portTICK_PERIOD_MS);
    }
}

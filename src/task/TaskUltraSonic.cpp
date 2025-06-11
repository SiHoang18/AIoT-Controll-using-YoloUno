#include "TaskUltraSonic.h"

void initUltrasonic()
{
    pinMode(18, OUTPUT); // Trigger pin
    pinMode(21, INPUT);  // Echo pin
    xTaskCreate(getValueUltrasonic, "getValueUltrasonic", 4096, NULL, 1, NULL);
}
void getValueUltrasonic(void *pvParameters)
{
    int trigger = 18;
    int echo = 21;
    while (1)
    {
        unsigned long duration; 
        int distance;           
        
        digitalWrite(trigger,0);  
        delayMicroseconds(2);
        digitalWrite(trigger,1);   
        delayMicroseconds(5);   
        digitalWrite(trigger,0);   
        duration = pulseIn(echo,HIGH);  
       
        distance = int(duration/2/29.412);
        Serial.print(distance);
        Serial.println("cm");
        publishData("ultrasonic", String(distance));
        vTaskDelay(4000 / portTICK_PERIOD_MS);
    }
}

#include "TaskUltraSonic.h"
// Create ultrasonic sensor instance

// Ultrasonic ultrasonic(GPIO_NUM_8, GPIO_NUM_9);
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
    // Ultrasonic ultrasonic(trigger, echo);
    while (1)
    {
        unsigned long duration; // biến đo thời gian
        int distance;           // biến lưu khoảng cách
        
        /* Phát xung từ chân trig */
        digitalWrite(trigger,0);   // tắt chân trig
        delayMicroseconds(2);
        digitalWrite(trigger,1);   // phát xung từ chân trig
        delayMicroseconds(5);   // xung có độ dài 5 microSeconds
        digitalWrite(trigger,0);   
        duration = pulseIn(echo,HIGH);  
        // Tính khoảng cách đến vật.
        distance = int(duration/2/29.412);
        
        /* In kết quả ra Serial Monitor */
        Serial.print(distance);
        Serial.println("cm");
        publishData("ultrasonic", String(distance));
        vTaskDelay(4000 / portTICK_PERIOD_MS); // Delay 1 second
    }
}

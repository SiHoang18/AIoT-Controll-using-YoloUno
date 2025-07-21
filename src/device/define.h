#ifndef INC_DEFINE_H
#define INC_DEFINE_H
#include<globals.h>
//Define Port
// #define BOOT 0
// #define LED_PORT 48
// #define NEOPIXEL_PORT 45
// #define RGB_PORT 6
// #define LIGHT_PORT 2
// #define MOISTURE_PORT 1
// #define RELAY_PORT 8
// #define FAN_PORT 10
// #define ULTRASONIC_TRIG_PORT 18
// #define ULTRASONIC_ECHO_PORT 21
// #define MY_SCL 11
// #define MY_SDA 12
extern int BOOT;
extern int LED_PORT;
extern int NEOPIXEL_PORT;
extern int RGB_PORT;
extern int LIGHT_PORT;
extern int MOISTURE_PORT;
extern int RELAY_PORT;
extern int FAN_PORT;
extern int ULTRASONIC_TRIG_PORT;
extern int ULTRASONIC_ECHO_PORT;
extern int MY_SCL;
extern int MY_SDA;
//Time Delay
#define WIFI_DELAY_TIME 500
#define MQTT_DELAY_TIME 500
#define BOOT_TIME 5000
#define RESET_DELAY_TIME 300
#endif
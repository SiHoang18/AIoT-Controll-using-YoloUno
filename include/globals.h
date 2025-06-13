#ifndef GLOBALS_H
#define GLOBALS_H

// include libraries
#include <Wire.h>
#include <WiFi.h>
#include <DHT20.h>
#include <PubSubClient.h>
#include <Ultrasonic.h>
#include <Arduino.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>
///
#include "../src/connect/TaskWifi.h"
#include "../src/connect/TaskMQTT.h"
#include "../src/task/TaskUltraSonic.h"
#include "../src/device/TaskDHT20.h"
#include "../src/task/ServerWeb.h"
#include "../src/device/LedRgb.h"
#include "../src/device/taskMiniFan.h"
#include "../src/device/LightSensor.h"
#endif
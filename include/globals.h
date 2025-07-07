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
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <SPIFFS.h>
#include <LittleFS.h>
///
#include "../src/device/define.h"
#include "../src/device/device.h"
#include "../src/connect/TaskWifi.h"
#include "../src/connect/TaskMQTT.h"
#include "../src/connect/connect_init.h"
#include "../src/device/TaskUltraSonic.h"
#include "../src/device/TaskDHT20.h"
#include "../src/connect/ServerWeb.h"
#include "../src/device/LedRgb.h"
#include "../src/device/Led.h"
#include "../src/device/Relay.h"
#include "../src/device/taskMiniFan.h"
#include "../src/device/NeoPixel.h"
#include "../src/device/LightSensor.h"
#include "../src/device/LCD.h"
#include "../src/device/MoistureSensor.h"
#include "../src/device/button.h"
#include "../src/common/info.h"
#include "../src/common/configServer.h"

///
#include "../src/task/FanRelayTask.h"
#include "../src/task/rgbTask.h"
#endif
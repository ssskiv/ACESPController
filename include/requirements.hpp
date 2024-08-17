#ifndef REQS
#define REQS

// * dependencies for all functions
#include "pinsAndMore.hpp"
#include <Arduino.h>

// * dependencies for IR and EEPROM systems
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <EEPROM.h>
#include <IRsend.h>

// * dependencies for Wi-Fi AP functions
// #include <WiFi.h>
// #include <DNSServer.h>
// #include <WebServer.h>

// * LCD 
#include <LiquidCrystal_I2C.h>

// * Sensors
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

// * Ethernet
#include <Ethernet.h>
#include <SPI.h>

#endif
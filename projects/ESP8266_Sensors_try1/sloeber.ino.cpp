#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-01-16 21:38:52

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include "OLED.h"
#include <OneWire.h>
#include "DallasTemperature.h"
#include "BeeperB.h"
#include "ButtonB.h"
#include "LedB.h"
#include "TimeTrigger.h"
#include <RtcDS3231.h>
#include "SensorsDS18_helper.h"

void setup ( void ) ;
void loop ( void ) ;
void setupButton();
void onOkButtonReleased();
void onOkButtonPressed();
String sensorsToHtml();
void refreshSensors();
int initSensors();
int getSensorscount();
void setupDisplay();
void setupWiFi();
String getDateTimeStr();
String getDateStr();
String getTimeStr();
void handleRoot() ;
void handleJson();
void handleNotFound() ;
void drawGraph() ;

#include "ESP8266_Sensors.ino"


#endif

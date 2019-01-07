#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-12-31 20:34:40

#include "Arduino.h"
#define DEBUG_ESP_CORE 1
#include <ESP8266WiFi.h>
#include "ESP8266WebServer.h"
#include <stdio.h>

void setup() ;
void onStationModeConnected(const WiFiEventStationModeConnected& evt);
void onStationModeDHCPTimeout();
void onStationModeDisconnected(const WiFiEventStationModeDisconnected& evt);
void onStationModeGotIP(const WiFiEventStationModeGotIP& evt);
void onTestHandler();
void onStationConnected(const WiFiEventSoftAPModeStationConnected& evt) ;
void onStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& evt) ;
void onProbeRequestPrint(const WiFiEventSoftAPModeProbeRequestReceived& evt) ;
void onProbeRequestBlink(const WiFiEventSoftAPModeProbeRequestReceived&) ;
void loop() ;
String macToString(const unsigned char* mac) ;


#include "WiFiWebServer.ino"

#endif

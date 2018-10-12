#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-10-12 18:04:37

#include "Arduino.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "EspSettingsBox.h"
#include "AbstractEvent.h"
#include <ESP8266WebServer.h>
#include "PinDigital.h"
#include "PinEvent.h"
#include "TimeTrigger.h"
#include "BH1750.h"

void clearDisplay();
void displayLineFunction(String message,int row,int column);
void ledOnFunction();
void ledOffFunction();
void setupServerUrlsFunction(ESP8266WebServer server);
PinEvent processEvent(PinEvent event);
void sendEventToTopic(PinEvent event);
void setup() ;
void loop() ;
void measureLm();
void sendData() ;
boolean publishToTopic(const char *topic, String payload);
void callback(char* topic, uint8_t* payload, unsigned int length) ;
void processHttpCommand();
void processMqtt();
boolean isMqttConnected();
void connectMqttIfNotConnected();
void subscribeToTopic(const char *topicIn);
void setupWiFi();
void connectWifiIfNotConnected();
boolean isWiFIConnected();
void connectWiFi();
void setupServer();

#include "MQTT_Switch_ON_OFF.ino"


#endif

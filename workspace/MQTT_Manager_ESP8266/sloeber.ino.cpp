#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-09-30 04:48:07

#include "Arduino.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

void setup() ;
void sendData() ;
void callback(char* topic, uint8_t* payload, unsigned int length) ;
void loop() ;
void processMqtt();
void connectMqtt();
void subscribeToTopic(const char *topicIn);

#include "MQTT_Manager_ESP8266.ino"


#endif

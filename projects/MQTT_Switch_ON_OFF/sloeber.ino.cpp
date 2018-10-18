#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-10-18 18:03:52

#include "Arduino.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "EspSettingsBox.h"
#include "MqttHelper.h"
#include "Looper.h"
#include "Loopable.h"
#include "PinEventProcessor.h"
#include "I2Chelper.h"
#include <Wire.h>
#include <ESP8266WebServer.h>
#include "PinDigital.h"
#include "PinEvent.h"
#include "TimeTrigger.h"
#include "PinExternalFuncUint16t.h"
#include <BME280_Measurer.h>
#include <BH1750_Measurer.h>

void setup() ;
void loop() ;
PinEvent onLampLeftChanged(PinEvent event);
PinEvent onLampRightChanged(PinEvent event);
PinEvent onButtonLeftChanged(PinEvent event);
PinEvent onButtonRightChanged(PinEvent event);
PinEvent onIrChanged(PinEvent event);
void measureSensors();
void setupWiFi();
boolean isWiFIConnected();
void checkWiFi();
void setupServer();
void processHttpCommand();
void processTestCommand();
PinEvent onButtonChanged(PinEvent event);
void onMovementDetected();
void onNoMovementDetected();
void onHumanPresentTrigger();
void handleLightOnOff();
PinEvent onPirWasChanged(PinEvent event);
PinEvent processEvent(PinEvent event);
void publishEvent(PinEvent event);
void signalIrDetection();
void processMqttEvent(String topic,String message);
void callback(char* topic, uint8_t* payload, unsigned int length) ;

#include "MQTT_Switch_ON_OFF.ino"


#endif

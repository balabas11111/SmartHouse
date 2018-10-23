#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-10-23 18:54:15

#include "Arduino.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "EspSettingsBox.h"
#include "MqttHelper.h"
#include "Loopable.h"
#include "PinEventProcessor.h"
#include "FS.h"
#include "I2Chelper.h"
#include "DisplayHelper.h"
#include "WiFiHelper.h"
#include <Wire.h>
#include <ESP8266WebServer.h>
#include "PinDigital.h"
#include "PinEvent.h"
#include "TimeTrigger.h"
#include "PinExternalFuncUint16t.h"
#include <BME280_Measurer.h>
#include <BH1750_Measurer.h>
#include <DeviceHelper.h>
#include "ConfigStorage.h"
#include "ConfigStorageWidget.h"
#include "PinDigitalWidget.h"
#include "PirDetector.h"

void setup() ;
void loop() ;
void postInitWebServer();
void handleHttpWidget();
PinEvent onButtonChanged(PinEvent event);
PinEvent processEvent(PinEvent event);
void processMqttEvent(String topic,String message);

#include "MQTT_Switch_ON_OFF.ino"


#endif

#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-10-14 16:35:05

#include "Arduino.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "EspSettingsBox.h"
#include "MqttHelper.h"
#include "Looper.h"
#include "Loopable.h"
#include "I2Chelper.h"
#include <Wire.h>
#include <ESP8266WebServer.h>
#include "PinDigital.h"
#include "PinEvent.h"
#include "TimeTrigger.h"
#include "PinExternalFuncUint16t.h"
#include "BH1750.h"
#include <Adafruit_BME280.h>

void clearDisplay();
void displayLineFunction(String message,int row,int column);
void ledOnFunction();
void ledOffFunction();
void setupServerUrlsFunction(ESP8266WebServer server);
void setup() ;
void loop() ;
void setupBme();
void measureBme280();
void setupLuxMeter();
uint16_t getLuxVal();
void setupWiFi();
boolean isWiFIConnected();
void checkWiFi();
void setupServer();
void processHttpCommand();
void processTestCommand();
PinEvent processEvent(PinEvent event);
void handleHuman(uint8_t val);
void onMovementDetected();
void onNoMovementDetected();
void onHumanPresentTrigger();
void handleLightOnOff();
void signalIrDetection();
void processMqttEvent(String topic,String message);

#include "MQTT_Switch_ON_OFF.ino"


#endif

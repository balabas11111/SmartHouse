#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-12-25 19:59:50

#include "Arduino.h"
#include <Arduino.h>
#include <BH1750_Sensor.h>
#include <BME280_Sensor.h>
#include <Hash.h>
#include <ESP8266WiFi.h>
#include "EspSettingsBox.h"
#include "Loopable.h"
#include "FS.h"
#include "I2Chelper.h"
#include "WiFiHelper.h"
#include <Wire.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "TimeTrigger.h"
#include <DeviceHelper.h>
#include <ThingSpeakHelper.h>
#include <PinDigital.h>
#include <Pir_Sensor.h>
#include "BH1750_Sensor.h"
#include "BME280_Sensor.h"
#include "DHT22_Sensor.h"
#include "DS18D20_Sensor.h"
#include "StatusMessage.h"
#include "PinDigitalVirtual.h"

void setup() ;
void loop() ;
void postInitWebServer();
void measureSensors();
void onLeftButtonChanged();
void onRightButtonChanged();
void onAcMeterChanged();
void setLampValue(PinDigital* lamp,uint8_t on);
void onLeftLampChanged();
void onRightLampChanged();
void onPirDetectorChanged();
void loadSensors();
void saveSensors();
String setSensorJson();
String getAllSensorsJson();
String executeCommand();
void executePostPonedCommand();
String setEspSettingsBoxValues();
void processThingSpeakPost();
void sendAbstractItemToHttp(AbstractItem* item);

#include "MQTT_Switch_ON_OFF.ino"


#endif

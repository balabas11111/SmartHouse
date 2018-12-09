#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-12-10 01:24:57

#include "Arduino.h"
#include "Arduino.h"
#include <Hash.h>
#include <ESP8266WiFi.h>
#include "EspSettingsBox.h"
#include "Loopable.h"
#include "FS.h"
#include "I2Chelper.h"
#include "WiFiHelper.h"
#include <Wire.h>
#include "NtpTimeClientService.h"
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "TimeTrigger.h"
#include <DeviceHelper.h>
#include <DisplayHelperAbstract.h>
#include <ThingSpeakHelper.h>
#include <PinDigital.h>
#include <Pir_Sensor.h>
#include <BeeperB.h>
#include "DS18D20_Sensor.h"
#include "StatusMessage.h"
#include "TM1637.h"

void setup() ;
void loop() ;
void processTimeClientEvent(int8_t* time);
void postInitWebServer();
void measureSensors();
void onMenuButtonChanged();
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

#include "ESP8266_Ntp_Pir.ino"


#endif

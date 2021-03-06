#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2019-01-08 00:44:21

#include "Arduino.h"
#include "Arduino.h"
#include <Hash.h>
#include "EspSettingsBox.h"
#include <ESPSett_Alarm.h>
#include <ESPSett_Display.h>
#include <ESPSett_Ntp.h>
#include <ESPSett_Telegram.h>
#include "FS.h"
#include <Wire.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WiFi.h>
#include "JSONprovider.h"
#include "Loopable.h"
#include "I2Chelper.h"
#include "WiFiHelper.h"
#include "Time/TimeIntervalService.h"
#include "Time/NtpTimeClientService.h"
#include "TimeTrigger.h"
#include <DeviceHelper.h>
#include <Display_Pageable/DisplayHelperAbstract.h>
#include <ThingSpeakHelper.h>
#include <PinDigital.h>
#include <Pir_Sensor.h>
#include <BeeperB.h>
#include "DS18D20_Sensor.h"
#include "BME280_Sensor.h"
#include "StatusMessage.h"
#include "Display_Custom/DisplayHelper_TM1637_Clock_PIR.h"
#include "TM1637.h"

void initComponents();
void setup() ;
void loop() ;
void playPostInitSounds();
void postInitTimeIntervalService();
void postInitWebServer();
ESP8266WebServer* getServer();
void onButtonMenuChanged();
void onPirDetectorChanged();
void updateSensors();
void processTimeIntervals();
void printPir();
void loadSensors();
void saveSensors();
String setSensorJson();
String getAllSensorsJson();
String getAllProvidersJson();
String executeCommand();
void executePostPonedCommand();
void executeThingSpeakPost();
void sendAbstractItemToHttp(AbstractItem* item);
void processThingSpeakPost();
void changeSymbol();

#include "ESP8266_Ntp_Pir.ino"


#endif

#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-11-01 23:04:07

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
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
#include "DHT.h"
#include "FS.h"
#include "KeyValue.h"
#include "EspSettingsBox.h"

void setup ( void ) ;
void loop ( void ) ;
void checkRestartHandled();
void initSettings();
void refreshTimeTriggers();
void setupButtons();
void setupButtonInterrupts();
void initFS();
void clearDisplay();
void displayHoldOn();
void initDisplay();
void turnOnDisplay();
void turnOffDisplay();
void refreshDisplay();
void displayLine(String line,uint8_t r,uint8_t c);
boolean onPageButtonChanged();
void selectNextDisplayPage();
void handlePageButton();
void startAlertMode();
void stopAlertMode();
void handleAlertNotifSound();
void sendALarmNotificationTS(String message);
void checkDisplayPageAutochange();
void handleDisplayPover();
uint8_t getMaxPossPage();
uint8_t getNextActivePage();
void refreshSensors(boolean displayDataActual);
int initSensors(boolean clearSettings);
void checkHighLowValues();
void setupWiFi();
void connectToWiFiIfNotConnected();
char* getDateTimeStr();
String getDateStrMinus();
String getDateStr();
String getTimeStr();
void handleRoot() ;
void checkAuthentication();
void getPageHtmlSettingsForm();
String processErrorsViev(String *errorString,String *warnString,String *successString);
void processSettingsRequestParams(String *vievName,String *redirectUrl,String *redirectDelay, 									String *errorString,String *warnString,String *successString) ;
void processPageRedirect(String *errorString,String *warnString,String *successString, 		String *redirect,String *redirectDelay);
void getPageHtmlRoot();
void getPageHtmlSetup();
String getFormThingSpeakSetings();
String getFormSensorsSettins();
String getFormRestart();
String getFormNetSettins();
String getFormDeviceInfo();
String getFormDeviceSettings();
String getFormAlarmSettins();
boolean sendDataToThingSpeak();
void handleJson();
void handleNotFound() ;
void scanI2C();

#include "ESP8266_Sensors.ino"


#endif

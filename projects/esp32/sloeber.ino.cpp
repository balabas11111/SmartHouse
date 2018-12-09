#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2018-12-09 22:18:21

#include "Arduino.h"
#include "Arduino.h"
#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#include "EspSettingsBox.h"
#include "ESP_Consts.h"
#include "I2Chelper.h"
#include "PinDigital.h"
#include "PinDigitalNoInt.h"
#include "DeviceHelper.h"
#include "BME280_Sensor.h"
#include "DS18D20_Sensor.h"
#include "DisplayHelperAbstract.h"
#include "WiFiHelper.h"
#include "HTTPClient.h"

void attachInterrupts();
void setup() ;
void loop() ;
void on_buttonMenuInterrupt();
void on_lampLeftInterrupt();
void on_acMeterInterrupt();
void onMenuButtonChanged();
void onLeftLampChanged();
void onAcMeterChanged();
void onAcMeter2Changed();
void measureSensors();
void measureAC();
void postInitWebServer();
String getAllSensorsJson();

#include "esp32.ino"


#endif

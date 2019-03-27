#include "Arduino.h"
#include <ArduinoJson.h>

#include <EntityAbstract.h>
#include <JsonDao.h>

#include <EntityConsts.h>
#include <ObjectUtils.h>
#include <FileUtils.h>
#include <NtpBox.h>
#include <WiFiManagerAsync.h>

#include <ServerSettingsBox.h>
#include "Bme280sensor.h"
#include "Bh1750sensor.h"
#include "DHT22sensor.h"
#include "DS18D20sensor.h"
#include "OutputPin.h"
#include "TM1637DisplayTime.h"
#include <Ticker.h>

Ticker ticker;

ServerSettingsBox conf("EntityLiv dev settings");
NtpBox ntpBox;

TM1637DisplayTime display;

Bme280sensor bme280;
Bh1750sensor bh1750;
OutputPin rele(D4,"DefaultRele");
DHT22sensor dht22(D5);
DS18D20sensor ds18d20(D6);

EntityJson* entities[] = {&bme280,&bh1750,&conf,&rele,&ntpBox,&dht22,&ds18d20};
JsonDao dao(entities, ARRAY_SIZE(entities));

WiFiManagerAsync server(&conf, &dao);

void setup()
{
ObjectUtils::initSerial();
FileUtils::init();
FileUtils::deleteAllFiles("/data/");

dao.init();
server.init();
ntpBox.startTime();


ticker.attach(1,updateDisplayTime);
ticker.attach(30,printHeap);
ticker.attach(60, updateSensors);

updateSensors();

ObjectUtils::printHeap();
ObjectUtils::printMillis();
}

void loop()
{

}

void printHeap(){
	ObjectUtils::printHeap();
}

void updateDisplayTime(){
	ntpBox.update();
	display.tick();
	display.updateTime(ntpBox.timeHm());
}

void updateSensors(){
	Serial.print(FPSTR("updateSensors"));

	bme280.update();
	bh1750.update();
	dht22.update();
	ds18d20.update();

	Serial.println(FPSTR("...done"));
}

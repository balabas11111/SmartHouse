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
#include <Ticker.h>

Ticker ticker;

ServerSettingsBox conf("EntityLiv dev settings");
NtpBox ntpBox;
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
Serial.begin(115200);
delay(5000);
Serial.println(FPSTR("Device starting"));
FileUtils::init();
//FileUtils::deleteAllFiles("/");
Serial.print(FPSTR(" heap="));
Serial.println(ESP.getFreeHeap());
dao.init();
Serial.print(FPSTR(" heap="));
Serial.println(ESP.getFreeHeap());
server.init();
ntpBox.startTime();

ticker.attach(conf.refreshInterval(), updateSensors);

Serial.print(FPSTR(" heap="));
Serial.println(ESP.getFreeHeap());
Serial.print(FPSTR("millis = "));
Serial.println(millis());
}

int i=0;

void loop()
{
	delay(30000);
	Serial.print(FPSTR("i="));
	Serial.print(i);
	Serial.print(FPSTR(" heap="));
	Serial.println(ESP.getFreeHeap());
	i++;

	ntpBox.update();

	bh1750.setField("light", i);
	bh1750.sendAsEventSourceEntity();
}

void updateSensors(){
	Serial.print(FPSTR("updateSensors"));

	bme280.update();
	bh1750.update();
	dht22.update();
	ds18d20.update();

	Serial.println(FPSTR("...done"));
}

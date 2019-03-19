#include "Arduino.h"
#include <ArduinoJson.h>

#include <EntityAbstract.h>
#include <JsonDao.h>

#include <EntityConsts.h>
#include <ObjectUtils.h>
#include <FileUtils.h>
#include <ServerSettingsBox.h>
#include <WiFiManagerAsync.h>

#include "Bme280sensor.h"
#include "Bh1750sensor.h"

ServerSettingsBox conf("EntityLiv dev settings");
Bme280sensor bme280;
Bh1750sensor bh1750;

EntityJson* entities[] = {&bme280,&bh1750,&conf};
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
Serial.print(FPSTR(" heap="));
Serial.println(ESP.getFreeHeap());
Serial.print(FPSTR("millis"));
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
	bh1750.setField("light", i);
}

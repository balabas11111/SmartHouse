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
#include "OutputPin.h"

ServerSettingsBox conf("EntityLiv dev settings");
NtpBox ntpBox;
Bme280sensor bme280;
Bh1750sensor bh1750;
OutputPin rele(D4,"DefaultRele");

EntityJson* entities[] = {&bme280,&bh1750,&conf,&rele,&ntpBox};
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


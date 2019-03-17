#include "Arduino.h"
#include <ArduinoJson.h>

#include <EntityAbstract.h>
#include <JsonDao.h>

#include <EntityConsts.h>
#include <ObjectUtils.h>
#include <FileUtils.h>
#include <ServerSettingsBox.h>
#include <WiFiManagerAsync.h>

#define BME280Descriptor "{\"data\": {\"temp\":\"-1\",\"hum\":\"-1\",\"press\":\"-1\"},\
\"model\":{\"stat\":[],\"var\":[\"temp\",\"hum\",\"press\"],\"load\":[],\"save\":[],\"set\":[],\"tvar\":[\"temp\",\"hum\"]}  }"

#define BH1750Descriptor "{\"data\": {\"light\":\"-1\"},\
\"model\":{\"stat\":[],\"var\":[\"light\"],\"load\":[],\"save\":[],\"set\":[],\"tvar\":[\"light\"]}  }"

ServerSettingsBox conf("EntityLiv dev settings");
EntityAbstract bme280(ROOT_GROUP_SENSORS,"bme280","Temperature/Humidity/AtmPressure",BME280Descriptor);
EntityAbstract bh1750(ROOT_GROUP_SENSORS,"bh1750","Light level",BH1750Descriptor);

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
dao.init();
server.init();
Serial.print(FPSTR("millis"));
Serial.println(millis());
}

void loop()
{
	delay(5000);
	Serial.println(ESP.getFreeHeap());
}

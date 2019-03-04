#include "Arduino.h"
#include <ArduinoJson.h>

#include <EntityAbstract.h>
#include <JsonDao.h>

#include <EntityConsts.h>

#define BME280Descriptor "{\"fields\": {\"temp\":\"-1\",\"hum\":\"-1\",\"press\":\"-1\"},\
\"actions\":{\"stat\":[],\"var\":[\"temp\",\"hum\",\"press\"],\"load\":[],\"save\":[],\"set\":[],\"templateVar\":[\"temp\",\"hum\"],\"eventListeners\":[]}  }"

#define BH1750Descriptor "{\"fields\": {\"light\":\"-1\"},\
\"actions\":{\"stat\":[],\"var\":[\"light\"],\"load\":[],\"save\":[],\"set\":[],\"templateVar\":[\"light\"],\"eventListeners\":[]}  }"

EntityAbstract bme280(ROOT_GROUP_SENSORS,"bme280","Temperature/Humidity/AtmPressure",BME280Descriptor);
EntityAbstract bh1750(ROOT_GROUP_SENSORS,"bh1750","Light level",BH1750Descriptor);

EntityJson* entities[] = {&bme280,&bh1750};
JsonDao dao(entities, 2);

void setup()
{
Serial.begin(115200);
delay(5000);
Serial.println(FPSTR("Device starting"));
dao.init();
Serial.print(FPSTR("millis"));
Serial.println(millis());
}

void loop()
{
	delay(5000);
	Serial.println(ESP.getFreeHeap());
}

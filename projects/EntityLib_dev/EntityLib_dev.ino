#include "Arduino.h"
#include <ArduinoJson.h>

#include <EntityAbstract.h>
#include <JsonDao.h>

#define BME280Descriptor "{\"fields\": {\"temp\":\"-1\",\"hum\":\"-1\",\"press\":\"-1\"},\
\"actions\":{\"stat\":[],\"var\":[\"temp\",\"hum\",\"press\"],\"load\":[],\"save\":[],\"set\":[]}  }"

#define BH1750Descriptor "{\"fields\": {\"light\":\"-1\"},\
\"actions\":{\"stat\":[],\"var\":[\"light\"],\"load\":[],\"save\":[],\"set\":[]}  }"

EntityAbstract bme280("sensors","bme280","Temperature/Humidity/AtmPressure",BME280Descriptor);
EntityAbstract bh1750("sensors","bh1750","Light level",BH1750Descriptor);

EntityJson* entities[] = {&bme280,&bh1750};
JsonDao dao(entities, 2);

void setup()
{
Serial.begin(115200);
delay(5000);
Serial.println(FPSTR("Device starting"));
dao.init();
}

void loop()
{
	delay(5000);
	Serial.println(ESP.getFreeHeap());
}

#include "Arduino.h"

#include <EntityApplication.h>
#include "DS18D20sensor.h"
#include "Bme280sensor.h"
#include "Bh1750sensor.h"

DS18D20sensor ds18d20(D6, (char*) "Температура пол");
Bme280sensor bme280((char*)"Микроклимат воздух");
Bh1750sensor bh1750((char*)"Освещенность");

Entity* entities[] = { &ds18d20, &bme280, &bh1750 };
EntityUpdate* updateableEntities[] = { &ds18d20, &bme280, &bh1750};

EntityApplication app("ESP8266_BME280_BH1750_DS18D20", (char*)"Микроклимат зеленая комната",
		entities, ARRAY_SIZE(entities),
		updateableEntities, ARRAY_SIZE(updateableEntities), "1F321");

void setup() {
	app.begin(true);
}

void loop() {
	app.loop();
}

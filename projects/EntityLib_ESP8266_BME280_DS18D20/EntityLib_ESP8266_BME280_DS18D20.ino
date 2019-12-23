#include "Arduino.h"

#include <EntityApplication.h>
#include "Bme280sensor.h"
#include "DS18D20sensor.h"

Bme280sensor bme280((char*) "Воздух BME280");
DS18D20sensor ds18d20_2(D7, (char*) "Температура DS18D20 2", "Ds18D2022");

Entity* entities[] = { &bme280, &ds18d20_2 };
EntityUpdate* updateableEntities[] = { &bme280, &ds18d20_2};

EntityApplication app("ESP8266_AM2302_DS18D20", (char*)"Улица гараж",
		entities, ARRAY_SIZE(entities),
		updateableEntities, ARRAY_SIZE(updateableEntities), "1F321");

void setup() {
	app.begin(true);
}

void loop() {
	app.loop();
}

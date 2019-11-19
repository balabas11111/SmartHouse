#include "Arduino.h"

#include <EntityApplication.h>
#include "DS18D20sensor.h"
#include "Bme280sensor.h"


DS18D20sensor ds18d20(GPIO_NUM_3, (char*) "Температура пол");
Bme280sensor bme280((char*)"Микроклимат воздух");


Entity* entities[] = { &ds18d20, &bme280 };
EntityUpdate* updateableEntities[] = { &ds18d20, &bme280};

EntityApplication app("ESP8266_BME280_BH1750_DS18D20", (char*)"Микроклимат зеленая комната",
		entities, ARRAY_SIZE(entities),
		updateableEntities, ARRAY_SIZE(updateableEntities), "1F321");

void setup() {
	app.begin(true);
}

void loop() {
	app.loop();
}

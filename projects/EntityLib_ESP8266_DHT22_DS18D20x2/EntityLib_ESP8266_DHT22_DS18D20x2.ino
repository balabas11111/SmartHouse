#include "Arduino.h"

#include <EntityApplication.h>
#include "DHT22sensor.h"
#include "DS18D20sensor.h"

//DHT22sensor dht22(D5, (char*) "Микроклимат Воздух", 21);
DS18D20sensor ds18d20(D1, (char*) "Температура бак");
DS18D20sensor ds18d202(D6, (char*) "Температура гребенка", "ds18d202");

Entity* entities[] = { &ds18d20, &ds18d202 };
EntityUpdate* updateableEntities[] = { &ds18d20, &ds18d202};

EntityApplication app("ESP8266_DHT22_DS18D20x2", (char*)"Система отопления",
		entities, ARRAY_SIZE(entities),
		updateableEntities, ARRAY_SIZE(updateableEntities), "1F525");

void setup() {
	app.begin();
}

void loop() {
	app.loop();
}

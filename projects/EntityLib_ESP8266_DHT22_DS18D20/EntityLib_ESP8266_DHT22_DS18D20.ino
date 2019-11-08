#include "Arduino.h"

#include <EntityApplication.h>
#include "DHT22sensor.h"
#include "DS18D20sensor.h"

DHT22sensor dht22(D5, (char*) "Микроклимат воздух", AM2301);
DS18D20sensor ds18d20(D1, (char*) "Температура пол");

Entity* entities[] = { &dht22, &ds18d20 };
EntityUpdate* updateableEntities[] = { &dht22, &ds18d20};

EntityApplication app("ESP8266_DHT22_DS18D20", (char*)"Микроклимат",
		entities, ARRAY_SIZE(entities),
		updateableEntities, ARRAY_SIZE(updateableEntities), "1F321");

void setup() {
	app.begin();
}

void loop() {
	app.loop();
}

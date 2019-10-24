#include "Arduino.h"
#include <functional>

#include <EntityApplication.h>
#include "Bme280sensor.h"
#include "Bh1750sensor.h"
#include "DHT22sensor.h"
#include "DS18D20sensor.h"
#include "OutputPin.h"

#include "ESP8266WiFi.h"
#include "PubSubClient.h"

Bme280sensor bme280( (char*) "Микроклимат воздуха");
Bh1750sensor bh1750( (char*) "Уровень освещения");
OutputPin rele(BUILTIN_LED,  (char*) "Диод на плате", "built In LED",  LOW, false);
DHT22sensor dht22(D5, (char*) "Микроклимат воздуха DHT", DHT22);
DS18D20sensor ds18d20(D6, (char*) "Температура пола");

Entity* entities[] = { &bme280, &bh1750, &rele, &dht22, &ds18d20 };
EntityUpdate* updateableEntities[] = { &bme280, &bh1750, &rele, &dht22, &ds18d20};

EntityApplication app("EntityLib dev device", (char*)"Тестовое устройство",
		entities, ARRAY_SIZE(entities),
		updateableEntities, ARRAY_SIZE(updateableEntities));

void setup() {
	app.initWithWiFi(nullptr);
	app.setOnEntitiesChanged(onEntitiesChanged);
	app.updateEntities(true);
	app.registerOnServer();
}

void loop() {
	app.loop();
}

void onEntitiesChanged(){
	Serial.print(FPSTR("-CF dispatched"));
}

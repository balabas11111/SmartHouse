#include "Arduino.h"
#include <ArduinoJson.h>

#include <EntityApplication.h>
#include <Entity.h>
#include <EntityUpdate.h>

#include <ObjectUtils.h>

#include <WIFi/WiFiSettingsBox.h>
#include "Bme280sensor.h"
#include "Bh1750sensor.h"
#include "DHT22sensor.h"
#include "DS18D20sensor.h"
#include "OutputPin.h"

Ticker tickerHeap;

Bme280sensor bme280;
Bh1750sensor bh1750;
OutputPin rele(D4, "DefaultRele", "Default relea pin");
DHT22sensor dht22(D5);
DS18D20sensor ds18d20(D6);

Entity* entities[] = { &bme280, &bh1750, &rele, &dht22, &ds18d20 };
EntityUpdate* entityUpdate[] = { &bme280, &bh1750, &rele, &dht22, &ds18d20 };

EntityApplication app("EntityLib dev device", entities,
		ARRAY_SIZE(entities), entityUpdate, ARRAY_SIZE(entityUpdate));

void setup() {
	app.init();

	tickerHeap.attach(30, printHeap);

	ObjectUtils::printHeap();
	ObjectUtils::printMillis();
}

void loop() {
	app.loop();
}

void printHeap() {
	ObjectUtils::printHeap();
}

#include "Arduino.h"
#include <ArduinoJson.h>

#include <EntityApplication.h>
#include <Entity.h>
#include <EntityUpdate.h>
#include <Notifiers/NotifierEntityManagerGroupName.h>
#include <ObjectUtils.h>

#include <WIFi/WiFiSettingsBox.h>
#include "Bme280sensor.h"
#include "Bh1750sensor.h"
#include "DHT22sensor.h"
#include "DS18D20sensor.h"
#include "OutputPin.h"

Bme280sensor bme280;
Bh1750sensor bh1750;
OutputPin rele(BUILTIN_LED, "DefaultRele", "Default relea pin");
DHT22sensor dht22(D5);
DS18D20sensor ds18d20(D6);

Entity* entities[] = { &bme280, &bh1750, &rele, &dht22, &ds18d20 };
EntityUpdate* updateableEntities[] = { &bme280, &bh1750, &rele, &dht22, &ds18d20};

EntityApplication app("EntityLib dev device",
		entities, ARRAY_SIZE(entities),
		updateableEntities, ARRAY_SIZE(updateableEntities));

void setup() {
	app.initWithWiFi();
	app.setOnEntityChanged(onEntityChanged);

	app.registerTicker(1000,changeLed);
	app.registerTicker(10000, printSensorsJson);
	app.registerTicker(updateEntities);

	app.updateEntities(false);
}

void loop() {
	app.loop();
}

void printSensorsJson() {
	app.notify((char*)GROUP_SENSORS);
	//app.notify();
	app.triggerOnServerRegister();
}

void updateEntities(){
	//app.getEntityManager()->print();
	app.updateEntities(false);
}

void changeLed(){
	rele.setOnOpposite();
}

void onEntityChanged(){
	Serial.println(FPSTR("onEntityChanged"));
}

#include "Arduino.h"
#include <functional>

#include <EntityApplication.h>
#include "DHT22sensor.h"
#include "DS18D20sensor.h"
#include "OutputPin.h"

#include "ESP8266WiFi.h"
#include "PubSubClient.h"

DHT22sensor dht22(D5, (char*) "Микроклимат Воздух", AM2301);
DS18D20sensor ds18d20(D1, (char*) "Температура бак");
DS18D20sensor ds18d202(D0, (char*) "Температура гребенка", "ds18d202");

Entity* entities[] = { &dht22, &ds18d20, &ds18d202 };
EntityUpdate* updateableEntities[] = { &dht22, &ds18d20, &ds18d202};

EntityApplication app("ESP8266_DHT22_DS18D20x2", (char*)"Система отопления",
		entities, ARRAY_SIZE(entities),
		updateableEntities, ARRAY_SIZE(updateableEntities));

void setup() {
	app.initWithWiFi(nullptr);
	app.setOnEntitiesChanged(onEntitiesChanged);
	app.updateEntities(true);
	app.checkServerRegistration();
}

void loop() {
	app.loop();
}

void onEntitiesChanged(){
	Serial.print(FPSTR("-CF dispatched"));
}

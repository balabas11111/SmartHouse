#include "Arduino.h"
#include <functional>

#include <EntityApplication.h>
#include "DHT22sensor.h"
#include "DS18D20sensor.h"
#include "OutputPin.h"

#include "ESP8266WiFi.h"
#include "PubSubClient.h"

OutputPin redLed(D6,  (char*) "Диод", "redLED",  HIGH);
OutputPin greenLed(D7,  (char*) "Зеленый диод", "greenLED",  HIGH);

DHT22sensor dht22(D5, (char*) "Микроклимат воздух", DHT22);
DS18D20sensor ds18d20(D1, (char*) "Температура пол");

Entity* entities[] = { &dht22, &ds18d20 };
EntityUpdate* updateableEntities[] = { &dht22, &ds18d20};

EntityApplication app("ESP8266_DHT22_DS18D20", (char*)"Микроклимат",
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

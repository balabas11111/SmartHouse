#include "Arduino.h"

#include <EntityApplication.h>
#include "DS18D20sensor.h"
#include "ImpulseRelay.h"
#include "MAX6675sensor.h"

DS18D20sensor ds18d20(GPIO_NUM_19, (char*) "Температура бак");
DS18D20sensor ds18d202(GPIO_NUM_23, (char*) "Температура гребенка");
ImpulseRelay pupm1(GPIO_NUM_32, GPIO_NUM_33, (char*)"Насос гребенка", "pump1", "1F321");
ImpulseRelay pupm2(GPIO_NUM_27, GPIO_NUM_35, (char*)"Насос косвенник", "pump2", "1F321");
ImpulseRelay pupm3(GPIO_NUM_25, GPIO_NUM_26, (char*)"Насос котел", "pump3", "1F321");
MAX6675sensor heater(GPIO_NUM_5, GPIO_NUM_23, GPIO_NUM_19, (char*)"Температура дымоход");


Entity* entities[] = { &ds18d20, &ds18d202, &pupm1, &pupm2, &pupm3, &heater};
EntityUpdate* updateableEntities[] = { &ds18d20, &ds18d202, &pupm1, &pupm2, &pupm3, &heater};

EntityApplication app("ESP32_DS18D20x2", (char*)"Lolin32 test",
		entities, ARRAY_SIZE(entities),
		updateableEntities, ARRAY_SIZE(updateableEntities), "1F321");

void setup() {
	app.begin(true);
}

void loop() {
	app.loop();
}

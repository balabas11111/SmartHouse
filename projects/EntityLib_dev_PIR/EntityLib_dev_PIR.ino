#include "Arduino.h"

#include <EntityApplication.h>
#include "PIRDetector.h"
#include "pin/OutputPin.h"

OutputPin rele(BUILTIN_LED,  (char*) "Диод на плате", "built In LED",  LOW, false);
PIRdetector pir(D5, (char*) "Детектор движения");
Entity* entities[] = { &pir, &rele };
EntityUpdate* updateableEntities[] = { &pir, &rele};

EntityApplication app("ESP8266_PIR_DETECTOR_DEV", (char*)"Система безопасности разработка",
		entities, ARRAY_SIZE(entities),
		updateableEntities, ARRAY_SIZE(updateableEntities), "1F575");

void setup() {
	app.begin();
}

void loop() {
	app.loop();
}

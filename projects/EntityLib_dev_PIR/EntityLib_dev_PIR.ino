#include "Arduino.h"

#include <EntityApplication.h>
#include "PIRDetector.h"
#include "DS18D20sensor.h"
#include "Bme280sensor.h"
#include "Bh1750sensor.h"
#include "pin/OutputPin.h"

#include <display/DisplayToOledAdapter.h>
#include <display/DisplayPage.h>

OutputPin rele(BUILTIN_LED,  (char*) "Диод на плате", "built In LED",  LOW, false);
PIRdetector pir(D5, (char*) "Детектор движения");
DS18D20sensor ds18d20(D3, (char*) "Температура пол");
Bme280sensor bme280((char*)"Микроклимат воздух");
Bh1750sensor bh1750((char*)"Освещение");

PageToDisplayAdapter* oled = new DisplayToOledAdapter ();

const char* bmeFields[] = {"h","t","p"};
const char* bmeFieldsDescr[] = {"Влажность ","Темп-тура ","Давление "};
const char* bmeFieldsDescrMeasure[] = {" %"," C"," Па"};

DisplayPage bmePage(&bme280, bmeFields, bmeFieldsDescr, bmeFieldsDescrMeasure,  ARRAY_SIZE(bmeFields));

const char* bh1750Fields[] = {"l"};
const char* bh1750FieldsDescr[] = {"освещенность "};
const char* bh1750FieldsDescrMeasure[] = {" Люкс"};

DisplayPage bh1750Page(&bh1750, bh1750Fields, bh1750FieldsDescr, bh1750FieldsDescrMeasure,  ARRAY_SIZE(bh1750Fields));

const char* ds18d20Fields[] = {"0:t"};
const char* ds18d20FieldsDescr[] = {"Темп-тура "};
const char* ds18d20FieldsDescrMeasure[] = {" C"};

DisplayPage ds18d20Page(&ds18d20, ds18d20Fields, ds18d20FieldsDescr, ds18d20FieldsDescrMeasure,  ARRAY_SIZE(ds18d20Fields));

DisplayPage* pages[] = {&bmePage, &ds18d20Page, &bh1750Page};

Entity* entities[] = { &pir, &rele, &ds18d20, &bme280, &bh1750 };
EntityUpdate* updateableEntities[] = { &pir, &rele, &ds18d20, &bme280, &bh1750};

EntityApplication app("ESP8266_PIR_DETECTOR_DEV", (char*)"Система безопасности разработка",
		entities, ARRAY_SIZE(entities),
		updateableEntities, ARRAY_SIZE(updateableEntities), "1F575",
		oled, pages, ARRAY_SIZE(pages));

bool btnPressed = false;

void setup() {
	pinMode(D8, INPUT_PULLUP);
	pinMode(D6, INPUT);
	attachInterrupt(digitalPinToInterrupt(D8), interruptButton, RISING);
	attachInterrupt(digitalPinToInterrupt(D6), interruptButton, RISING);

	app.begin(true);
	app.switchToPage(1);
	//app.switchPagesWithInterval(15000);
}

void loop() {
	app.loop();

	if(btnPressed) {
		app.getDisplayManager()->switchToNextPageOrTurnPowerOn();
		btnPressed = false;
	}
}

void interruptButton() {
	btnPressed = true;
	Serial.println(FPSTR("Interrupted"));
}

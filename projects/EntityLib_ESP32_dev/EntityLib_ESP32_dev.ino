#include "Arduino.h"

#include <EntityApplication.h>
#include "DS18D20sensor.h"
#include "ImpulseRelay.h"
#include "MAX6675sensor.h"
#include "AnalogSensor.h"
#include "pin/InputPin.h"

#include "display/PageToDisplayAdapter.h"
#include "display/DisplayToOledAdapter.h"
#include "display/DisplayPage.h"
#include "display/DisplayPageBoolean.h"

#include "BeeperB.h"
#include "BeeperSerial.h"

#include "WS2811Led.h"
#include "serve/Alarmer.h"

#define NAME_PUMP_GREB "Насос гребенка"
#define NAME_PUMP_KOSV "Насос косвенник"
#define NAME_PUMP_HEATER "Насос котел"

BeeperB beeper(GPIO_NUM_13);
BeeperSerial beeperSerial(&beeper);

WS2811Led strip(GPIO_NUM_12, 1);

Alarmer alarmer(&beeperSerial, [](){return checkForAlarm();}, &strip);

DS18D20sensor ds18d20(GPIO_NUM_19, (char*) "Температура бак");
DS18D20sensor ds18d20_2(GPIO_NUM_16, (char*) "Температура гребенка", "ds18d20_2");
ImpulseRelay pump1(GPIO_NUM_25, GPIO_NUM_33, (char*) NAME_PUMP_GREB, "pump1", "1F321");
ImpulseRelay pump2(GPIO_NUM_26, GPIO_NUM_34, (char*) NAME_PUMP_KOSV, "pump2", "1F321");
ImpulseRelay pump3(GPIO_NUM_27, GPIO_NUM_35, (char*) NAME_PUMP_HEATER, "pump3", "1F321");
MAX6675sensor heater(GPIO_NUM_5, GPIO_NUM_18, GPIO_NUM_23, (char*) "Температура дымоход");

AnalogSensorPin mq2(GPIO_NUM_32, (char*) "Загазованость", "mq2", "Уровень ");
InputPin button(GPIO_NUM_14, (char*) "button", []() {onButtonPressed();});

Entity* entities[] = { &ds18d20, &ds18d20_2, &pump1, &pump2, &pump3, &heater, &mq2 };
EntityUpdate* updateableEntities[] = { &ds18d20, &ds18d20_2, &pump1, &pump2,
		&pump3, &heater, &mq2, &button, &beeperSerial, &alarmer };

PageToDisplayAdapter* oled = new DisplayToOledAdapter();

const char* ds18d20Fields[] = { "0:t", "1:t", "2:t" };
const char* ds18d20FieldsDescr[] = { "Верх ", "Сред ", "Низ " };
const char* ds18d20FieldsDescrMeasure[] = { " C", " C", " C" };

DisplayPage ds18d20Page(&ds18d20, ds18d20Fields, ds18d20FieldsDescr,
		ds18d20FieldsDescrMeasure, ARRAY_SIZE(ds18d20Fields), "Бак");

const char* ds18d20_2Fields[] = { "0:t", "1:t" };
const char* ds18d20_2FieldsDescr[] = { "Верх ", "Низ " };
const char* ds18d20_2FieldsDescrMeasure[] = { " C", " C" };

DisplayPage ds18d20_2Page(&ds18d20_2, ds18d20_2Fields, ds18d20_2FieldsDescr,
		ds18d20_2FieldsDescrMeasure, ARRAY_SIZE(ds18d20_2Fields),
		"Гребенка");

const char* heaterFields[] = { "t" };
const char* heaterFieldsDescr[] = { "т-ра " };
const char* heaterFieldsDescrMeasure[] = { " C" };

DisplayPage heaterPage(&heater, heaterFields, heaterFieldsDescr,
		heaterFieldsDescrMeasure, ARRAY_SIZE(heaterFields), "Дымоход");

const char* pumpFields[] = { "on" };
const char* pumpFieldsDescr[] = { "ВЫКЛ ", "вкл " };

DisplayPageBoolean pump1Page(&pump1, pumpFields, pumpFieldsDescr,
		ARRAY_SIZE(pumpFields), NAME_PUMP_GREB);

DisplayPageBoolean pump2Page(&pump2, pumpFields, pumpFieldsDescr,
		ARRAY_SIZE(pumpFields), NAME_PUMP_KOSV);

DisplayPageBoolean pump3Page(&pump3, pumpFields, pumpFieldsDescr,
		ARRAY_SIZE(pumpFields), NAME_PUMP_HEATER);

DisplayPage* pages[] = { &ds18d20Page, &ds18d20_2Page, &heaterPage, &pump1Page,
		&pump2Page, &pump3Page };

EntityApplication app("ESP32_DS18D20x2_MAX6675_MQ2_ImpulseRelay_OLED", (char*) "Lolin32 test", entities,
		ARRAY_SIZE(entities), updateableEntities,
		ARRAY_SIZE(updateableEntities), "1F321", oled, pages,
		ARRAY_SIZE(pages));

void setup() {
	strip.setRed();
	app.begin(true);
	strip.setGreen();
}

void loop() {
	app.loop();
}

void onButtonPressed() {
	if(!button.isOn()) {
		//app.getDisplayManager()->switchToNextPageOrTurnPowerOn();
	}
}

bool checkForAlarm() {
	return ds18d20.isAnyTempValueGreaterThan(75) || ds18d20.isAnyTempValueLessThan(10);
}

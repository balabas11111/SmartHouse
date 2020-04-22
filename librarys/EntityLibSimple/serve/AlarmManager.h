/*
 * AlarmManager.h
 *
 *  Created on: 22 апр. 2020 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_SERVE_ALARMMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_SERVE_ALARMMANAGER_H_

#define DEFAULT_PANIC_LENGTH 120000

#define PANIC_STARTED_BEEPS 3
#define PANIC_FINISHED_BEEPS 1

#define PANIC_FIELD "p"
#define PANIC_FIELD_DESCR "Тревога"

#include "Arduino.h"
#include "BeeperSerial.h"
#include "WS2811Led.h"
#include "EntityUpdate.h"
#include "Entity.h"

#include <functional>
#include <FunctionalInterrupt.h>

class AlarmManager: public Entity, public EntityUpdate {
public:
	AlarmManager(BeeperSerial* beeper, WS2811Led* led, const char* name =
			"AlarmManager", char* descr = (char*) "Менеджер паники") :
			Entity(GROUP_SENSORS, name, descr) {
		this->beeper = beeper;
		this->led = led;

		this->hasPost = true;

	}

	virtual ~AlarmManager();

	bool isPanic() {
		return this->panic;
	}

	void beginPanic() {
		if (!panic) {
			panic = true;
			beepAlarm();
			Serial.println(FPSTR("Panic start"));
		} else {
			extend = true;
			Serial.println(FPSTR("Panic extend"));
		}
	}

	void stopPanic() {
		ledOff = true;
		panic = false;
		extend = false;
		led->setVoid();
		beep = PANIC_FINISHED_BEEPS;

		led->setGreen();
		Serial.println(FPSTR("Panic stop"));
	}

	void beepAlarm() {
		extend = false;
		ledOff = false;

		led->setRed();
		beep = PANIC_STARTED_BEEPS;
		lastPanicNotification = millis();
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		response[PANIC_FIELD] = isPanic();
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
		UNUSED(response);
		bool onPosted = false;

		if (params.is<char*>(PANIC_FIELD)
				&& (strcmp("1", params[PANIC_FIELD]) == 0
						|| strcmp("true", params[PANIC_FIELD]) == 0)) {
			onPosted = true;
		}
		Serial.print(FPSTR("panic on ="));
		Serial.println(onPosted);
		if (onPosted) {
			beginPanic();
		} else {
			stopPanic();
		}
	}

	virtual void doAppendFieldsSwg(JsonObject& swgJson) override {
		EntityDescriptor::appendSwgEntityParams(swgJson, EMOJI_BULB);
		EntityDescriptor::appendSwgFieldBooleanOnOff(swgJson, PANIC_FIELD,
		PANIC_FIELD_DESCR, nullptr, false,
		EDC_FIELD_BOOLEAN_VALUE_ON, EDC_FIELD_BOOLEAN_VALUE_OFF,
		EDC_DESCR_ON, EDC_DESCR_OFF,
		EDC_DESCR_ACTION_PANIC_ON, EDC_DESCR_ACTION_PANIC_OFF,
		EMOJI_PANIC_ON, EMOJI_PANIC_OFF);
	}

	virtual void loop() {
		if (beep > 0) {

			beeper->getBeeper()->shortBeep(beep);
			Serial.print(FPSTR("beep="));
			Serial.println(beep);
			beep = 0;

			if(ledOff) {
				led->setVoid();
				ledOff = false;
			}
		}

		if (panic) {
			if (lastPanicNotification + DEFAULT_PANIC_LENGTH < millis()) {
				if (!extend) {
					stopPanic();
				} else {
					beepAlarm();
				}
			}
		}
	}
	;
private:
	BeeperSerial* beeper;
	WS2811Led* led;

	int beep = 0;

	bool ledOff = false;
	bool panic = false;
	bool extend = false;
	unsigned long lastPanicNotification = 0;

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_SERVE_ALARMMANAGER_H_ */

/*
 * OutputPin.h
 *
 *  Created on: 23 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_OUTPUTPIN_H_
#define LIBRARIES_ENTITYLIBSENSORS_OUTPUTPIN_H_

#include "Arduino.h"
#include "Entity.h"
#include "EntityUpdate.h"

#define ON_FIELD "on"

#define OUTPUT_PIN_NAME "digPinOutput"
#define OUTPUT_PIN_DESCRIPTION "Digital pin Output"

#define OUTPUT_PIN_DESCR "d"
#define OUTPUT_PIN_VALUE "v"

class OutputPin: public Entity, public EntityUpdate {
public:
	OutputPin(uint8_t pin, const char* name = strdup(OUTPUT_PIN_NAME), char* descr = strdup(OUTPUT_PIN_DESCRIPTION)) :
			Entity(GROUP_SENSORS, name, descr) {
		this->pin = pin;
	}

	virtual ~OutputPin() {
	}

	virtual void init() override {
	}

	virtual void doUpdate() override {
	}

	void setOn(bool on) {
		dispatchChangeEvent(isOn() != on);
		digitalWrite(on, on?HIGH:LOW);
	}

	bool isOn() {
		return digitalRead(pin) == HIGH;
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		setJsonField(response, ON_FIELD, this->isOn());
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
		uint8_t on = isOn();
		if (isKeyExistsInJsonAndNotEqValue(params, ON_FIELD, on)) {
			on = (getJsonField<uint8_t>(params, ON_FIELD));

			setChanged(true);

			setOn(on);
		}
	}

	virtual void doLoad(JsonObject& jsonFromFile) override {
	}

	virtual void doSave(JsonObject& jsonToFile) override {
	}
protected:
	uint8_t pin;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_OUTPUTPIN_H_ */

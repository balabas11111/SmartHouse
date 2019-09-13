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

#include <functional>

#define ON_FIELD "on"

#define OUTPUT_PIN_NAME "digPinOutput"
#define OUTPUT_PIN_DESCRIPTION "Digital pin Output"

#define OUTPUT_PIN_DESCR "d"
#define OUTPUT_PIN_VALUE "v"

class OutputPin: public Entity, public EntityUpdate {
public:
	OutputPin(uint8_t pin, const char* name = strdup(OUTPUT_PIN_NAME), const char* descr = OUTPUT_PIN_DESCRIPTION,
			std::function<void(void)> selfEventProcessFunction = nullptr) :
			Entity(GROUP_SENSORS, name, strdup(descr), selfEventProcessFunction) {
		this->pin = pin;
	}

	virtual ~OutputPin() {
	}

	virtual void init() override {
	}

	virtual void doUpdate() override {
	}

	void setOn(bool on = true) {
		bool ch =  isOn() != on;
		digitalWrite(on, on?HIGH:LOW);

		dispatchChangeEvent(ch);
	}

	bool isOn() {
		return digitalRead(pin) == HIGH;
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		setJsonField(response, ON_FIELD, this->isOn());
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
		UNUSED(response);
		uint8_t on = isOn();
		if (isKeyExistsInJsonAndNotEqValue(params, ON_FIELD, on)) {
			on = (getJsonField<uint8_t>(params, ON_FIELD));

			setChanged(true);

			setOn(on);
		}
	}

	virtual void doLoad(JsonObject& jsonFromFile) override {
		UNUSED(jsonFromFile);
	}

	virtual void doSave(JsonObject& jsonToFile) override {
		UNUSED(jsonToFile);
	}
protected:
	uint8_t pin;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_OUTPUTPIN_H_ */

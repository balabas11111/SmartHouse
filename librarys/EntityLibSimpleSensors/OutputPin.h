/*
 * OutputPin.h
 *
 *  Created on: 23 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_OUTPUTPIN_H_
#define LIBRARIES_ENTITYLIBSENSORS_OUTPUTPIN_H_

#include "Arduino.h"
#include "Comp/Pin.h"
#include "Entity.h"
#include "EntityUpdate.h"

#include <functional>

#define ON_FIELD "on"

#define OUTPUT_PIN_NAME "digPinOutput"
#define OUTPUT_PIN_DESCRIPTION "Digital pin Output"

class OutputPin:public Pin, public Entity, public EntityUpdate {
public:
	OutputPin(uint8_t pin, const char* name = strdup(OUTPUT_PIN_NAME), const char* descr = OUTPUT_PIN_DESCRIPTION,
			std::function<void(void)> selfEventProcessFunction = nullptr, bool applicationDispatcher = false) :
			Pin(pin,OUTPUT),
			Entity(GROUP_SENSORS, name, strdup(descr), selfEventProcessFunction, applicationDispatcher) {
	}

	virtual ~OutputPin() {
	}

	virtual void init() override {
	}

	virtual void doUpdate() override {
	}

	void setValue(uint8_t value) override {
		bool ch =  getValue() != value;
		digitalWrite(pin, value);

		markEntityAsChangedIfTrue(ch);
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		setJsonField(response, ON_FIELD, this->isOn());
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
		UNUSED(response);
		uint8_t on = isOn();
		if (isKeyExistsInJsonAndNotEqValue(params, ON_FIELD, on)) {
			setOn(getJsonField<uint8_t>(params, ON_FIELD));
		}
	}

	virtual void doLoad(JsonObject& jsonFromFile) override {
		UNUSED(jsonFromFile);
	}

	virtual void doSave(JsonObject& jsonToFile) override {
		UNUSED(jsonToFile);
	}
protected:

	bool validateMode() override{
		if(pinMod == OUTPUT){
			return true;
		}

		Serial.println(FPSTR("Bad pin or interrupt Mode"));
		return false;
	}

};

#endif /* LIBRARIES_ENTITYLIBSENSORS_OUTPUTPIN_H_ */

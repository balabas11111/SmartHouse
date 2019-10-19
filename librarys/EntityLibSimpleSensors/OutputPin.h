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
	OutputPin(uint8_t pin, char* descr = (char*)OUTPUT_PIN_DESCRIPTION, const char* name = OUTPUT_PIN_NAME,
			uint8_t onLevel = HIGH,
			std::function<void(void)> selfEventProcessFunction = nullptr, bool applicationDispatcher = false) :
			Pin(pin, OUTPUT, onLevel),
			Entity(GROUP_SENSORS, name, descr, selfEventProcessFunction, applicationDispatcher, true, true) {
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
		//setJsonField(response, ENTITY_FIELD_ITEM_CLASS,ENTITY_ITEM_CLASS_TOOGLE_BUTTON);
	}

	virtual void doAppendFieldsSwg(JsonObject& swgJson) override{
		EntityDescriptor::appendSwgEntityParams(swgJson, EMOJI_BULB);

		EntityDescriptor::appendSwgFieldBooleanOnOff(swgJson, ON_FIELD, EDC_DESCR_STATE);
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
		UNUSED(response);
		uint8_t on = isOn();
		uint8_t onPosted = 0;
		if(params.is<char*>(ON_FIELD) && strcmp("1",params[ON_FIELD])==0){
			onPosted = 1;
		}
		if (on !=onPosted) {
			Serial.print(this->getName());
			Serial.print(FPSTR(" on="));
			Serial.println(onPosted);
			setOn(onPosted);
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

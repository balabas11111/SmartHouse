/*
 * OutputPin.h
 *
 *  Created on: 23 ���. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_PIN_OUTPUTPIN_H_
#define LIBRARIES_ENTITYLIBSIMPLE_PIN_OUTPUTPIN_H_

#include <pin/Pin.h>
#include "Arduino.h"
#include "Entity.h"
#include "EntityUpdate.h"

#include <functional>

#define ON_FIELD "on"

#define OUTPUT_PIN_NAME "digPinOutput"
#define OUTPUT_PIN_DESCRIPTION "Digital pin Output"

class OutputPin:public Pin, public Entity, public EntityUpdate {
public:
	OutputPin(uint8_t pin, char* descr = (char*)OUTPUT_PIN_DESCRIPTION, const char* name = OUTPUT_PIN_NAME,
			uint8_t onLevel = HIGH, bool ro = true,
			std::function<void(void)> selfEventProcessFunction = nullptr, bool applicationDispatcher = false) :
			Pin(pin, OUTPUT, onLevel),
			Entity(GROUP_SENSORS, name, descr, selfEventProcessFunction, applicationDispatcher, true, true) {

		this->ro = ro;
	}

	virtual ~OutputPin() {
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
		if(!ro) {
			EntityDescriptor::appendSwgFieldBooleanOnOff(swgJson, ON_FIELD, EDC_DESCR_STATE);
		}
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
		UNUSED(response);
		uint8_t on = isOn();
		uint8_t onPosted = 0;
		//Serial.println(FPSTR("Output pin do post"));

		if(JsonObjectUtil::getFieldBooleanValue(params,ON_FIELD)){
			onPosted = 1;
		}

		//Serial.println(onPosted);

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
	bool ro = true;

	bool validateMode() override{
		if(pinMod == OUTPUT){
			return true;
		}

		Serial.println(FPSTR("Bad pin or interrupt Mode"));
		return false;
	}

};

#endif /* LIBRARIES_ENTITYLIBSENSORS_OUTPUTPIN_H_ */
/*
 * InputPin.h
 *
 *  Created on: 13 сент. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLESENSORS_INPUTPIN_H_
#define LIBRARIES_ENTITYLIBSIMPLESENSORS_INPUTPIN_H_

#include "Arduino.h"
#include "ArduinoJson.h"
#include "Comp/Pin.h"
#include "Entity.h"
#include "EntityUpdate.h"

#define ON_FIELD "on"

#define INPUT_PIN_NAME "digPinInput"
#define INPUT_PIN_DESCRIPTION "Digital pin Input"

class InputPin: public Pin, public Entity, public EntityUpdate {
public:
	InputPin(uint8_t pin, const char* name = strdup(INPUT_PIN_NAME), const char* descr = INPUT_PIN_DESCRIPTION,
			int interruptMode = CHANGE,
			int pinMode = INPUT_PULLUP,
			std::function<void(void)> interruptProcessFunction = nullptr,
			std::function<void(void)> selfEventProcessFunction = nullptr) :
			Pin(pin, pinMode), Entity(GROUP_SENSORS, name, strdup(descr), selfEventProcessFunction){

		this->interruptMode = interruptMode;
		this->lastState = digitalRead(pin);
		this->interruptProcessFunction = interruptProcessFunction;

	}
	virtual ~InputPin(){};

	virtual bool preValidate() override{
		return validateMode();
	}

	virtual bool validate() override{
		return validateMode();
	}

	virtual void init() override {
		this->attachPinInterrupt([this](){processInterrupt();}, this->interruptMode);
	}

	virtual void doUpdate() override {
	}

	virtual void loop() override{
		if(interrupted){
			lastState = getValue();
			interrupted = false;

			if(interruptProcessFunction!=nullptr){
				interruptProcessFunction();
			}

			if(selfEventProcessFunction!=nullptr){
				selfEventProcessFunction();
			}
		}
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		setJsonField(response, ON_FIELD, this->isOn());
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
		UNUSED(response);
	}

	virtual void doLoad(JsonObject& jsonFromFile) override {
		UNUSED(jsonFromFile);
	}

	virtual void doSave(JsonObject& jsonToFile) override {
		UNUSED(jsonToFile);
	}

protected:
	int interruptMode;
	bool interrupted = false;
	uint8_t lastState;

	std::function<void(void)> interruptProcessFunction;

	void processInterrupt(){
		uint8_t currState = getValue();

		if(currState!=lastState){
			interrupted = true;
			lastState = currState;
		}
	}

protected:
	bool validateMode() override {
		if( (pinMod == INPUT || pinMod ==INPUT_PULLUP)
						&&(interruptMode == RISING || interruptMode == FALLING || interruptMode == CHANGE)){
			return true;
		}

		Serial.println(FPSTR("Bad pin or interrupt Mode"));
		return false;
	}
};

#endif /* LIBRARIES_ENTITYLIBSIMPLESENSORS_INPUTPIN_H_ */

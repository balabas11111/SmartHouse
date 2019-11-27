/*
 * InputPin.h
 *
 *  Created on: 13 ����. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_PIN_INPUTPIN_H_
#define LIBRARIES_ENTITYLIBSIMPLE_PIN_INPUTPIN_H_

#include <pin/Pin.h>
#include "Arduino.h"
#include "ArduinoJson.h"
#include <functional>
#include "Entity.h"
#include "EntityUpdate.h"

#define ON_FIELD "on"

#define INPUT_PIN_NAME "digPinInput"
#define INPUT_PIN_DESCRIPTION "Digital pin Input"

class InputPin: public Pin, public Entity, public EntityUpdate {
public:
	InputPin(uint8_t pin,
			const char* name = INPUT_PIN_NAME,
			std::function<void(void)> interruptProcessFunction = nullptr):
			Pin(pin, INPUT_PULLUP), Entity(GROUP_SENSORS, name, (char*)INPUT_PIN_DESCRIPTION, nullptr) {

		construct(CHANGE, interruptProcessFunction, false);
	}

	InputPin(uint8_t pin, char* descr = (char*)INPUT_PIN_DESCRIPTION,
			const char* name = INPUT_PIN_NAME, int interruptMode = CHANGE,
			int pinMode = INPUT_PULLUP,
			std::function<void(void)> interruptProcessFunction = nullptr,
			std::function<void(void)> onSetChangedEventFunction = nullptr,
			bool hasGetMeth = false):
			Pin(pin, pinMode), Entity(GROUP_SENSORS, name, descr,
					onSetChangedEventFunction) {

		construct(interruptMode, interruptProcessFunction, hasGetMeth);
	}

	virtual ~InputPin() {}

	virtual bool preValidate() override {
		return validateMode();
	}

	virtual bool validate() override {
		return validateMode();
	}

	virtual void init() override {
		this->attachPinInterrupt([this]() {processInterrupt();},
				this->interruptMode);
		Serial.println(FPSTR("Interrupt attached"));
	}

	virtual void doUpdate() override {
	}

	virtual void loop() override {
		if (interrupted) {

			/*Serial.print(this->getName());
			Serial.println(FPSTR("Interrupted "));
*/
			checkLastState();
			this->interrupted = false;

			if (interruptProcessFunction != nullptr) {
				interruptProcessFunction();
			}

			if (onSetChangedEventFunction != nullptr) {
				onSetChangedEventFunction();
			}
		} else {
			this->interrupted = checkLastState();
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
	int interruptMode;bool interrupted = false;
	uint8_t lastState;

	std::function<void(void)> interruptProcessFunction;

	void construct(int interruptMode, std::function<void(void)> interruptProcessFunction, bool hasGetMeth) {
		this->interruptMode = interruptMode;
		this->lastState = getValue();
		this->interruptProcessFunction = interruptProcessFunction;
		this->hasGet = hasGetMeth;
		this->interval = -1;
	}

	void processInterrupt() {
		Serial.println(FPSTR("Process interrupt"));
		this->interrupted = checkLastState();
	}

	bool checkLastState() {
		uint8_t currState = getValue();

		if (currState != this->lastState) {
			this->lastState = currState;
			return true;
		}
		return false;
	}

	bool validateMode() override {
		if ((pinMod == INPUT || pinMod == INPUT_PULLUP)
				&& (interruptMode == RISING || interruptMode == FALLING
						|| interruptMode == CHANGE)) {
			return true;
		}

		Serial.println(FPSTR("Bad pin or interrupt Mode"));
		return false;
	}
};

#endif /* LIBRARIES_ENTITYLIBSIMPLESENSORS_INPUTPIN_H_ */

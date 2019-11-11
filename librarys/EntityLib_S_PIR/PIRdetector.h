/*
 * PIRdetector.h
 *
 *  Created on: 10 нояб. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIB_S_PIR_PIRDETECTOR_H_
#define LIBRARIES_ENTITYLIB_S_PIR_PIRDETECTOR_H_

#include <Arduino.h>
#include <functional>
#include <FunctionalInterrupt.h>
#include <Entity.h>
#include <EntityUpdate.h>

#define PIR_NAME "PIR"
#define PIR_DESCRIPTION "Movement detector"
#define PIR_DEFAULT_DELAY 10000

#define PIR_MOVEMENT "move"
//buildEntityHeader 55
class PIRdetector: public Entity, public EntityUpdate {
public:
	PIRdetector(uint8_t pin, char* description = (char*) PIR_DESCRIPTION,
			const char* name = PIR_NAME,
			unsigned long delay = PIR_DEFAULT_DELAY,
			std::function<void(void)> onMovementDetected = nullptr,
			std::function<void(void)> onNoMovementDetected = nullptr) :
			Entity(GROUP_SENSORS, name, description) {
		this->pin = pin;
		this->delay = delay;

		this->onMovementDetected = onMovementDetected;
		this->onNoMovementDetected = onNoMovementDetected;

		pinMode(pin, INPUT_PULLUP);
		attachInterrupt(digitalPinToInterrupt(pin), [this](){processMovementChange();},
				CHANGE);
	};

	virtual ~PIRdetector() {}

	bool isMovementDetected() {
		return this->detected;
	}

	virtual void doUpdate() override {}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		response[PIR_MOVEMENT] = detected;
	}

	virtual void doAppendFieldsSwg(JsonObject& swgJson) override{
		EntityDescriptor::appendSwgEntityParams(swgJson, EMOJI_DETECTIVE);

		EntityDescriptor::appendSwgFieldBooleanOnOffWithClass(swgJson, PIR_MOVEMENT, EDC_DESCR_MOVEMENT,
				EMOJI_DETECTIVE, true, EDC_CLASS_VIEW_BUTTON,
				EDC_FIELD_BOOLEAN_VALUE_ON, EDC_FIELD_BOOLEAN_VALUE_OFF,
				EDC_DESCR_MOVEMENT_ON, EDC_DESCR_MOVEMENT_OFF,
				EDC_DESCR_MOVEMENT_ON, EDC_DESCR_MOVEMENT_OFF,
				EMOJI_POLICE_CAR_LIGHTS, EMOJI_NO_PEDESTRIANTS
				);
	}

	virtual void loop() override {
		if(startTimer && lastTime + delay < millis()) {
			Serial.println(FPSTR("MOvement timed OUT"));
			detected = false;
			startTimer = false;
			markEntityAsChangedIfTrue(true, true);
		}
	}

private:
	void processMovementChange() {
		if(checkPin()) {
			movementDetected();
		} else {
			noMovementDetected();
		}
	}

	void movementDetected() {
		Serial.println(FPSTR("Movement detected"));

		startTimer = false;

		if(!detected) {
			markEntityAsChangedIfTrue(true, true);
		}

		detected = true;
	}
	void noMovementDetected() {
		Serial.println(FPSTR("NO Movement"));

		startTimer = true;
		lastTime = millis();
		Serial.println(FPSTR("TimerStarted"));

	}

	bool checkPin() {
		return digitalRead(pin) == HIGH;
	}

	uint8_t pin;
	unsigned long delay;
	unsigned long lastTime = 0;
	bool startTimer = false;

	bool detected;

	std::function<void(void)> onMovementDetected;
	std::function<void(void)> onNoMovementDetected;

};

#endif /* LIBRARIES_ENTITYLIB_S_PIR_PIRDETECTOR_H_ */

/*
 * PIRdetector.h
 *
 *  Created on: 10 нояб. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIB_S_IMPULSE_RELAY_H_
#define LIBRARIES_ENTITYLIB_S_IMPULSE_RELAY_H_

#include <Arduino.h>
#include <functional>
#include <FunctionalInterrupt.h>
#include <Entity.h>
#include <EntityUpdate.h>

#define SIGNAL_TIME 100
#define ON_FIELD "on"

class ImpulseReleay: public Entity, public EntityUpdate {
public:
	ImpulseReleay(uint8_t pin, uint8_t signalPin, char* description ,
			const char* name,
			const char* emoji,
			unsigned long signalTime = SIGNAL_TIME,
			uint8_t onLevel = LOW,
			std::function<void(void)> onTurnedOn = nullptr,
			std::function<void(void)> onTurnedOff = nullptr) :
			Entity(GROUP_SENSORS, name, description) {
		this->pin = pin;
		this->signalPin = signalPin;
		this->signalTime = signalTime;
		this->onLevel = onLevel;

		this->emoji = emoji;
		this->onTurnedOn = onTurnedOn;
		this->onTurnedOff = onTurnedOff;

		pinMode(signalPin, INPUT_PULLUP);
		attachInterrupt(digitalPinToInterrupt(signalPin), [this](){onStateChanged()();},
				CHANGE);

		pinMode(pin, OUTPUT);
		lastValue = isOn();
	};

	virtual ~ImpulseReleay() {}

	virtual void doUpdate() override {}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		response[ON_FIELD] = isOn();
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
		UNUSED(response);
		uint8_t on = isOn();
		uint8_t onPosted = 0;

		if(params.is<char*>(ON_FIELD) && (strcmp("1",params[ON_FIELD])==0 || strcmp("true",params[ON_FIELD])==0)){
			onPosted = 1;
		}

		if (on !=onPosted) {
			Serial.print(this->getName());
			Serial.print(FPSTR(" on="));
			Serial.println(onPosted);
			setOn(onPosted);
		}
	}

	virtual void doAppendFieldsSwg(JsonObject& swgJson) override{
		EntityDescriptor::appendSwgEntityParams(swgJson, EMOJI_BULB);
		EntityDescriptor::appendSwgFieldBooleanOnOff(swgJson, ON_FIELD, EDC_DESCR_STATE, emoji);
	}

	bool isOn() {
		return digitalRead(signalPin) == onLevel;
	}

	void setOn(bool on) {
		if(on!=isOn()) {
			Serial.print(FPSTR("RELAY"));
			delay(1);
			yield();
			digitalWrite(pin, HIGH);
			delay(this->signalTime);
			digitalWrite(pin, LOW);
			Serial.println(FPSTR("...done"));
			markEntityAsChangedIfTrue(true, true);
		}
	}

	virtual void loop() override {
		if(stateChanged) {
			bool on = isOn();
			if(lastValue!=on) {
				if(on && onTurnedOn!=nullptr) {
					onTurnedOn();
				} else
					if (!on && onTurnedOff!=nullptr) {
						onTurnedOff();
					}

			}
			lastValue = on;
			stateChanged = false;
		}
	}

private:
	void onStateChanged() {
		Serial.print(FPSTR("Relay changed to "));
		Serial.println(isOn());

		stateChanged = true;
	}

	uint8_t pin;
	uint8_t signalPin;
	unsigned long signalTime;
	uint8_t onLevel;

	const char* emoji;
	bool lastValue = false;
	bool stateChanged = false;

	std::function<void(void)> onTurnedOn;
	std::function<void(void)> onTurnedOff;

};

#endif /* LIBRARIES_ENTITYLIB_S_PIR_PIRDETECTOR_H_ */

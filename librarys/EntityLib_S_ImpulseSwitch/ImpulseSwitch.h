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

class ImpulseSwitch: public Entity, public EntityUpdate {
public:
	ImpulseSwitch(uint8_t pin, uint8_t signalPin1, uint8_t signalPin2, char* description,
			const char* name,
			const char* pos1Description,
			const char* pos2Description,
			const char* pos1ActionDescription,
			const char* pos2ActionDescription,
			const char* emoji,
			unsigned long signalTime = SIGNAL_TIME,
			uint8_t onLevel = LOW,
			std::function<void(void)> onTurnedOn = nullptr,
			std::function<void(void)> onPos2 = nullptr) :
			Entity(GROUP_SENSORS, name, description) {
		this->pin = pin;
		this->signalPin1 = signalPin1;
		this->signalPin2 = signalPin2;
		this->signalTime = signalTime;
		this->onLevel = onLevel;

		this->emoji = emoji;
		this->pos1Description = pos1Description;
		this->pos2Description = pos2Description;
		this->pos1ActionDescription = pos1ActionDescription;
		this->pos2ActionDescription = pos2ActionDescription;

		this->onPos1 = onPos1;
		this->onPos2 = onPos2;

		pinMode(signalPin1, INPUT_PULLUP);
		attachInterrupt(digitalPinToInterrupt(signalPin1), [this](){onStateChanged()();},
				CHANGE);

		pinMode(pin, OUTPUT);
		lastValue = isInPos1();
	};

	virtual ~ImpulseSwitch() {}

	virtual void doUpdate() override {}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		response[ON_FIELD] = isInPos1();
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
		UNUSED(response);
		uint8_t on = isInPos1();
		uint8_t onPosted = 0;

		if(params.is<char*>(ON_FIELD) && (strcmp("1",params[ON_FIELD])==0 || strcmp("true",params[ON_FIELD])==0)){
			onPosted = 1;
		}

		if (on !=onPosted) {
			Serial.print(this->getName());
			Serial.print(FPSTR(" on="));
			Serial.println(onPosted);
			setToPos1(onPosted);
		}
	}

	virtual void doAppendFieldsSwg(JsonObject& swgJson) override{
		EntityDescriptor::appendSwgEntityParams(swgJson, EMOJI_DETECTIVE);
		EntityDescriptor::appendSwgFieldBooleanOnOff(swgJson, ON_FIELD, EDC_DESCR_STATE);

		EntityDescriptor::appendSwgFieldBooleanOnOffWithClass(swgJson, ON_FIELD, EDC_DESCR_POSITION,
			emoji, false, EDC_CLASS_VIEW_BUTTON_BOOLEAN,
			EDC_FIELD_BOOLEAN_VALUE_ON, EDC_FIELD_BOOLEAN_VALUE_OFF,
			pos1Description, pos2Description,
			pos1ActionDescription, pos2ActionDescription
		);
	}

	bool isInPos1() {
		return digitalRead(signalPin1) == onLevel;
	}

	void setToPos1(bool on) {
		if(on!=isInPos1()) {
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
			bool on = isInPos1();
			if(lastValue!=on) {
				if(on && onPos1!=nullptr) {
					onPos1();
				} else
					if (!on && onPos2!=nullptr) {
						onPos2();
					}

			}
			lastValue = on;
			stateChanged = false;
		}
	}

private:
	void onStateChanged() {
		Serial.print(FPSTR("Relay changed to "));
		Serial.println(isInPos1());

		stateChanged = true;
	}

	uint8_t pin;
	uint8_t signalPin1;
	uint8_t signalPin2;
	unsigned long signalTime;
	uint8_t onLevel;

	bool lastValue = false;
	bool stateChanged = false;

	const char* emoji;
	const char* pos1Description;
	const char* pos2Description;
	const char* pos1ActionDescription;
	const char* pos2ActionDescription;

	std::function<void(void)> onPos1;
	std::function<void(void)> onPos2;

};

#endif /* LIBRARIES_ENTITYLIB_S_PIR_PIRDETECTOR_H_ */

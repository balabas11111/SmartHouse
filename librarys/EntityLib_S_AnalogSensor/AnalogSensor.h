/*
 * Bme280sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_ANALOGSENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_ANALOGSENSOR_H_

#include <Entity.h>
#include <EntityUpdate.h>
#include "Emoji.h"
#include <DeviceConfig.h>

//--------------------------------

#define VALUE "val"
#define DEFAULT_MEASURE "ед"

class AnalogSensorPin: public Entity, public EntityUpdate {
public:
	AnalogSensorPin(uint8_t pin, char* description, const char* name,
			const char* valueDescription, const char* valueMeasure = DEFAULT_MEASURE, const char* emoji = EMOJI_CYCLONE) :
			Entity(GROUP_SENSORS, name, description) {
		this->pin = pin;
		this->valueDescription = valueDescription;
		this->valueMeasure = valueMeasure;
		this->emoji = emoji;
		pinMode(pin, INPUT);
	}

	virtual ~AnalogSensorPin() {
	}

	virtual void init() override {
	}

	virtual void doUpdate() override {
		int analogValue = analogRead(this->pin);

		bool tchg = analogValue!=this->value;

		markEntityAsChangedIfTrue(tchg);
		this->value = analogValue;
		if(tchg){putToBuffer(VALUE, this->value);}
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		response[VALUE] = this->value;
	}

	virtual void doAppendFieldsSwg(JsonObject& swgJson) override{
		EntityDescriptor::appendSwgEntityParams(swgJson, EMOJI_BAR_CHART);
		EntityDescriptor::appendSwgFieldFloat(swgJson, VALUE, valueDescription, emoji, valueMeasure);
	}

protected:
	int value = -100;
	uint8_t pin;

	const char* valueDescription;
	const char* valueMeasure;
	const char* emoji;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_ */

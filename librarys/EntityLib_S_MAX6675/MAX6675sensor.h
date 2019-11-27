/*
 * Bme280sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_MAX6675SENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_MAX6675SENSOR_H_

#include <Entity.h>
#include <EntityUpdate.h>
#ifndef SENSORS_MOCKED
#include "max6675.h"
#else
#include <MAX6675Mock.h>
#endif
#include "Emoji.h"
#include <DeviceConfig.h>

//--------------------------------
#define MAX6675_NAME "max6675"
#define MAX6675_DESCRIPTION "Temperature/Humidity"

#define MAX6675_TEMPERATURE "t"

/*
	SO connected to Pin 19 (MISO)
	SS/CS connected to Pin 23 (MOSI)
	SCK connected to Pin 5 (SS)
*/

class MAX6675sensor: public Entity, public EntityUpdate {
public:
	MAX6675sensor(uint8_t pinSCK, uint8_t pinCS, uint8_t pinSO, char* description = (char*)MAX6675_DESCRIPTION, const char* name = MAX6675_NAME) :
			Entity(GROUP_SENSORS, name, description) {
#ifndef SENSORS_MOCKED
		this->thermocouple = new MAX6675(pinSCK, pinCS, pinSO);
#else
		this->thermocouple = new MAX6675Mock(pinSCK, pinCS, pinSO);
#endif
	}

	virtual ~MAX6675sensor() {
	}

	virtual void init() override {
#ifndef SENSORS_MOCKED
		for(int i=0; i<3; i++) {
			double t = thermocouple->readCelsius();

			if(!isnan(t)){
				this->temp = t;
				break;
			}

			Serial.print(FPSTR("  thermocouple->readCelsius()="));
			Serial.println(t);
		}
#endif
	}

	virtual void doUpdate() override {
		double t = thermocouple->readCelsius();

		for(int i=0; i<2; i++) {
			if(!isnan(t)){
				bool tchg = t!=this->temp;

				markEntityAsChangedIfTrue(tchg);
				this->temp = t;
				if(tchg){putToBuffer(MAX6675_TEMPERATURE, this->temp);}

				break;
			}
		}
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		double t = isnan(this->temp)?-279:this->temp;

		response[MAX6675_TEMPERATURE] = t;
	}

	virtual void doAppendFieldsSwg(JsonObject& swgJson) override{
		EntityDescriptor::appendSwgEntityParams(swgJson, EMOJI_CANDLE);

		EntityDescriptor::appendSwgFieldFloat(swgJson, MAX6675_TEMPERATURE, EDC_DESCR_TEMPERATURE, EMOJI_FIRE, EDC_MEASURE_GRAD_CELSIUS);
	}

	bool isTempValueGreaterThan(int value) {
		return temp >= value;
	}

	bool isTempValueLessThan(int value) {
		return temp <= value;
	}

	double getTemperature() {
		return temp;
	}

protected:
#ifndef SENSORS_MOCKED
	MAX6675* thermocouple = nullptr;;
#else
	MAX6675Mock* thermocouple = nullptr;;
#endif

	double temp = -100;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_ */

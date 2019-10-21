/*
 * Bme280sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_

#include <Entity.h>
#include <EntityUpdate.h>

#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <Bme280Mock.h>
#include "Emoji.h"
#include <DeviceConfig.h>

//-----------------------------------------------------
#define BME280 "bme280"
#define BME280_DESCRIPTION "Temperature/Humidity/AtmPressure"

#define BME280_HUMIDITY "h"
#define BME280_TEMPERATURE "t"
#define BME280_PRESSURE "p"

class Bme280sensor: public Entity, public EntityUpdate {
public:
	Bme280sensor(char* description = (char*)BME280_DESCRIPTION, const char* name = BME280):
		Entity(GROUP_SENSORS, name, description){};

	virtual ~Bme280sensor(){};

	virtual void init() override{
#ifndef SENSORS_MOCKED
		bme = new Adafruit_BME280();
#else
		bme= new Bme280Mock();
#endif
		bme->begin();
	}

	virtual void doUpdate() override{
		float h = bme->readHumidity();
		float t = bme->readTemperature();
		float p = bme->readPressure();

		bool hchg = h!=this->hum;
		bool tchg = t!=this->temp;
		bool pchg = p!=this->press;

		markEntityAsChangedIfTrue(hchg || tchg || pchg);

		this->hum = h;
		this->temp = t;
		this->press = p;

		if(hchg){putToBuffer(BME280_HUMIDITY, this->hum);}
		if(tchg){putToBuffer(BME280_TEMPERATURE, this->temp);}
		if(pchg){putToBuffer(BME280_PRESSURE, this->press);}

	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		response[BME280_HUMIDITY] = this->hum;
		response[BME280_TEMPERATURE] = this->temp;
		response[BME280_PRESSURE] = this->press;
	}

	virtual void doAppendFieldsSwg(JsonObject& swgJson) override{
		EntityDescriptor::appendSwgEntityParams(swgJson, EMOJI_SUN_CLOUD_RAIN);

		EntityDescriptor::appendSwgFieldFloat(swgJson, BME280_HUMIDITY, EDC_DESCR_HUMIDITY, EMOJI_DROPLET);
		EntityDescriptor::appendSwgFieldFloat(swgJson, BME280_TEMPERATURE, EDC_DESCR_TEMPERATURE, EMOJI_THERMOMETER);
		EntityDescriptor::appendSwgFieldFloat(swgJson, BME280_PRESSURE, EDC_DESCR_ATMPRESSURE, EMOJI_ARROW_DOWN);
	}

protected:
#ifndef SENSORS_MOCKED
	Adafruit_BME280* bme;
#else
	Bme280Mock* bme = nullptr;
#endif

	float hum = -100;
	float temp = -100;
	float press = -100;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_ */

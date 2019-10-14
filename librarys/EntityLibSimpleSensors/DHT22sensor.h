/*
 * Bme280sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_DHT22SENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_DHT22SENSOR_H_

#include <Entity.h>
#include <EntityUpdate.h>
#include <DHT.h>
#include <DHT22Mock.h>
#include "Emoji.h"

//--------------------------------
#define DHT22_NAME "dht22"
#define DHT22_DESCRIPTION "Temperature/Humidity"

#define DHT22_HUMIDITY "h"
#define DHT22_TEMPERATURE "t"

class DHT22sensor: public Entity, public EntityUpdate {
public:
	DHT22sensor(uint8_t pin, char* description = (char*)DHT22_DESCRIPTION, const char* name = DHT22_NAME) :
			Entity(GROUP_SENSORS, name, description) {
		//this->dht = new DHT(pin, DHT22);
		this->dht = new DHT22Mock(pin, DHT22);
	}

	virtual ~DHT22sensor() {
	}

	virtual void init() override {
		dht->begin();
	}

	virtual void doUpdate() override {
		float h = dht->readHumidity();
		float t = dht->readTemperature();

		bool hchg = h!=this->hum;
		bool tchg = t!=this->temp;

		markEntityAsChangedIfTrue(hchg || tchg);

		this->hum = h;
		this->temp = t;

		if(hchg){putToBuffer(BME280_HUMIDITY, this->hum);}
		if(tchg){putToBuffer(BME280_TEMPERATURE, this->temp);}
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		response[DHT22_HUMIDITY] = this->hum;
		response[DHT22_TEMPERATURE] = this->temp;
	}

	virtual void doAppendFieldsSwg(JsonObject& swgJson) override{
		EntityDescriptor::appendSwgEntityParams(swgJson, EMOJI_SUN_CLOUD);

		EntityDescriptor::appendSwgFieldFloat(swgJson, DHT22_HUMIDITY, EDC_DESCR_HUMIDITY, EMOJI_DROPLET);
		EntityDescriptor::appendSwgFieldFloat(swgJson, DHT22_TEMPERATURE, EDC_DESCR_TEMPERATURE, EMOJI_THERMOMETER);
	}

protected:
	//DHT* dht;
	DHT22Mock* dht;

	float hum = -100;
	float temp = -100;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_ */

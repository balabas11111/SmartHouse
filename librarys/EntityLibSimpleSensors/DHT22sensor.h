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

//--------------------------------
#define DHT22_NAME "dht22"
#define DHT22_DESCRIPTION "Temperature/Humidity"

#define DHT22_DESCR "d"
#define DHT22_HUMIDITY "h"
#define DHT22_TEMPERATURE "t"

class DHT22sensor: public Entity, public EntityUpdate {
public:
	DHT22sensor(int pin) :
			Entity(GROUP_SENSORS, DHT22_NAME, DHT22_DESCRIPTION) {
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

		dispatchChangeEvent((h!=this->hum) || t!=this->temp);

		this->hum = h;
		this->temp = t;
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		setJsonField(response, DHT22_HUMIDITY, this->hum);
		setJsonField(response, DHT22_TEMPERATURE, this->temp);
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
	}

	virtual void doLoad(JsonObject& jsonFromFile) override {
	}

	virtual void doSave(JsonObject& jsonToFile) override {
	}
protected:
	//DHT* dht;
	DHT22Mock* dht;

	float hum = -100;
	float temp = -100;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_ */

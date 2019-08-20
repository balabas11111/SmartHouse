/*
 * Bme280sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_DHT22SENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_DHT22SENSOR_H_

#define DHT22_NAME "dht22"
#define DHT22_DESCR "Temperature/Humidity"

#include "Entity.h"
#include "UpdateAble.h"
#include "DHT.h"
#include "DHT22Mock.h"

class DHT22sensor: public Entity, public UpdateAble {
public:
	DHT22sensor(int pin) :
			Entity(ROOT_GROUP_SENSORS, DHT22_NAME, DHT22_DESCR) {
		//this->dht = new DHT(pin, DHT22);
		this->dht = new DHT22Mock(pin, DHT22);
	}

	virtual ~DHT22sensor() {
	}

	virtual void init() override {
		dht->begin();
	}

	virtual void update() override {
		float h = dht->readHumidity();
		float t = dht->readTemperature();

		dispatchChangeEvent((h!=this->hum) || t!=this->temp);

		this->hum = h;
		this->temp = t;
	}
protected:
	//DHT* dht;
	DHT22Mock* dht;

	float hum = -100;
	float temp = -100;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_ */

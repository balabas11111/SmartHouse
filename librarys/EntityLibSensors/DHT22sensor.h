/*
 * Bme280sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_DHT22SENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_DHT22SENSOR_H_

#include "EntityJson.h"
#include "UpdateAble.h"
#include "DHT.h"

#define Dht22Descriptor "{\"data\": {\"temp\":\"-1\",\"hum\":\"-1\"},\
\"model\":{\"var\":[\"temp\",\"hum\"],\"tvar\":[\"temp\",\"hum\"]}  }"

class DHT22sensor: public EntityJson, public UpdateAble {
public:
	DHT22sensor(int pin):
		EntityJson(ROOT_GROUP_SENSORS,"dht22","Temperature/Humidity",Dht22Descriptor){
		this->dht = new DHT(pin, DHT22);
	};

	virtual ~DHT22sensor(){};

	virtual void init() override{}

	virtual void postModelDataInit() override{
		dht->begin();
	}

	virtual void update() override{
		float h = dht->readHumidity();
		float t = dht->readTemperature();

		this->getModelDataProvider()->setField(id, JSONKEY_temp,t);
		this->getModelDataProvider()->setField(id, JSONKEY_hum,h);

		sendAsEventSourceEntity();
	}
protected:
	DHT* dht;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_ */

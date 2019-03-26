/*
 * Bme280sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_

#include "EntityJson.h"
#include "UpdateAble.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <Bme280Mock.h>

#define BME280Descriptor "{\"data\": {\"temp\":\"-1\",\"hum\":\"-1\",\"press\":\"-1\"},\
\"model\":{\"var\":[\"temp\",\"hum\",\"press\"],\"tvar\":[\"temp\",\"hum\"]}  }"

class Bme280sensor: public EntityJson, public UpdateAble {
public:
	Bme280sensor():
		EntityJson(ROOT_GROUP_SENSORS,"bme280","Temperature/Humidity/AtmPressure",BME280Descriptor){};

	virtual ~Bme280sensor(){};

	virtual void init() override{	}

	virtual void postModelDataInit() override{
		//bme = new Adafruit_BME280();
		bme= new Bme280Mock();
		bme->begin();
	}

	virtual void update() override{
		float h = bme->readHumidity();
		float t = bme->readTemperature();
		float p = bme->readPressure();

		this->getModelDataProvider()->setField(id, JSONKEY_temp,t);
		this->getModelDataProvider()->setField(id, JSONKEY_hum,h);
		this->getModelDataProvider()->setField(id, JSONKEY_press,p);

		sendAsEventSourceEntity();
	}
protected:
	//Adafruit_BME280* bme;
	Bme280Mock* bme;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_ */

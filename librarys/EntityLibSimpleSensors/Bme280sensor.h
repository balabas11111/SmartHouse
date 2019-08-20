/*
 * Bme280sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_

#define BME280 "bme280"
#define BME280_DESCR "Temperature/Humidity/AtmPressure"

#include "Entity.h"
#include "UpdateAble.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <Bme280Mock.h>

class Bme280sensor: public Entity, public UpdateAble {
public:
	Bme280sensor():
		Entity(GROUP_SENSORS,BME280,BME280_DESCR){};

	virtual ~Bme280sensor(){};

	virtual void init() override{
		//bme = new Adafruit_BME280();
		bme= new Bme280Mock();
		bme->begin();
	}

	virtual void update() override{
		float h = bme->readHumidity();
		float t = bme->readTemperature();
		float p = bme->readPressure();

		dispatchChangeEvent((h!=this->hum) || (t!=this->temp) || (p!=this->press));

		this->hum = h;
		this->temp = t;
		this->press = p;
	}
protected:
	//Adafruit_BME280* bme;
	Bme280Mock* bme = nullptr;

	float hum = -100;
	float temp = -100;
	float press = -100;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_ */

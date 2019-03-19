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

#define BME280Descriptor "{\"data\": {\"temp\":\"-1\",\"hum\":\"-1\",\"press\":\"-1\"},\
\"model\":{\"var\":[\"temp\",\"hum\",\"press\"],\"tvar\":[\"temp\",\"hum\"]}  }"

class Bme280sensor: public EntityJson, public UpdateAble {
public:
	Bme280sensor():
		EntityJson(ROOT_GROUP_SENSORS,"bme280","Temperature/Humidity/AtmPressure",BME280Descriptor){};

	virtual ~Bme280sensor(){};

	virtual void init() override{

	}

	virtual void postModelDataInit() override{

	}

	virtual void update() override{

	}
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_ */

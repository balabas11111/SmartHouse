/*
 * Bh1750sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_BH1750SENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_BH1750SENSOR_H_

#include "EntityJson.h"
#include "UpdateAble.h"

#include <Wire.h>
#include <BH1750.h>
#include <Bh1750Mock.h>

#define BH1750Descriptor "{\"data\": {\"light\":\"-1\"},\
\"model\":{\"var\":[\"light\"],\"tvar\":[\"light\"]}  }"

class Bh1750sensor: public EntityJson, public UpdateAble {
public:
	Bh1750sensor():
		EntityJson(ROOT_GROUP_SENSORS,"bh1750","Light level",BH1750Descriptor){};

	virtual ~Bh1750sensor(){};

	virtual void init() override{	}

	virtual void postModelDataInit() override{
		lightMeter = new Bh1750Mock();
		lightMeter->begin();
	}

	virtual void update() override{
		uint16_t lux = lightMeter->readLightLevel();
		this->getModelDataProvider()->setField(id, JSONKEY_light,lux);
		sendAsEventSourceEntity();
	}
protected:
	Bh1750Mock* lightMeter;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BH1750SENSOR_H_ */

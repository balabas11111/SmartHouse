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

#define BH1750Descriptor "{\"data\": {\"light\":\"-1\"},\
\"model\":{\"var\":[\"light\"],\"tvar\":[\"light\"]}  }"

class Bh1750sensor: public EntityJson, public UpdateAble {
public:
	Bh1750sensor():
		EntityJson(ROOT_GROUP_SENSORS,"bh1750","Light level",BH1750Descriptor){};

	virtual ~Bh1750sensor(){};

	virtual void init() override{

	}

	virtual void postModelDataInit() override{

	}

	virtual void update() override{

	}
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BH1750SENSOR_H_ */

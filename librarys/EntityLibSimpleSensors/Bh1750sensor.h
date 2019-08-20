/*
 * Bh1750sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_BH1750SENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_BH1750SENSOR_H_

#include "Entity.h"
#include "UpdateAble.h"

#include <ArduinoJson.h>
#include <Wire.h>
#include <BH1750.h>
#include <Bh1750Mock.h>

//---------------------------------------
#define BH1750 "bh1750"
#define BH1750_DESCRIPTION "Light level"

#define BH1750_DESCR "d"
#define BH1750_LUX "l"

class Bh1750sensor: public Entity, public UpdateAble {
public:
	Bh1750sensor() :
			Entity(GROUP_SENSORS, BH1750, BH1750_DESCRIPTION) {
	}

	virtual void init() override {
		//lightMeter = new BH1750();
		lightMeter = new Bh1750Mock();
		lightMeter->begin();
	}

	virtual void update() override {
		uint16_t l = lightMeter->readLightLevel();

		dispatchChangeEvent(l != this->lux);

		this->lux = l;
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		setJsonField(response, BH1750_LUX, this->lux);
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
	}

	virtual void doLoad(JsonObject& jsonFromFile) override {
	}

	virtual void doSave(JsonObject& jsonToFile) override {
	}

protected:
	//BH1750* lightMeter;
	Bh1750Mock* lightMeter = nullptr;

private:
	uint16_t lux = 0;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BH1750SENSOR_H_ */

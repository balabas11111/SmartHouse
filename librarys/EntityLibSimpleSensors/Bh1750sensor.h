/*
 * Bh1750sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_BH1750SENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_BH1750SENSOR_H_

#include "Arduino.h"
#include <ArduinoJson.h>
#include "Entity.h"
#include <EntityUpdate.h>
#include <EntityDescriptor.h>

#include <Wire.h>
#include <BH1750.h>
#include <Bh1750Mock.h>
#include "Emoji.h"
#include <DeviceConfig.h>


//---------------------------------------
#define BH1750 "bh1750"
#define BH1750_DESCRIPTION "Light level"

#define BH1750_LUX "l"

class Bh1750sensor: public Entity, public EntityUpdate {
public:
	Bh1750sensor(char* description = (char*)BH1750_DESCRIPTION, const char* name = BH1750) :
			Entity(GROUP_SENSORS, name, description) {
	}

	virtual void init() override {
	#ifndef SENSORS_MOCKED
		lightMeter = new BH1750();
	#else
		lightMeter = new Bh1750Mock();
	#endif
		lightMeter->begin();
	}

	virtual void doUpdate() override {
		uint16_t l = lightMeter->readLightLevel();
		bool chg = l != this->lux;
		markEntityAsChangedIfTrue(chg);

		this->lux = l;

		if(chg){
			putToBuffer(BH1750_LUX, this->lux);
		}
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		response[BH1750_LUX] = this->lux;
	}

	virtual void doAppendFieldsSwg(JsonObject& swgJson) override{
		EntityDescriptor::appendSwgEntityParams(swgJson, EMOJI_SUN_WITH_FACE);

		EntityDescriptor::appendSwgFieldFloat(swgJson, BH1750_LUX, EDC_DESCR_LIGHT_LEVEL, EMOJI_DIM_BUTTON);
	}

protected:
#ifndef SENSORS_MOCKED
	BH1750* lightMeter;
#else
	Bh1750Mock* lightMeter = nullptr;
#endif

private:
	uint16_t lux = 0;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BH1750SENSOR_H_ */

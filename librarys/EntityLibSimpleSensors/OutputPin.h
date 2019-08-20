/*
 * OutputPin.h
 *
 *  Created on: 23 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_OUTPUTPIN_H_
#define LIBRARIES_ENTITYLIBSENSORS_OUTPUTPIN_H_

#include "EntityJson.h"
#include "UpdateAble.h"

#define ON_FIELD "on"

#define OutputPinDescriptor "{\"data\": {\"on\":0},\
\"model\":{\"var\":[\"on\"],\"tvar\":[\"on\"],\"set\":[\"on\"]}  }"

class OutputPin: public EntityJson, public UpdateAble {
public:
	OutputPin(int pin,const char* name):
		EntityJson(ROOT_GROUP_SENSORS,name,"Digital pin Output",OutputPinDescriptor){
		this->pin=pin;
	};
	virtual ~OutputPin(){};

	virtual void init() override{	}

	virtual void postModelDataInit() override{

	}

	virtual void update() override{

	}

	virtual void postDataPosted(){
		Serial.print(FPSTR("Pin value post handle pin="));
		Serial.print(pin);
	}

	bool isOn(){
		 return this->getModelDataProvider()->getFieldInt(id, ON_FIELD);
	}
protected:
	int pin;
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_OUTPUTPIN_H_ */

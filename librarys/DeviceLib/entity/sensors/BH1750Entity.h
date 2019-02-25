/*
 * BH1750Entity.h
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_SENSORS_BH1750ENTITY_H_
#define LIBRARIES_DEVICELIB_ENTITY_SENSORS_BH1750ENTITY_H_

#include <entity/model/Entity.h>

class BH1750_Entity: public Entity  {
public:
	BH1750_Entity():Entity("sensors","BME1750","BME1750 lights Lm"){};
	virtual ~BH1750_Entity(){};

	virtual void initModel() override{
		registerVariableField(Key_light, "light N/A");
	}
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_SENSORS_BH1750ENTITY_H_ */

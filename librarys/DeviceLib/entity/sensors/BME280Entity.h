/*
 * BME280Entity.h
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_SENSORS_BME280ENTITY_H_
#define LIBRARIES_DEVICELIB_ENTITY_SENSORS_BME280ENTITY_H_

#include <entity/model/Entity.h>

class BME280_Entity: public Entity {
public:
	BME280_Entity():Entity("sensors","BME280","BME280 temperature/humidity/pressure"){};
	virtual ~BME280_Entity(){};

	virtual void initModel() override{
		registerVariableField(Key_coordinate,"unknown");
		registerVariableField(Key_temperature,"temp N/A");
		registerVariableField(Key_humidity,"hum N/A");
		registerVariableField(Key_pressure,"press N/A");

		registerField_loadable(Key_coordinate);

		registerField_saveable(Key_coordinate);
	}
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_SENSORS_BME280ENTITY_H_ */

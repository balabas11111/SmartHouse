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

	virtual void init_fields(JsonObject& entityModel,JsonObject& entityTemplate) override{
		registerVarBase<const char*>(entityModel,entityTemplate,Key_coordinate,"unknown");
		registerVarBase<const char*>(entityModel,entityTemplate,Key_temperature,"temp N/A");
		registerVarBase<const char*>(entityModel,entityTemplate,Key_humidity,"hum N/A");
		registerVarBase<const char*>(entityModel,entityTemplate,Key_pressure,"press N/A");

	}

	virtual void prepare_fields_load(JsonObject& loadModel, JsonObject& entityTemplate) override{
		addStaticField(loadModel, entityTemplate, Key_id);
		addVariableField(loadModel,entityTemplate, Key_coordinate);
	}

	virtual void prepare_fields_save(JsonObject& entityTemplate, JsonObject& saveModel) override{
		addStaticField(entityTemplate,saveModel, Key_id);
		addVariableField(entityTemplate, saveModel, Key_coordinate);
	}
	virtual void prepare_fields_set(JsonObject& setTemplate, JsonObject& entityModel) override{

	}

};

#endif /* LIBRARIES_DEVICELIB_ENTITY_SENSORS_BME280ENTITY_H_ */

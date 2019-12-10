/*
 * EntityNrf.h
 *
 *  Created on: 8 дек. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITYNRF_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITYNRF_H_

#include "Entity.h"

class EntityNrf: public Entity {
public:
	EntityNrf(char* description , const char* name, char** fieldNames, char** fieldDescrs, char** fieldMeas, uint8_t fieldCount) :
			Entity(GROUP_SENSORS, name, description){
		this->fieldNames = fieldNames;
		this->fieldDescrs = fieldDescrs;
		this->fieldMeas = fieldMeas;
		this->fieldCount = fieldCount;

		this->fieldValues = new char*[fieldCount];
	}
	virtual ~EntityNrf(){};

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		for(int i=0; i<fieldCount; i++) {
			response[fieldNames[i]] = fieldValues[i];
		}
	}

	virtual void doAppendFieldsSwg(JsonObject& swgJson) override{
		EntityDescriptor::appendSwgEntityParams(swgJson, EMOJI_SUN_WITH_FACE);

		for(int i=0; i<fieldCount; i++) {
			EntityDescriptor::appendSwgFieldFloat(swgJson, fieldNames[i], fieldDescrs[i], EMOJI_DIM_BUTTON, fieldMeas[i]);
		}

	}

	virtual void setValue(const char* key, const char* value) override{
		for(int i=0; i<fieldCount; i++) {
			if(strcmp(key, fieldNames[i])==0){
				fieldValues[i] = strdup(value);
			}
		}
	}
private:
	char** fieldNames;
	char** fieldDescrs;
	char** fieldMeas;
	char** fieldValues;
	uint8_t fieldCount;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYNRF_H_ */

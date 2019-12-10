/*
 * EntityNrf.h
 *
 *  Created on: 8 дек. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITYNRFHOLDER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITYNRFHOLDER_H_

#include "Entity.h"


class EntityNrfHolder {
public:
	EntityNrfHolder(Entity* entity,
	char* prefValue,
	char** prefValues,
	uint8_t prefLength){
		this->entity = entity;
		this->prefValue = prefValue;
		this->prefValues = prefValues;
		this->prefLength = prefLength;
	}
	virtual ~EntityNrfHolder(){};

	bool isTarget(char* value) {
		Serial.println(FPSTR("isTarget"));
		String strVal = value;
		bool result = false;

		for(uint8_t i=0; i<prefLength;i++) {
			if(strVal.indexOf(prefValues[i])>-1){
				result = true;
			}
		}

		bool trgtPref = strVal.startsWith(prefValue);
		return result && trgtPref;
	}

	void set(char* value){
		String strVal = value;

		int index1 = strVal.indexOf(":");
		int index2 = strVal.indexOf(":", index1+1);
		int index3 = strVal.indexOf(":", index2+1);

		String fromDevice = strVal.substring(0, index1);
		String fromField = strVal.substring(index1+1, index2);
		String valueField = strVal.substring(index2+1, index3);

		Serial.print(FPSTR("fromDevice="));
		Serial.print(fromDevice);
		Serial.print(FPSTR(" fromField="));
		Serial.print(fromField);
		Serial.print(FPSTR(" valueField="));
		Serial.println(valueField);

		entity->setValue(fromField.c_str(), valueField.c_str());
	}
private:
	Entity* entity;
	char* prefValue;
	char** prefValues;
	uint8_t prefLength;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYNRF_H_ */

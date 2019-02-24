/*
 * EntityFieldHolder.h
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYFIELDHOLDER_H_
#define LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYFIELDHOLDER_H_

#include <Arduino.h>

class EntityFieldHolderBase{
public:
	virtual ~EntityFieldHolderBase(){};

	virtual void print()=0;

	virtual String getValStr()=0;
};

template<typename T>
class EntityFieldHolder: public EntityFieldHolderBase {
public:
	EntityFieldHolder(int entityId,T value){
		this->entityId=entityId;
		this->value=value;
	};
	virtual ~EntityFieldHolder() {}

	int getEntityId() const {
		return entityId;
	}

	T getValue() const {
		return value;
	}

	void setValue(T value) {
		this->value = value;
	}

	virtual String getValStr(){
		return String(value);
	}

	void print() override{
		Serial.print(FPSTR("holder ("));
		Serial.print(entityId);
		Serial.print(FPSTR(" : "));
		Serial.print(value);
		Serial.print(FPSTR(") "));
	}

protected:
	T value;
	int entityId;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYFIELDHOLDER_H_ */

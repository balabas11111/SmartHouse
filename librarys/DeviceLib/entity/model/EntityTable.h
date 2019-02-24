/*
 * EntityTable.h
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYTABLE_H_
#define LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYTABLE_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <list>
#include <entity/model/EntityFieldHolder.h>
#include <functional>

class EntityTable {
public:

	typedef enum {
	  Class_int, Class_char, Class_float
	} EntityFieldClass;

	EntityTable(){
	};
	virtual ~EntityTable(){};

	bool containsEntityId(int entityIndexId){
		if(entityFieldsHolders.size()>entityIndexId){
			return true;
		}
		return false;
	}

	template<typename T>
	int registerEntityField(int entityId,T value){
		EntityFieldHolderBase* holder = new EntityFieldHolder<T>(entityId,value);

		entityFieldsHolders.push_back(holder);

		int ind = entityFieldsHolders.size()-1;
		Serial.print(FPSTR(" ind="));
		Serial.println(ind);

		return ind;
	}

	template<typename T>
	void setValByEntityId(int entityIdIndex,T value){
		/*	Serial.print(FPSTR("set value index="));
			Serial.print(entityIdIndex);
			Serial.print(FPSTR(" value="));
			Serial.println(value);
		*/

		auto h = std::next(entityFieldsHolders.begin(), entityIdIndex);

		EntityFieldHolder<T>* holder = static_cast<EntityFieldHolder<T>*>(*h);

		holder->setValue(value);

		dispatchEvent(entityIdIndex);
	}

	void dispatchEvent(int entityIdIndex){
		if(funcEvent!=nullptr){
			funcEvent(entityIdIndex);
		}
	}

	template<typename T>
	T getValByEntityId(int entityIdIndex){
		auto h = std::next(entityFieldsHolders.begin(), entityIdIndex);
		EntityFieldHolder<T>* holder = static_cast<EntityFieldHolder<T>*>(*h);

		return holder->getValue();
	}

	String getTemplateValueByEntityId(int entityIdIndex){
		auto h = std::next(entityFieldsHolders.begin(), entityIdIndex);
		EntityFieldHolderBase* holder = *h;

		return holder->getValStr();
	}

	void print(){
		Serial.println(FPSTR("EntityTable"));
		for (std::list<EntityFieldHolderBase*>::iterator it = entityFieldsHolders.begin(); it != entityFieldsHolders.end(); it++){
			EntityFieldHolderBase* fh=*it;
			fh->print();
		}

		Serial.println();
	}

	void printValByEntityId(int entityIdIndex){
		auto h = std::next(entityFieldsHolders.begin(), entityIdIndex);
		EntityFieldHolderBase* holder = *h;

		holder->print();
	}

	const std::function<void(int)>& getFuncEvent() const {
		return funcEvent;
	}

	void setFuncEvent(const std::function<void(int)>& funcEvent) {
		this->funcEvent = funcEvent;
	}

protected:
	int registerEntityIdByClass(uint8_t clazz,int entityId);

	std::list<EntityFieldHolderBase*> entityFieldsHolders;
	std::function<void(int)> funcEvent;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYTABLE_H_ */



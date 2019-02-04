/*
 * EntityBase.h
 *
 *  Created on: Feb 4, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYBASE_H_
#define LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYBASE_H_

#include "Arduino.h"

#include "interfaces/Identifiable.h"
#include "interfaces/Nameable.h"

#include "ESP_Consts.h"

const uint8_t ENTITY_THING_SPEAK_INDEX PROGMEM = 0;
const uint8_t ENTITY_MIN_MAX_INDEX     PROGMEM = 1;
const uint8_t ENTITY_MQ_INDEX          PROGMEM = 2;

const char* const EntityBaseNames[]      PROGMEM=
{
		"ThingSpeakFields",
		"MinMaxFields",
		"MqFields"
};

class EntityBase: public Nameable,public Identifiable {
public:
	virtual ~EntityBase(){};

	virtual const char* const* getKeys()=0;

	virtual const char* const* getDefaults()=0;

	virtual EntityBase* clone() const =0;


	virtual uint8_t getSize(){return ARRAY_SIZE(getKeys());}

	virtual String getName() override{
		return EntityBaseNames[getId()];
	}

	virtual String getJsonDefault(){
		return getJson(getDefaults());
	}

	virtual String getJson(const char* const* values){
		uint8_t size=ARRAY_SIZE(values);
		String result="{\"id\": \""+String(getId())+"\", \"name\": \""+getName()+"\", \"size\": \""+size+"\", \"fields\": [";
		for(uint8_t i=0;i<size;i++){
			result+="{\"id\": \""+String(getKeys()[i])+"\", \"value\": \""+String(values[i])+"\"},";
		}

		result.setCharAt(result.length(), ']');
		result+="}";

		return result;
	}
protected:
	String* values;
};

#endif /* LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYBASE_H_ */

/*
 * EntityService.h
 *
 *  Created on: 11 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_ENTITYSERVICEBASE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_ENTITYSERVICEBASE_H_

#include <ArduinoJson.h>
#include "src/ESP8266HTTPClient.h"

#ifdef ESP8266
	#include "FunctionalInterrupt.h"
#endif
#ifdef ESP32
	#include <functional>
#endif

#include <interfaces/Nameable.h>
#include <interfaces/EntityConsts.h>

class EntityServiceBase{
public:

	virtual ~EntityServiceBase(){};

	virtual uint8_t getEntityId()=0;

	virtual const char* getEntityName(){
		return ENTITY_NAME[getEntityId()];
	}

	//All generated items will be inserted into &items data is income data from customer
	virtual int getAbstractItems(JsonArray& data,JsonArray& items,uint8_t pageId){
		return getAbstractItems(items,pageId);
	}

	//All generated items will be inserted into &items
	virtual int getAbstractItems(JsonArray& items,uint8_t pageId)=0;

	//All items from &items array will be processed by service
	virtual int postAbstractItems(JsonArray& items,uint8_t pageId)=0;

};


#endif /* LIBRARIES_DEVICELIB_INTERFACES_ENTITYSERVICE_H_ */

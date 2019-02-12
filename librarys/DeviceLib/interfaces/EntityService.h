/*
 * EntityService.h
 *
 *  Created on: 11 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_ENTITYSERVICE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_ENTITYSERVICE_H_

#include <ArduinoJson.h>
#include <interfaces/Nameable.h>

const char DEFAULT_CHILDREN_TAG[]="items";

//ids of all possible entity services
enum Entity: uint8_t {
	Entity_sensors, Entity_settings, Entity_commands, Entity_timeIntervals
};
const char* const ENTITY_NAME[] PROGMEM = {
	"sensors","settings","command", "timeIntervals"
};

//ids of all possible pages(operations), which could be executed on EntityService
enum Page: uint8_t {
	Page_list, Page_listVal, Page_save, Page_saveVal, Page_delete, Page_execute
};
const char* const PAGE_NAME[] PROGMEM = {
	"list","getVal","save","saveVal","delete", "execute"
};
/* Entity request Is deployed at /entity HTTP_ANY param=entity
 *
 * entity?entity={"Entity":{"id":"0","name":"sensors","pageId":"0","pageName":"list"},"data":{}}
 * entity?entity={"Entity":{"id":"0","name":"sensors","pageId":"0","pageName":"getVal"},"data":{}}
 *
 * { "Entity":{
 * 			"id":"0",
 *	 		"name":"sensors",
 * 			"pageId":"0",
 * 			"pageName":"list"
 * 			},
 * 	 "data":{}
 * }
 */
class EntityService{
public:

	virtual uint8_t getEntityId()=0;

	virtual const char* getEntityName(){
		return ENTITY_NAME[getEntityId()];
	}

	//All generated items will be inserted into &items data is income data from customer
	virtual JsonArray& getAbstractItems(JsonArray& data,JsonArray& items,uint8_t pageId){
		return getAbstractItems(items,pageId);
	}

	//All generated items will be inserted into &items
	virtual JsonArray& getAbstractItems(JsonArray& items,uint8_t pageId)=0;

	//All items from &items array will be processed by service
	virtual JsonArray& postAbstractItems(JsonArray& items,uint8_t pageId)=0;
};


#endif /* LIBRARIES_DEVICELIB_INTERFACES_ENTITYSERVICE_H_ */

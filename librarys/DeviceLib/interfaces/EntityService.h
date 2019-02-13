/*
 * EntityService.h
 *
 *  Created on: 11 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_ENTITYSERVICE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_ENTITYSERVICE_H_

#include <ArduinoJson.h>
#include "src/ESP8266HTTPClient.h"

#ifdef ESP8266
	#include "FunctionalInterrupt.h"
#endif
#ifdef ESP32
	#include <functional>
#endif

#include <interfaces/Nameable.h>

//tags for Json parsing/constructing
const char DEFAULT_RESULT_TAG[]      PROGMEM = "result";
const char DEFAULT_CHILDREN_TAG[]    PROGMEM = "items";

const char HTTP_STATUS_TAG[]         PROGMEM = "httpStatus";
const char HTTP_STATUS_DETAILS_TAG[] PROGMEM = "httpDetails";

//default status details text
const char STATUS_OK[]            PROGMEM="Ok";
const char STATUS_WRONG_SERVICE[] PROGMEM="Entity Service not found";
const char STATUS_WRONG_PAGE[]    PROGMEM="Page not found";
const char STATUS_WRONG_JSON[]    PROGMEM="Bad income Json";
const char STATUS_NO_ENTITY[]     PROGMEM="No Entity in Json";

//ids of all possible entity services
enum Entity: uint8_t {
	Entity_sensors, Entity_settings, Entity_commands, Entity_timeIntervals
};
const char* const ENTITY_NAME[] PROGMEM = {
	"sensors","settings","command", "timeIntervalService"
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
 * entity?entity={"Entity":{"id":"0","name":"sensors","pageId":"1","pageName":"getVal"},"data":{}}
 *
 * entity?entity={"Entity":{"id":"1","name":"settings","pageId":"0","pageName":"list"},"data":{}}
 * entity?entity={"Entity":{"id":"1","name":"settings","pageId":"1","pageName":"getVal"},"data":{}}
 *
 * entity?entity={"Entity":{"id":"3","name":"timeIntervalService","pageId":"0","pageName":"list"},"data":{}}
 * entity?entity={"Entity":{"id":"3","name":"timeIntervalService","pageId":"1","pageName":"getVal"},"data":{}}
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
	virtual int getAbstractItems(JsonArray& data,JsonArray& items,uint8_t pageId){
		return getAbstractItems(items,pageId);
	}

	//All generated items will be inserted into &items
	virtual int getAbstractItems(JsonArray& items,uint8_t pageId)=0;

	//All items from &items array will be processed by service
	virtual int postAbstractItems(JsonArray& items,uint8_t pageId)=0;
};


#endif /* LIBRARIES_DEVICELIB_INTERFACES_ENTITYSERVICE_H_ */

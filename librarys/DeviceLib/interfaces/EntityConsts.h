/*
 * EntityConsts.h
 *
 *  Created on: Feb 15, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_ENTITYCONSTS_H_
#define LIBRARIES_DEVICELIB_INTERFACES_ENTITYCONSTS_H_

#include <Arduino.h>

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
enum EntityIds: uint8_t {
	Entity_sensors, Entity_settings, Entity_commands, Entity_timeIntervals
};
const char* const ENTITY_NAME[] PROGMEM = {
	"sensors","settings","command", "timeIntervalService"
};

//ids of all possible pages(operations), which could be executed on EntityService
enum Page: uint8_t {
	Page_list, Page_listVal, Page_save, Page_saveVal, Page_delete, Page_execute, Page_add
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

#endif /* LIBRARIES_DEVICELIB_INTERFACES_ENTITYCONSTS_H_ */

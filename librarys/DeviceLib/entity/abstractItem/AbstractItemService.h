/*
 * AbstractItemService.h
 *
 *  Created on: 17 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_ABSTRACTITEM_ABSTRACTITEMSERVICE_H_
#define LIBRARIES_DEVICELIB_ENTITY_ABSTRACTITEM_ABSTRACTITEMSERVICE_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <functional>

#include <list/AbstractItemDAOList.h>

/* Request Is deployed at /services HTTP_ANY param=p
 *
 * /services?p={"Request":{"id":"0","dao":"sensors","aid":"0","action":"list","hashid": "0"},"data":{}}
 * /services?p={"Request":{"id":"0","dao":"sensors","aid":"1","action":"list","hashid": "0"},"data":{}}
 *
 * /services?p={"Request":{"id":"1","dao":"settings","aid":"0","action":"list","hashid": "0"},"data":{}}
 * /services?p={"Request":{"id":"1","dao":"settings","aid":"1","action":"vals","hashid": "0"},"data":{}}
 *
 * /services?p={"Request":{"id":"2","dao":"timeIntervals","aid":"0","action":"list","hashid": "0"},"data":{}}
 * /services?p={"Request":{"id":"2","dao":"timeIntervals","aid":"1","action":"save","hashid": "0"},"data":{}}
 *
 * { "Request":{
 * 			"id":"0",
 *	 		"dao":"sensors",
 * 			"aid":"0",
 * 			"action":"list",
 * 			"hashid": "0"
 * 			},
 * 	 "data":{
 *
 * 	 		}
 * }
 *
 *{"ItemHash": {
 *		"mod:": {},
 *		"descr:": {}
 *}
 *
 *
 *
 *

 */

class AbstractItemService {
public:
	AbstractItemService(AbstractItemDAO* daos[],std::function<void(JsonObject&,int)> sendResult,std::function<void(JsonObject&,int)> sendCache);
	virtual ~AbstractItemService(){};

	int processRequest(String str);

	boolean isCached(JsonObject& obj);

	void returnCachedUrl(JsonObject& obj);

	int initialize();

	AbstractItemDAOList getDaos();

	int startModel(JsonObject* model,JsonObject* descriptor);

protected:

	void startDAOsModels();

	AbstractItemDAOList daos;

	JsonObject* model;
	JsonObject* descriptor;
	DynamicJsonBuffer* jsonBuffer;

	std::function<void(JsonObject&,int)> sendResult;
	std::function<void(JsonObject&,int)> sendCache;

};

#endif /* LIBRARIES_DEVICELIB_ENTITY_ABSTRACTITEM_ABSTRACTITEMSERVICE_H_ */

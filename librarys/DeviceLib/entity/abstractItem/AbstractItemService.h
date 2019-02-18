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
 *  { "Response": {
 *					"mod:": { items models here -> "itemHash1": {item1}, "itemHash2": "{item2}", ...},
 *					"descr:": { all descriptors here "descrHash": {descr1}, ...},
 *					"res": {"status":"200", "str": "Ok", "dao": "sensors", "action": "cache", "hashid":"hashId"}
 *  			  }
 *  }
 *
 *{"ItemHash": {
 *		"mod:": {},
 *		"descr:": {}
 *}
 *
 *
 *{ "mod": { "descr": "descriptorName", "descrHash": "",
 * 				"fields": {
 *							"id": "0",
 *							"name": "ADDR01",
 *							"descr": "Some address1",
 *							"val": "10",
 *							"hash": "aw1245",
 *							"items": []
 * 				          },
 * 				"dict": {
 *						"descr": { "ADDR01": "Some address1", "ADDR02": "SomeAdredd 22"}
 * 						}
 *
 * 		     }
 *
 * {"descr": {"name": "descriptorName", "descrHash": "descrHash", "save": "1", "cache": "0",
 *              "hash": { "keys":{"id":"","name":""},"val": "hash"},
 * 				"provide": {"thingSpeakService":"","timeIntervalService": ""},
 *              "persist": {
 *              			<!--
 *              			     set     - apply another model to current a/e external set from json
 *              			     put     - put value from local device
 *              				 load    - load field from file
 *              				 save    - save field to file
 *              				 dict    - allows set from dictionary value is key column name
 *              			-->
 *              				"set": {"descr":""},
 *              				"put": {"val":""}
 *              				"load": {"descr": ""},
 *              				"save": {"id":"","descr": "", "items": ""},
 *              				"descr": {"save": "1"},
 *              				"dictSet": {"descr - target field": "name - keyField"}
 *              			},
 *              "compare": {"id":"", "name":"", "hash":"" },
 *              "dict": {"descr": "name"},
 *
 *              "default": {"descr": "Default descr field value", "name": ""},
 *
 * 				"fields": {
		 					"id": "int",
		 					"name": "String",
							"descr": "String",
							"val": "float",
							"hash": "String",
							"items": "List"
 * 						  },
 * 				"childs": ["items"]
 * 				}
 *
 * }
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

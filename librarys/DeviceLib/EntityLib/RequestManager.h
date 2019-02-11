/*
 * RequestManager.h
 *
 *  Created on: 8 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITYLIB_REQUESTMANAGER_H_
#define LIBRARIES_DEVICELIB_ENTITYLIB_REQUESTMANAGER_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include <EntityLib/Request.h>
#include <EntityLib/RequestExecutor.h>
#include <EntityLib/Url_Registerer.h>

#include <AbstractSensor.h>

const char ONE_PARAM_URL_GET[]    PROGMEM = "Entity_GET";
const char ONE_PARAM_URL_POST[]    PROGMEM = "Entity_POST";

const char THREE_PARAM_URL_GET[]    PROGMEM = "Entity_GET";
const char THREE_PARAM_URL_POST[]    PROGMEM = "Entity_POST";

/*{"Request":{"httpStatus":"0",
 *              "executor":{"id":"0","name":"Sensors"},
 *                  "kind":{"id":"0"},
 *                  "page":{"id":"0","name":""},
*                   "data":
*                   	{
*                   		Entity:{"id":"0","name":"", "static":{}, "fields":[], "entityFields":[], "vals": [], "entities": []}
*                   	}
*  }}
*/


/*
 * Class receives String Request parses int8_t executors id and pageId
 *
 * if request received with String ids only, then selects executor and page's ids by name
 */



class RequestManager {
public:
	RequestManager(Url_Registerer* registerer);
	virtual ~RequestManager();

	virtual uint8_t registerPaths();

	virtual void registerDefaultExecutors();

	virtual uint16_t registerExecutors(RequestExecutor* executors, uint8_t count);

	virtual Request executeJson(String target,String page,String json);

	virtual Request executeJsonGet(String getText);

	virtual Request executeJsonPost(String postText);

	virtual RequestExecutor* getExecutors();
	virtual uint8_t getExecutorCount();

protected:
	virtual Request executeRequest(Request* request);
	virtual Request executeStringToThreeParams(String executeText);

private:
	RequestExecutor* request;
	Url_Registerer* registerer;

};

#endif /* LIBRARIES_DEVICELIB_ENTITYLIB_REQUESTMANAGER_H_ */

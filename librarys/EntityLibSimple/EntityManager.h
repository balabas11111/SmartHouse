/*
 * EntityManager.h
 *
 *  Created on: Aug 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITYMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITYMANAGER_H_

#include <list>
#include <functional>

#include <Arduino.h>
#include <ArduinoJson.h>
#include <JsonObjectUtil.h>

#include <Entity.h>

/*
#define GROUP "Group"
#define  NAME "Name";
#define  BODY "Body";
*/
const char GROUP[] PROGMEM ="Group";
const char NAME[] PROGMEM ="Name";
const char BODY[] PROGMEM ="Body";

const char DEFAULT_VALUE[] PROGMEM ="DEFAULT";
const char EMPTY_GROUP[] PROGMEM ="Empty group";

const char HTTP_GET[] PROGMEM ="GET";
const char HTTP_POST[] PROGMEM ="POST";

class EntityManager {
public:
	EntityManager(Entity* entities[],int count);
	virtual ~EntityManager(){};

	void processEvent(int entityIndex);
	void executeMethod(JsonObject& params, JsonObject& response, const char* method);

protected:
	std::list<Entity*> entities;
	const int count;

	std::list<Entity*> getEntitiesByGroup(const char* group);
	Entity* getEntityByGroupAndName(const char* group, const char* name);

	void executeMethod(JsonObject& params, JsonObject& response, const char* method, Entity* entity);
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYMANAGER_H_ */

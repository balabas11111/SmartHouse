/*
 * EntityManager.h
 *
 *  Created on: Aug 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITYMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITYMANAGER_H_

#include "Arduino.h"
#include "ArduinoJson.h"
#include "JsonObjectUtil.h"
#include "FileUtils.h"

#include <list>
#include <functional>

#include "Entity.h"
#include "WiFi/WiFiSettingsBox.h"

#include "EntityJsonRequestResponse.h"
//#include <ApplicationContext.h>

/*
 #define GROUP "Group"
 #define  NAME "Name";
 #define  BODY "Body";
 */

#define FILE_PATH "/data/entity/entity.json"
#define PARAMETERS "params"
#define RESPONSE "resp"


const char DEFAULT_VALUE[] PROGMEM = "DEFAULT";
const char EMPTY_GROUP[] PROGMEM = "Empty group";


#define METHOD "Method"
#define REQUEST_GET "GET"
#define REQUEST_POST "POST"

class EntityManager {
public:
	EntityManager(Entity* entities[], int count, WiFiSettingsBox* conf, std::function<void(void)> onEntitiesChanged = nullptr);
	virtual ~EntityManager() {
	}

	void registerAndPreInitEntity(Entity* entity);
	void init();

	void get(EntityJsonRequestResponse* reqResp);
	void post(EntityJsonRequestResponse* reqResp);

	void executeMethod(EntityJsonRequestResponse* reqResp, const char* method = REQUEST_GET);

	void executeMethod(JsonObject& params, JsonObject& response,
			const char* method = REQUEST_GET);

	void loadEntitiesFromFile();
	void saveEntitiesToFile();

	std::list<Entity*>& getEntities() {
		return this->entities;
	}

	void groupNameToParam(char* group, char* name, EntityJsonRequestResponse* json);

	EntityJsonRequestResponse* createEntityJsonRequestResponse();
	void deleteEntityJsonRequestResponse(EntityJsonRequestResponse* json);

	bool processChangedEntities();

	void print();

	void setOnEntitiesChanged(std::function<void(void)> onEntitiesChanged);

	WiFiSettingsBox* getConf();

	char* getSensorsGroup();

protected:
	std::list<Entity*> entities;
	int count=0;
	int failPreinit=0;

	int initOk=0;
	int failValidate=0;

	Entity* getEntityByGroupAndName(const char* group, const char* name);

	Entity* getEntityByGroupAndNameFromParams(JsonObject& params);

	void addNotAllowed(JsonObject& response, const char* method);
	bool executeMethodOnEntity(JsonObject& params, JsonObject& response, Entity* entity,
									const char* method = REQUEST_GET);

	bool executeMethodOnAll(JsonObject& params, JsonObject& response,
			const char* method);

	bool executeMethodOnGroup(JsonObject& params, JsonObject& response,
				const char* method);

	bool hasNoGroupNoName(JsonObject& params);

	bool hasAllGroupNoName(JsonObject& params);

	bool hasGroupNoName(JsonObject& params);

	bool hasGroupName(JsonObject& params);

	void executeLoadOnEntity(JsonObject& json, Entity* entity);
	void executeSaveOnEntity(JsonObject& json, Entity* entity);

	void persist(std::function<void(JsonObject& json, Entity* entity)> onEntityFunction,
					std::function<void(JsonObject& json)> postPersistFunction);

	bool finishChangesProcess();

	void markEntitiesAsChanged();

	DynamicJsonBuffer buf;
	JsonObject* obj;

	bool entitiesChanged = false;

	std::function<void(void)> onEntitiesChanged;

	WiFiSettingsBox* conf;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYMANAGER_H_ */

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
#include <FileUtils.h>


#include <Entity.h>
#include <EntityJsonRequestResponse.h>

#include <functional>
/*
 #define GROUP "Group"
 #define  NAME "Name";
 #define  BODY "Body";
 */

#define FILE_PATH "/data/entity/entity.json"
#define PARAMETERS "params"
#define RESPONSE "resp"

#define GROUP "Group"
#define NAME "Name"
#define BODY "Body"

const char DEFAULT_VALUE[] PROGMEM = "DEFAULT";
const char EMPTY_GROUP[] PROGMEM = "Empty group";


#define METHOD "Method"
#define REQUEST_GET "GET"
#define REQUEST_POST "POST"

class EntityManager {
public:
	EntityManager(Entity* entities[], int count, std::function<void(void)> onEntityChanged = nullptr);
	virtual ~EntityManager() {
	}

	void registerAndPreInitEntity(Entity* entity);
	void init();

	void processEntityChangedEvent(int entityIndex);

	void get(EntityJsonRequestResponse* reqResp);
	void post(EntityJsonRequestResponse* reqResp);

	void executeHttpMethod(JsonObject& params, JsonObject& response,
			const char* method);

	void executeHttpMethod(EntityJsonRequestResponse* reqResp,
				const char* method);

	void loadEntitiesFromFile();
	void saveEntitiesToFile();

	std::list<Entity*>& getEntities() {
		return this->entities;
	}

	void groupNameToParam(char* group, char* name, EntityJsonRequestResponse* json);

	EntityJsonRequestResponse* createEntityJsonRequestResponse();
	void deleteEntityJsonRequestResponse(EntityJsonRequestResponse* json);

	void dispatchAllChangedEntities();

	void loop();
protected:
	std::list<Entity*> entities;
	int count=0;

	Entity* getEntityByGroupAndName(const char* group, const char* name);

	void addNotAllowed(JsonObject& response, const char* method);
	void executeHttpMethodOnEntity(JsonObject& params, JsonObject& response,
									const char* method, Entity* entity);

	bool executeHttpMethodOnAll(JsonObject& params, JsonObject& response,
			const char* method);

	bool executeHttpMethodOnGroup(JsonObject& params, JsonObject& response,
				const char* method);

	bool executeHttpMethodOnEntityOnly(JsonObject& params, JsonObject& response,
					const char* method);

	bool hasNoGroupNoName(JsonObject& params);

	bool hasAllGroupNoName(JsonObject& params);

	bool hasGroupNoName(JsonObject& params);

	bool hasGroupName(JsonObject& params);

	void executeLoadOnEntity(JsonObject& json, Entity* entity);
	void executeSaveOnEntity(JsonObject& json, Entity* entity);

	void persist(std::function<void(JsonObject& json, Entity* entity)> onEntityFunction,
					std::function<void(JsonObject& json)> postPersistFunction);

	DynamicJsonBuffer buf;
	JsonObject* obj;

	bool entitiesChanged = false;

	std::function<void(void)> onEntityChanged;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYMANAGER_H_ */

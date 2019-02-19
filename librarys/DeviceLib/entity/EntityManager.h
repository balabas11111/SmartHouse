/*
 * EntityManager.h
 *
 *  Created on: Feb 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_ENTITYMANAGER_H_
#define LIBRARIES_DEVICELIB_ENTITY_ENTITYMANAGER_H_

#include  <Arduino.h>
#include <ArduinoJson.h>
#include <entity/Entity.h>
#include <list>
#include <list/ArrayList.h>
#include <utils/FileUtils.h>

const PROGMEM char BASE_PATH[]   ="/Store/EntityDao/";
const PROGMEM char MODEL_FILE[]   ="Model.json";
const PROGMEM char DATA_FILE[]   ="Data.json";

const PROGMEM char STORAGE_PATH[]= "/Store/EntityDao/Data.json";

const PROGMEM char MODEL_PATH[]  ="/Store/EntityDao/Model/";
const PROGMEM char DATA_PATH[]   ="/Store/EntityDao/Data/";
const PROGMEM char TEMP_PATH[]   ="/Store/EntityDao/Temp/";
const PROGMEM char CACHE_PATH[]  ="/Store/EntityDao/Cache/";

const PROGMEM char MODEL_KEY[] = "Model";
const PROGMEM char DATA_KEY[]  = "Data";

class EntityManager {
public:
	EntityManager(Entity* items[]);
	virtual ~EntityManager(){};

	std::list<Entity*>& getEntities();

	int begin();

	JsonObject& getRoot();

	JsonObject& getModel();
	JsonObject& getData();

	JsonObject& getEntityModel(String name);
	JsonObject& getEntityData(String name);

	template <typename T>
	T getEntityModelValue<T>(String entityName,String keyName);

	template <typename T>
	T getEntityDataValue<T>(String entityName,String keyName);

	DynamicJsonBuffer& getRootBuf();

	int copyJsonObjects(JsonObject& from, JsonObject& to,bool over);

protected:
	int initModels();

	bool ok=false;
	std::list<Entity*> entities;

	DynamicJsonBuffer tmpBuf;
	DynamicJsonBuffer rootBuf;
	JsonVariant root;
};

template<typename T>
inline T EntityManager::getEntityModelValue<T>(String entityName,
		String keyName) {

	return getEntityModel(entityName).get<T>(keyName);
}

template<typename T>
inline T EntityManager::getEntityDataValue<T>(String entityName,
		String keyName) {

	return getEntityData(entityName).get<T>(keyName);
}

#endif /* LIBRARIES_DEVICELIB_ENTITY_ENTITYMANAGER_H_ */

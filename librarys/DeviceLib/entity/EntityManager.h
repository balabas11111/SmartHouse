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
#include <entity/EntityConsts.h>
#include <entity/model/base/Entity.h>
#include <list>
#include <utils/FileUtils.h>
//Storage paths
//filenames

const PROGMEM char ENTITY_MANAGER_STORAGE_FILE_PATH[]= "/Store/Entity/store.json";

/*
const PROGMEM char TEMP_PATH[]   ="/Store/Entity/Temp/";
const PROGMEM char CACHE_PATH[]  ="/Store/Entity/Cache/";

 //root/model/dao/entityName - description of model
 //root/data/dao/entityName  - data fields/tables
 */
class EntityManager {
public:
	EntityManager(Entity* items[]);
	virtual ~EntityManager(){};

	std::list<Entity*>& getEntities();

	int begin();

	JsonObject& getRoot();
	JsonObject& getTmp();

	JsonObject& getModel();
	JsonObject& getData();
	JsonObject& getWebTempl();

	JsonObject& getTmpModel();
	JsonObject& getTmpData();
/*
	JsonObject& getDaoModel(const char* daoName);
	JsonObject& getDaoData(const char* daoName);

	JsonObject& getEntityModel(const char* daoName,const char* entityName);
	JsonObject& getEntityData(const char* daoName,const char* entityName);

	JsonObject& getEntityModel_node(const char* daoName,const char* entityName,const char* nodeName);
	JsonObject& getEntityData_node(const char* daoName,const char* entityName,const char* nodeName);

	template <typename T>
	T getEntity_Val(const char* daoName,const char* entityName,const char* field){
		return getEntityModel_node(daoName,entityName,JSONKEY_data).get<T>(field);
	}

	template <typename T>
	T setEntity_Val(const char* daoName,const char* entityName,const char* field,T value){
		return getEntityModel_node(daoName,entityName,JSONKEY_data).set<T>(field,value);
	}

	bool containsKey(JsonObject& obj,const char* key);
*/

/*
	template <typename T>
	T getEntityData_RootVal(const char* daoName,const char* entityName,const char* keyName){
		return getEntityData(entityName).get<T>(keyName);
	}

	template <typename T>
	T getEntityData_FieldValue(){

	}
*/
	DynamicJsonBuffer& getRootBuf();

	int copyJsonObjects(JsonObject& from, JsonObject& to,bool over);

protected:
	int initEntity(Entity* entity);
	int loadEntity(Entity* entity);
	int prepareTemplateFields(Entity* entity);

	int copyObjFields(JsonObject& from, JsonObject& to);

	int init();
	const char* getProgmemKey(String key);

	bool ok=false;
	std::list<Entity*> entities;

	DynamicJsonBuffer tmpBuf;
	DynamicJsonBuffer rootBuf;
	JsonVariant tmp;
	JsonVariant root;

	bool entFileExists=false;
	int templFields=0;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_ENTITYMANAGER_H_ */

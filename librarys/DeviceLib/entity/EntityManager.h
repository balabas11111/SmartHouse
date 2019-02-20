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
#include <entity/Entity.h>
#include <list>
#include <utils/FileUtils.h>
//Storage paths
//filenames
const PROGMEM char BASE_PATH[]   ="/Store/EntityDao/";
const PROGMEM char MODEL_FILE[]   ="Model.json";
const PROGMEM char DATA_FILE[]   ="Data.json";

const PROGMEM char STORAGE_PATH[]= "/Store/EntityDao/Data.json";

const PROGMEM char MODEL_PATH[]  ="/Store/EntityDao/Model/";
const PROGMEM char DATA_PATH[]   ="/Store/EntityDao/Data/";
const PROGMEM char TEMP_PATH[]   ="/Store/EntityDao/Temp/";
const PROGMEM char CACHE_PATH[]  ="/Store/EntityDao/Cache/";
/*
 //root/model/dao/entityName/Model - description of model
 //root/data/dao/entityName/Data  - data fields/tables
 */
class EntityManager {
public:
	EntityManager(Entity* items[]);
	virtual ~EntityManager(){};

	std::list<Entity*>& getEntities();

	int begin();

	JsonObject& getRoot();

	JsonObject& getModel();
	JsonObject& getData();

	JsonObject& getDaoModel(const char* daoName);
	JsonObject& getDaoData(const char* daoName);

	JsonObject& getEntityModel(const char* daoName,const char* entityName);
	JsonObject& getEntityData(const char* daoName,const char* entityName);

	JsonObject& getEntityModel_field(const char* daoName,const char* entityName,const char* fieldName);
	JsonObject& getEntityData_field(const char* daoName,const char* entityName);
/*
	template <typename T>
	T getEntityModel_RootVal(const char* daoName,const char* entityName,const char* keyName){
		return getEntityModel(entityName).get<T>(keyName);
	}

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
	int initEntitysModel(Entity* entity);
	int loadEntitysModel(Entity* entity);
	int loadEntitysData(Entity* entity);
	int prepareTemplateFields(Entity* entity);

	int init();

	bool ok=false;
	std::list<Entity*> entities;

	DynamicJsonBuffer tmpBuf;
	DynamicJsonBuffer rootBuf;
	JsonVariant root;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_ENTITYMANAGER_H_ */

/*
 * Executed base operations on data set
 *
 *  Created on: 2 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIB_JSONDAO_H_
#define LIBRARIES_ENTITYLIB_JSONDAO_H_

#include <list>
#include <utility>
#include <string>
#include <sstream>

#include <Arduino.h>
#include <ArduinoJson.h>
#include <EntityJson.h>

#include <FileUtils.h>
#include <JsonObjectUtil.h>

#include "EntityModelDataProvider.h"
#include "EntityConsts.h"
/*
{"model": {
  	"sensors":{
  		"BME280":{"id": 0,
  				  	"name":"BME280",
 	  				"descr":"BME280 hum/temp/press",
  				  "fields":{"id":{"default":0},"name":{"default"}},
  				  "actions":{
  							"stat":["id","name"],
 	 	  					"var":["descr"],
 	 	  					"load":["descr"],
 	 	  					"save":["descr"],
 	 	  					"set":["descr"]
  						}
  				  }
  			  },
  	"settings":{

  	           },
  	"services":{
  			   }
  		},
"data":{},
"template":{}
}

 */

class JsonDao: public EntityModelDataProvider {
public:
	JsonDao(EntityJson* entitiesIn[],int size);
	virtual ~JsonDao(){};

	void init();
	std::list<EntityJson*>* getEntities();
	EntityJson* getEntity(int i);

	JsonObject& getRoot();
	JsonObject& getRootPath(const char* rootPath);
	JsonObject& getRootPathGroup(const char* rootPath,const char* group);
	JsonObject& getRootPathGroupEntity(const char* rootPath,const char* group,const char* entName);

	void createEntityJson(const char* rootPath,EntityJson* ent);

	void printEntities();

	void initEntityModel(EntityJson* ent);
	void createEntityPrimaryFields(EntityJson* ent,JsonObject& data);

	JsonObject& loadRootIfExists();
	bool saveJsonObjectToFile(const char* fileName,JsonObject& json);

	void initModels();
	void persistModels();
	void saveModels();

	void initDatas();
	void persistDatas();
	void saveDatas();

	void initTemplates();
	void persistTemplates();
	void saveTemplates();
	void cleanTemplates();

	void generateTemplateKey(EntityJson* ent,const char* key);
	String getByTemplateKey(const char* key);

	virtual bool hasFieldInt(int entityId,const char* key) override;
	virtual bool hasFieldFloat(int entityId,const char* key) override;
	virtual bool hasFieldConstChar(int entityId,const char* key) override;
	virtual bool hasFieldChar(int entityId,const char* key) override;

	virtual bool setField(int entityId,const char* key,int value) override;
	virtual bool setField(int entityId,const char* key,float value) override;
	virtual bool setField(int entityId,const char* key,const char* value) override;
	virtual bool setField(int entityId,const char* key,char* value) override;

	virtual int getFieldInt(int entityId,const char* key) override;
	virtual float getFieldFloat(int entityId,const char* key) override;
	virtual const char* getFieldConstChar(int entityId,const char* key) override;
	virtual char* getFieldChar(int entityId,const char* key) override;

	virtual JsonObject& getEntityModel(int entityId) override;
	virtual JsonObject& getEntityData(int entityId) override;

	JsonObject& getEntityModel(EntityJson* e);
	JsonObject& getEntityData(EntityJson* e);
	JsonObject& getEntityDeployed(EntityJson* e);

	JsonObject& getEntityModelAllFields(EntityJson* e);
	JsonObject& getEntityModelActions(EntityJson* e);

	JsonArray& getEntityModelStatFields(EntityJson* e);
	JsonArray& getEntityModelVarFields(EntityJson* e);
	JsonArray& getEntityModelTemplateVarFields(EntityJson* e);

	JsonArray& getEntityDataLoadFields(EntityJson* e);
	JsonArray& getEntityDataSaveFields(EntityJson* e);
	JsonArray& getEntityDataSetFields(EntityJson* e);

	bool getFieldHasAction(EntityJson* e,const char* key,const char* action);

	template<typename T>
	bool hasField(int entityId,const char* key){
		JsonObject& obj = getEntityData(getEntity(entityId));
		return JsonObjectUtil::hasField<T>(obj,key);
	}
	template<typename T>
	bool setField(int entityId,const char* key,T value){
		EntityJson* ent = getEntity(entityId);
		JsonObject& obj = getEntityData(ent);
		bool changed = JsonObjectUtil::setField<T>(obj,key,value);

		ent->setChanged(changed);
		dispatchFieldChange(ent, key);

		return changed;
	}
	template<typename T>
	T getField(int entityId,const char* key){
		JsonObject& obj = getEntityData(getEntity(entityId));
		return JsonObjectUtil::getField<T>(obj,key);
	}

	template<typename T>
	void registerEntityModelField(JsonObject& model,const char* key,T defValue,bool stat,bool var,bool load, bool save, bool deploy,bool set){
		JsonObjectUtil::getObjectChildOrCreateNew(model, JSONKEY_fields);
		JsonObjectUtil::getObjectChildOrCreateNew(model, JSONKEY_actions);

		JsonObject& fields = model.get<JsonObject>(JSONKEY_fields);
		JsonObject& actions = model.get<JsonObject>(JSONKEY_actions);

		setField(fields,key,defValue);

		JsonObjectUtil::getObjectChildArrayOrCreateNew(actions, JSONKEY_stat);
		JsonObjectUtil::getObjectChildArrayOrCreateNew(actions, JSONKEY_var);
		JsonObjectUtil::getObjectChildArrayOrCreateNew(actions, JSONKEY_load);
		JsonObjectUtil::getObjectChildArrayOrCreateNew(actions, JSONKEY_save);
		JsonObjectUtil::getObjectChildArrayOrCreateNew(actions, JSONKEY_deploy);
		JsonObjectUtil::getObjectChildArrayOrCreateNew(actions, JSONKEY_set);

		if(stat){actions.get<JsonArray>(JSONKEY_stat).add(key);}
		if(var){actions.get<JsonArray>(JSONKEY_var).add(key);}
		if(load){actions.get<JsonArray>(JSONKEY_load).add(key);}
		if(save){actions.get<JsonArray>(JSONKEY_save).add(key);}
		if(deploy){actions.get<JsonArray>(JSONKEY_deploy).add(key);}
		if(set){actions.get<JsonArray>(JSONKEY_set).add(key);}
	}

	void dispatchFieldChange(EntityJson* entity,const char* key){
		Serial.print(FPSTR("Field changed Entity="));
		Serial.print(entity->getName());
		Serial.print(FPSTR(" key="));
		Serial.println(key);
		//TODO: Implement this method
	}

protected:
	bool isDefaultField(const char* name);
	void addFieldToActions(JsonObject& actions, const char* kind, const char* key);
	int loadedModelToModel(EntityJson* model,JsonObject& loaded);
	int loadedDataToData(EntityJson* data,JsonObject& loaded);

	int mergeModels(JsonObject& from,JsonObject& to);

	JsonObject& getEntitysJson(const char* rootPathJson,EntityJson* entity);
	JsonObject& getEntitysJson(const char* rootPathJson,int entityId);

	std::list<EntityJson*> entities;

	DynamicJsonBuffer buf;
	DynamicJsonBuffer bufTmp;

	JsonObject& root = this->buf.parse("{}").as<JsonObject>();
	JsonObject& rootTmp = this->bufTmp.parse("{}").as<JsonObject>();
};

#endif /* LIBRARIES_ENTITYLIB_JSONDAO_H_ */

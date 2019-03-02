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

#include <Arduino.h>
#include <ArduinoJson.h>
#include <EntityJson.h>

#include <MathUtils.h>
#include <FileUtils.h>
#include <JsonObjectUtil.h>

#define ROOT_PATH_MODEL "model"
#define ROOT_PATH_DATA "data"
#define ROOT_PATH_DEPLOYED "deploy"
#define ROOT_PATH_TEMP "temp"

#define ROOT_GROUP_SENSORS "sensors"
#define ROOT_GROUP_SETTINGS "settings"
#define ROOT_GROUP_SERVICES "services"

#define JSONKEY_stat "stat"
#define JSONKEY_var "var"
#define JSONKEY_load "load"
#define JSONKEY_save "save"
#define JSONKEY_deploy "deploy"
#define JSONKEY_set "set"

#define JSONKEY_id      "id"
#define JSONKEY_name    "name"
#define JSONKEY_group   "group"
#define JSONKEY_fields  "fields"
#define JSONKEY_actions "actions"
#define JSONKEY_declaredFields  "declaredFields"
#define JSONKEY_declaredActions "declaredActions"
#define JSONKEY_descr   "descr"
#define JSONKEY_path    "path"
#define JSONKEY_dao     "dao"
#define JSONKEY_class   "class"
#define JSONKEY_open    "open"

#define PATH_MODEL_file    "/data/entity/model/data.json"

const char* const ROOT_PATHS[] PROGMEM = {
		ROOT_PATH_MODEL,
		ROOT_PATH_DATA,
		ROOT_PATH_DEPLOYED,
		ROOT_PATH_TEMP

};

const int ROOT_PATHS_TOTAL PROGMEM = 4;

const char* const ROOT_PATHS_GROUPS[] PROGMEM = {
		ROOT_GROUP_SENSORS,
		ROOT_GROUP_SETTINGS,
		ROOT_GROUP_SERVICES
};

const int ROOT_PATHS_GROUPS_TOTAL PROGMEM = 3;

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

class JsonDao {
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

	JsonObject& getObjectOrCreateString(JsonObject& parent,String key);

	JsonObject& getObjectOrCreateChar(JsonObject& parent,char* key);
	JsonObject& getObjectOrCreate(JsonObject& parent,const char* key);
	JsonArray& getArrayOrCreate(JsonObject& parent,const char* key);

	void registerEntityModel(EntityJson* ent);

	void loadEntityModelFile();
	void saveEntityModelFile();

	void initDataByModel();
	void initTemplateByModel();

	template<typename T>
	void registerEntityModelField(JsonObject& model,const char* key,T defValue,bool stat,bool var,bool load, bool save, bool deploy,bool set){
		getObjectOrCreate(model, JSONKEY_fields);
		getObjectOrCreate(model, JSONKEY_actions);

		JsonObject& fields = model.get<JsonObject>(JSONKEY_fields);
		JsonObject& actions = model.get<JsonObject>(JSONKEY_actions);

		setField(fields,key,defValue);

		getArrayOrCreate(actions, JSONKEY_stat);
		getArrayOrCreate(actions, JSONKEY_var);
		getArrayOrCreate(actions, JSONKEY_load);
		getArrayOrCreate(actions, JSONKEY_save);
		getArrayOrCreate(actions, JSONKEY_deploy);
		getArrayOrCreate(actions, JSONKEY_set);

		if(stat){actions.get<JsonArray>(JSONKEY_stat).add(key);}
		if(var){actions.get<JsonArray>(JSONKEY_var).add(key);}
		if(load){actions.get<JsonArray>(JSONKEY_load).add(key);}
		if(save){actions.get<JsonArray>(JSONKEY_save).add(key);}
		if(deploy){actions.get<JsonArray>(JSONKEY_deploy).add(key);}
		if(set){actions.get<JsonArray>(JSONKEY_set).add(key);}

	}

	template<typename T>
	void getFieldOrCreate(JsonObject& parent,const char* key, T value){
		if(!parent.containsKey(key) || !parent.is<T>(key)){
			parent.set<T>(key,value);
		}
	}
	template<typename T>
	bool setField(JsonObject& parent,const char* key,T value){
		bool changed=false;

		getFieldOrCreate(parent,key,value);
		if(parent.get<T>(key)!=value){
			parent.set<T>(key, value);
			changed=true;
		}

		return changed;
	}
	template<typename T>
	T getField(JsonObject& parent,const char* key){
		return parent.get<T>(key);
	}

protected:
	bool isDefaultField(const char* name);
	void addFieldToActions(JsonObject& actions, const char* kind, const char* key);
	int loadEntityModel(EntityJson* ent,JsonObject& loadedModel);

	std::list<EntityJson*> entities;
	DynamicJsonBuffer buf;

	JsonObject& root = this->buf.parse("{}").as<JsonObject>();
	DynamicJsonBuffer bufTmp;

	JsonObject& rootTmp = this->bufTmp.parse("{}").as<JsonObject>();
};

#endif /* LIBRARIES_ENTITYLIB_JSONDAO_H_ */

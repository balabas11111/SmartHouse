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
#include "EventSender.h"
#include "ESP_Consts.h"
/*
{"model": {
  	"sensors":{
		"BME280":{			"stat":["id","name"],
						"var":["descr"],
						"load":["descr"],
						"save":["descr"],
						"set":["descr"]

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

	JsonObject& getEntitysJson_ByPath_OrCreateNew(JsonObject& container,const char* path,EntityJson* entity);
	JsonObject& getEntitysJson_ByPath(JsonObject& container,const char* path,EntityJson* entity);
	JsonObject& getEntitysJson_ByPath_entId(JsonObject& container,const char* path,int entityId);

	virtual JsonObject& getEntityRoot() override;

	void printEntities();

	void initEntityModelData(EntityJson* entity,JsonObject& loaded);
	int createEntityDataPrimaryFields(EntityJson* entity,JsonObject& data);

	JsonObject& loadRootIfExists();
	bool saveJsonObjectToFile(const char* fileName,JsonObject& json);

	void initEntitiesModelData();
#ifdef DEPLOY_TEMPLATES
	void initTemplates();
	void persistTemplates();
	void saveTemplates();
	void cleanTemplates();

	void generateTemplateKey(EntityJson* entity,const char* key);
	String getByTemplateKey(const String& var) override;
#endif
	virtual JsonObject& getEntityModel(int entityId) override;
	virtual JsonObject& getEntityData(int entityId) override;
	virtual JsonArray& getEntityDataFieldsByAction(int entityId,const char* action);
	virtual const char* validateField(int entityId,const char* key,const String& value);

	bool getEntityHasAction(EntityJson* entity,const char* action);
	bool getEntityDataFieldHasAction(EntityJson* entity,const char* dataFieldName,const char* action);
	JsonArray& getEntityDataFieldsByAction(EntityJson* entity,const char* action);

	virtual bool setField(int entityId,const char* key,int value) override;
	virtual bool setField(int entityId,const char* key,float value) override;
	virtual bool setField(int entityId,const char* key,const char* value) override;
	virtual bool setField(int entityId,const char* key,char* value) override;
	virtual bool setField(int entityId,const char* key,String value) override;

	virtual int getFieldInt(int entityId,const char* key) override;
	virtual float getFieldFloat(int entityId,const char* key) override;
	virtual const char* getFieldConstChar(int entityId,const char* key) override;
	virtual char* getFieldChar(int entityId,const char* key) override;

	template<typename T>
	bool hasField(int entityId,const char* key){
		JsonObject& obj = getEntityData(entityId);
		return JsonObjectUtil::hasField<T>(obj,key);
	}
	template<typename T>
	bool setField(int entityId,const char* key,T value){
		EntityJson* entity = getEntity(entityId);
		JsonObject& data = getEntityData(entityId);
		bool changed = JsonObjectUtil::setField<T>(data,key,value);

		entity->setChanged(changed);
		dispatchFieldChange(entity, key);

		return changed;
	}
	template<typename T>
	T getField(int entityId,const char* key){
		JsonObject& obj = getEntityData(entityId);
		return JsonObjectUtil::getField<T>(obj,key);
	}
	void dispatchFieldChange(EntityJson* entity,const char* key){
		/*Serial.print(FPSTR("Field changed Entity="));
		Serial.print(entity->getName());
		Serial.print(FPSTR(" key="));
		Serial.println(key);
		*/
		if(eventSender!=nullptr && eventSender!=NULL){
			JsonObject& obj = getEntitysJson_ByPath(root, ROOT_PATH_DATA, entity);
			obj.set("chgKey", key);
			eventSender->sendAsEventSource(obj);
		}
	}

	virtual EventSender* getEventSender() override {
		return eventSender;
	}

	virtual void setEventSender(EventSender* eventSender){
		this->eventSender = eventSender;
	}

protected:
	bool isDefaultField(const char* name);

	int mergeModels(JsonObject& from,JsonObject& to);
	int mergeDatas(JsonObject& from, JsonObject& to);

	std::list<EntityJson*> entities;

	DynamicJsonBuffer buf;
	DynamicJsonBuffer bufTmp;

	JsonObject& root = this->buf.parse("{}").as<JsonObject>();
	JsonObject& rootTmp = this->bufTmp.parse("{}").as<JsonObject>();

	EventSender* eventSender;

	int incomeEntities;
	long initTime = 0;
	long beginTime = 0;
};

#endif /* LIBRARIES_ENTITYLIB_JSONDAO_H_ */

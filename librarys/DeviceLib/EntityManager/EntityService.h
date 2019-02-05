/*
 * EntityService.h
 *
 *  Created on: Feb 4, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYSERVICE_H_
#define LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYSERVICE_H_

#include <EntityManager/EntityFields.h>
#include "Arduino.h"

#include "interfaces/Identifiable.h"
#include "interfaces/Nameable.h"

#include "EspSettingsUtil.h"

#include "EntityManager/Entity.h"
#include "EntityManager/EntityFields.h"
#include "EntityManager/EntityServiceInt.h"

#include "EntityManager/EntityMinMax.h"
#include "EntityManager/EntityThSp.h"

const char DIV[] PROGMEM = "/";
const char TXT[] PROGMEM = ".txt";

class EntityService:public Identifiable, public Nameable, public EntityServiceInt {
public:
	EntityService(){
		entityCount=0;
		entityFieldsAll=nullptr;
		entities=nullptr;
	};
	virtual ~EntityService();

	void initEntitiesBase(){
		EntityFields* eb=new EntityMinMax();
		EntityFields* eb2=new EntityThSp();

		EntityFields* arr[2];

		arr[eb->getId()]=eb;
		arr[eb2->getId()]=eb2;

		entityFieldsAll=arr;
	}

	static void registerEntityCount(Entity* entity){
		entityCount+=entity->getSize();
	}

	void registerEntity(Entity* entity){

		uint8_t size=entity->getSize();
		EntityFields* arr[size];

		if(entity->inMemory()){
			for(uint8_t i=0;i<size;i++){
				uint8_t key=entity->getKeys()[i];
				EntityFields* cloned=entityFieldsAll[key]->clone();
				arr[i]=cloned;
			}

			entity->initEntity(arr,this);
		}else{
			entity->initEntity(nullptr,this);
		}
	}

	virtual boolean fileExists(String fileName)=0;
	virtual boolean saveStringToFile(String fileName,String data)=0;
	virtual String loadStringFromFile(String fileName)=0;

	virtual boolean saveEntityDefaults(String parent,uint8_t eId){
		Entity* entity=entities[eId];

		String result="{"+entity->getJson()+", \"entities\": [";
		for(uint8_t i=0;i<entity->getSize();i++){

			EntityFields* proto=entityFieldsAll[entity->getKeys()[i]];

			const char* const* keys=proto->getKeys();
			const char* const* values=proto->getDefaults();

			result+=proto->getJsonDefault()+",";

			for(uint8_t j=0;j<proto->getSize();j++){
				saveEntityValueCacheByNames(parent, entity->getName(), proto->getName(), keys[j], values[j]);
			}
		}

		result.setCharAt(result.length(), ']');
		result+="}";

		return saveEntityCache(parent, eId, result);
	}

	virtual String loadEntityCache(String parent,uint8_t eId);
	virtual String loadEntityValueCache(String parent,uint8_t eId,uint8_t eBid,uint8_t fId);
	virtual boolean saveEntityCache(String parent,uint16_t eId,String data);
	virtual boolean saveEntityCacheByNames(String parent,String entityName,String data);
	virtual boolean saveEntityValueCache(String parent,uint8_t eId,uint8_t eBid,uint8_t fId,String data);
	virtual boolean saveEntityValueCacheByNames(String parent,String entityName,String baseEntityName,String fieldName,String data);

	virtual String constructEntityPath(String parent,uint8_t eId);
	virtual String constructEntityPathByNames(String parent,String entityName);

	virtual String constructEntityValuePath(String parent,uint8_t eId,uint8_t eBid,uint8_t fId);
	virtual String constructEntityValuePathByNames(String parent,String entityName,String baseEntityName,String fieldName);
private:
	EntityFields** entityFieldsAll;
	Entity **entities;

	static int entityCount;


};

#endif /* LIBRARIES_DEVICELIB_ENTITYMANAGER_ENTITYSERVICE_H_ */

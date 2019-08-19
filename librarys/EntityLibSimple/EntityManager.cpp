/*
 * EntityManager.cpp
 *
 *  Created on: Aug 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <EntityManager.h>

EntityManager::EntityManager(Entity* entities[],int count) {
	for(int i=0;i<count;i++){
		Entity* entity = entities[i];
		entity->initialize(i, [this](int val){processEvent(val);});
		this->entities.push_back(entity);
	}

	this->count = this->entities.size();
}

void EntityManager::processEvent(int entityIndex) {

}

std::list<Entity*> EntityManager::getEntitiesByGroup(const char* group) {
	std::list<Entity*> result = new std::list<Entity*>();

	for(Entity* entity : this->entities){
		if(strcmp(entity->getGroup(), group)){
			result.push_back(entity);
		}
	}

	return result;
}

Entity* EntityManager::getEntityByGroupAndName(const char* group,
		const char* name) {
	for(Entity* entity : this->entities){
		if(entity->isTarget(group, name)){
			return entity;
		}
	}

	return nullptr;
}

void EntityManager::executeMethod(JsonObject& params, JsonObject& response,
		const char* method) {

	if(!JsonObjectUtil::hasField<const char*>(params, GROUP)
				&& !JsonObjectUtil::hasField<const char*>(params, NAME)){

		for(Entity* entity: this->entities){
			executeMethod(params, response, method, entity);
		}
		return;
	}

	if(JsonObjectUtil::hasField<const char*>(params, GROUP)
			&& !JsonObjectUtil::hasField<const char*>(params, NAME)){

		std::list<Entity*> entitiesToGet = getEntitiesByGroup(JsonObjectUtil::getFieldIfKeyExistsOrDefault(params,GROUP,DEFAULT_VALUE));

		if(entitiesToGet==nullptr || entitiesToGet.size()==0){
			JsonObjectUtil::setField(response, MESSAGE, NOT_FOUND);
			return;
		}

		for(Entity* entity: entitiesToGet){
			executeMethod(params, response, method, entity);
		}
	}

	if(JsonObjectUtil::hasField<const char*>(params, GROUP)
			&& JsonObjectUtil::hasField<const char*>(params, NAME)){

		executeMethod(params, response, method,
				getEntityByGroupAndName(JsonObjectUtil::getFieldIfKeyExistsOrDefault(params,GROUP,DEFAULT_VALUE),
										JsonObjectUtil::getFieldIfKeyExistsOrDefault(params,NAME,DEFAULT_VALUE)));
		return;
	}
}

void EntityManager::executeMethod(JsonObject& params, JsonObject& response,
		const char* method, Entity* entity) {

	Serial.print(FPSTR("Execute method "));
	Serial.println(method);

	JsonObjectUtil::print(params);

	if(entity == nullptr){
		JsonObjectUtil::setField(response, MESSAGE, NOT_FOUND);
		return;
	}

	if(strcmp(method, HTTP_GET) == 0){
		entity->executeGet(params, JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(response, entity->getGroup(), entity->getName()));
	} else if(strcmp(method, HTTP_POST) == 0){
		entity->executePost(JsonObjectUtil::getFieldIfKeyExistsOrDefault<JsonObject&>(params,BODY,params),
							JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(response, entity->getGroup(), entity->getName()));
	} else {
		JsonObjectUtil::setField(response, MESSAGE, BAD_METHOD);
	}
}



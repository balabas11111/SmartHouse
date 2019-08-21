/*
 * EntityManager.cpp
 *
 *  Created on: Aug 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <EntityManager.h>

EntityManager::EntityManager(Entity* entities[], int count) {
	for (int i = 0; i < count; i++) {
		Entity* entity = entities[i];
		entity->initialize(i,
				[this](int val) {processEntityChangedEvent(val);});
		this->entities.push_back(entity);
	}

	this->count = this->entities.size();
}

void EntityManager::processEntityChangedEvent(int entityIndex) {
	Serial.print(FPSTR("Change event id="));
	Serial.println(entityIndex);
}

std::list<Entity*> EntityManager::getEntitiesByGroup(const char* group) {
	std::list<Entity*>* result = new std::list<Entity*>();

	for (Entity* entity : this->entities) {
		if (strcmp(entity->getGroup(), group)) {
			result->push_back(entity);
		}
	}

	return *result;
}

Entity* EntityManager::getEntityByGroupAndName(const char* group,
		const char* name) {
	for (Entity* entity : this->entities) {
		if (entity->isTarget(group, name)) {
			return entity;
		}
	}

	return nullptr;
}

void EntityManager::addNotAllowed(JsonObject& response, const char* method) {
	JsonObjectUtil::setField(response, MESSAGE, NOT_ALLOWED);
	JsonObjectUtil::setField(response, METHOD, method);
}

void EntityManager::executeHttpMethod(JsonObject& params, JsonObject& response,
		const char* method) {

	if (!JsonObjectUtil::hasField<const char*>(params, GROUP)
			&& !JsonObjectUtil::hasField<const char*>(params, NAME)) {

		for (Entity* entity : this->entities) {
			executeHttpMethodOnEntity(params, response, method, entity);
		}
		return;
	}

	if (JsonObjectUtil::hasField<const char*>(params, GROUP)
			&& !JsonObjectUtil::hasField<const char*>(params, NAME)) {

		std::list<Entity*> entitiesToGet = getEntitiesByGroup(
				JsonObjectUtil::getFieldIfKeyExistsOrDefault(params, GROUP,
						DEFAULT_VALUE));

		if (entitiesToGet.size() == 0) {
			JsonObjectUtil::setField(response, MESSAGE, NOT_FOUND);
			return;
		}

		bool changed = false;

		for (Entity* entity : entitiesToGet) {
			entity->setChanged(false);

			executeHttpMethodOnEntity(params, response, method, entity);

			changed = changed || entity->isChanged();
		}

		if (changed) {
			saveEntitiesToFile();
		}

		return;
	}

	if (JsonObjectUtil::hasField<const char*>(params, GROUP)
			&& JsonObjectUtil::hasField<const char*>(params, NAME)) {

		Entity* entity = getEntityByGroupAndName(
				JsonObjectUtil::getFieldIfKeyExistsOrDefault(params, GROUP,
						DEFAULT_VALUE),
				JsonObjectUtil::getFieldIfKeyExistsOrDefault(params, NAME,
						DEFAULT_VALUE));
		entity->setChanged(false);
		executeHttpMethodOnEntity(params, response, method, entity);

		if (entity->isChanged()) {
			saveEntitiesToFile();
		}

		return;
	}
}

void EntityManager::executeHttpMethodOnEntity(JsonObject& params,
		JsonObject& response, const char* method, Entity* entity) {

	Serial.print(FPSTR("Execute method "));
	Serial.println(method);

	JsonObjectUtil::print(params);

	if (entity == nullptr) {
		JsonObjectUtil::setField(response, MESSAGE, NOT_FOUND);
		return;
	}

	if (strcmp(method, HTTP_GET) == 0) {
		if (entity->hasGetMethod()) {
			entity->executeGet(params,
					JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(response,
							entity->getGroup(), entity->getName()));
			return;
		}
	} else if (strcmp(method, HTTP_POST) == 0) {
		if (entity->hasPostMethod()) {
			entity->setChanged(false);
			entity->executePost(
					JsonObjectUtil::getFieldIfKeyExistsOrDefault<JsonObject&>(
							params, BODY, params),
					JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(response,
							entity->getGroup(), entity->getName()));
			return;
		}
	} else {
		JsonObjectUtil::setField(response, MESSAGE, BAD_METHOD);
		return;
	}

	addNotAllowed(response, method);
	return;
}

void EntityManager::executeLoadOnEntity(JsonObject& jsonFromFile,
		Entity* entity) {
	if (entity->canLoadState()) {
		entity->executeLoad(jsonFromFile);
	}
}

void EntityManager::executeSaveOnEntity(JsonObject& jsonToFile,
		Entity* entity) {
	if (entity->canSaveState()) {
		entity->executeSave(jsonToFile);
	}
}

void EntityManager::init() {
	FileUtils::init();

	for (Entity* entity : entities) {
		entity->init();
	}

	loadEntitiesFromFile();
}

void EntityManager::loadEntitiesFromFile() {
	Serial.println(FPSTR("Load entities"));
	persist(
			[this](JsonObject& json, Entity* entity) {executeLoadOnEntity(json,entity);},
			nullptr);
}

void EntityManager::saveEntitiesToFile() {
	Serial.println(FPSTR("Save entities"));
	persist(
			[this](JsonObject& json, Entity* entity) {executeSaveOnEntity(json,entity);},
			[this](JsonObject& json) {FileUtils::saveJsonToFileIfDiff(FILE_PATH, json);});
}

void EntityManager::persist(
		std::function<void(JsonObject& json, Entity* entity)> onEntityFunction,
		std::function<void(JsonObject& json)> postPersistFunction) {

	Serial.println(FPSTR("-------------"));
	buf.clear();

	FileUtils::loadJsonFromFile(FILE_PATH, buf, obj);
	JsonObjectUtil::print(obj);

	for (Entity* entity : entities) {
		JsonObject& json = JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(
				*obj, entity->getGroup(), entity->getName());
		entity->print();
		JsonObjectUtil::print(json);
		onEntityFunction(json, entity);
	}

	if (postPersistFunction != nullptr) {
		postPersistFunction(*obj);
	}

	buf.clear();

	Serial.println(FPSTR("------persisted-------"));
}

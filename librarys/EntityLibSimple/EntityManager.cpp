/*
 * EntityManager.cpp
 *
 *  Created on: Aug 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <EntityManager.h>

EntityManager::EntityManager(Entity* entities[], int count, std::function<void(void)> onEntityChanged) {
	for (int i = 0; i < count; i++) {
		registerAndPreInitEntity(entities[i]);
	}
	this->onEntityChanged = onEntityChanged;
}

void EntityManager::registerAndPreInitEntity(Entity* entity) {
	Serial.print(FPSTR("register "));
	entity->print();

	this->entities.push_back(entity);

	entity->preInitialize(this->count,
					[this](int val) {processEntityChangedEvent(val);});
	this->count++;
}

void EntityManager::processEntityChangedEvent(int entityIndex) {
	Serial.print(FPSTR("Change event id="));
	Serial.println(entityIndex);
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

void EntityManager::executeHttpMethod(EntityJsonRequestResponse* reqResp,
		const char* method) {
	executeHttpMethod(reqResp->getRequest(), reqResp->getResponse(),
			 method);
}

void EntityManager::executeHttpMethod(JsonObject& params, JsonObject& response,
		const char* method) {

	this->entitiesChanged = (this->entitiesChanged || false);
	bool changed = false;

	Serial.print(method);
	Serial.print(FPSTR(" "));

	JsonObjectUtil::printWithPreffix(PARAMETERS, params);

	if (hasNoGroupNoName(params) || hasAllGroupNoName(params)) {
		executeHttpMethodOnAll(params, response, method);
	}else
	if (hasGroupNoName(params)) {
		executeHttpMethodOnGroup(params, response, method);
	}else
	if (hasGroupName(params)) {
		executeHttpMethodOnEntityOnly(params, response, method);
	}

	JsonObjectUtil::printWithPreffix(RESPONSE, response);
}

void EntityManager::executeHttpMethodOnEntity(JsonObject& params,
		JsonObject& response, const char* method, Entity* entity) {
/*
	Serial.print(FPSTR("Execute method "));
	Serial.print(method);
	Serial.print(FPSTR(" Entity = "));
	Serial.println(entity->getName());
*/
	//JsonObjectUtil::print(params);

	if (entity == nullptr) {
		JsonObjectUtil::setField(response, MESSAGE, NOT_FOUND);
		return;
	}

	if (strcmp(method, REQUEST_GET) == 0) {
		if (entity->hasGetMethod()) {
			entity->executeGet(params,
					JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(response,
							entity->getGroup(), entity->getName()));
			return;
		}
	} else if (strcmp(method, REQUEST_POST) == 0) {
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
	Serial.println(FPSTR("----------------------------------"));
	Serial.println(FPSTR("Init entityManager"));
	FileUtils::init();

	//buf = new DynamicJsonBuffer();

	for (Entity* entity : entities) {
		entity->init();
	}

	loadEntitiesFromFile();
	saveEntitiesToFile();

	Serial.println(FPSTR("Init entityManager completed"));
	Serial.println(FPSTR("==============================="));
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

void EntityManager::groupNameToParam(char* group, char* name,
		EntityJsonRequestResponse* json) {
	json->addRequestParam(GROUP, group);
	json->addRequestParam(NAME, name);
}

EntityJsonRequestResponse* EntityManager::createEntityJsonRequestResponse() {
	ObjectUtils::printHeap();
	return new EntityJsonRequestResponse();
}

void EntityManager::deleteEntityJsonRequestResponse(
		EntityJsonRequestResponse* json) {
	delete json;
	ObjectUtils::printHeap();
}

void EntityManager::dispatchAllChangedEntities() {
	if(entitiesChanged){
		for(Entity* entity: entities){

			if(entity->isChanged()){
				entity->dispatchChangeEvent(false);
				entity->setChanged(false);
			}
		}

		if(onEntityChanged!=nullptr){
			onEntityChanged();
		}

		Serial.println(FPSTR("entities onCHange processed"));
	}
	entitiesChanged = false;
}

void EntityManager::loop() {
	dispatchAllChangedEntities();
}

bool EntityManager::executeHttpMethodOnAll(JsonObject& params,
		JsonObject& response, const char* method) {

	bool changed = false;

	for (Entity* entity : this->entities) {
		executeHttpMethodOnEntity(params, response, method, entity);
		changed = changed || entity->isChanged();
	}

	this->entitiesChanged = (this->entitiesChanged || changed);
	return changed;
}

bool EntityManager::executeHttpMethodOnGroup(JsonObject& params,
		JsonObject& response, const char* method) {

	bool changed = false;

	int totalCount = 0;

	const char* group = params[GROUP].as<char*>();

	for (Entity* entity : this->entities) {
		/*Serial.print(FPSTR("check only ="));
		Serial.print(entity->getGroup());
*/
		if(strcmp(entity->getGroup(),group)==0){
			executeHttpMethodOnEntity(params, response, method, entity);

			changed = changed || entity->isChanged();

			totalCount++;
		}
	}

	if (totalCount == 0) {
		JsonObjectUtil::setField(response, MESSAGE, NOT_FOUND);
		return false;;
	}

	if (changed) {
		saveEntitiesToFile();
	}

	this->entitiesChanged = (this->entitiesChanged || changed);

	return changed;
}

bool EntityManager::executeHttpMethodOnEntityOnly(JsonObject& params,
		JsonObject& response, const char* method) {

	bool changed = false;
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

	changed = entity->isChanged();

	return changed;
}

bool EntityManager::hasNoGroupNoName(JsonObject& params) {
	return !JsonObjectUtil::hasFieldWithAnyType(params, GROUP)
				&& !JsonObjectUtil::hasFieldWithAnyType(params, NAME);
}

bool EntityManager::hasGroupNoName(JsonObject& params) {
	return JsonObjectUtil::hasFieldWithAnyType(params, GROUP)
					&& !JsonObjectUtil::hasFieldWithAnyType(params, NAME);
}

bool EntityManager::hasGroupName(JsonObject& params) {
	return JsonObjectUtil::hasFieldWithAnyType(params, GROUP)
					&& JsonObjectUtil::hasFieldWithAnyType(params, NAME);
}

bool EntityManager::hasAllGroupNoName(JsonObject& params) {
	return JsonObjectUtil::hasFieldAndValueEquals(params, GROUP, GROUP_ALL);
}

void EntityManager::persist(
		std::function<void(JsonObject& json, Entity* entity)> onEntityFunction,
		std::function<void(JsonObject& json)> postPersistFunction) {

	Serial.println(FPSTR("-------------"));
	JsonObject& jsonEmpty = buf.parse("{}").asObject();

	JsonObject& jsonTemp = FileUtils::loadJsonFromFile(FILE_PATH, &buf, jsonEmpty);


	Serial.println(FPSTR("Loaded entities"));
	JsonObjectUtil::print(jsonTemp);
	Serial.println(FPSTR("-------------"));

	for (Entity* entity : entities) {
		JsonObject& json = JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(
				jsonTemp, entity->getGroup(), entity->getName());
		entity->print();
		JsonObjectUtil::print(json);
		onEntityFunction(json, entity);
	}

	if (postPersistFunction != nullptr) {
		postPersistFunction(jsonTemp);
		Serial.println(FPSTR("Execute post persist function"));
	}

	//buf->clear();

	Serial.println(FPSTR("------persisted-------"));
}

void EntityManager::get(EntityJsonRequestResponse* reqResp) {
	executeHttpMethod(reqResp, REQUEST_GET);
}

void EntityManager::post(EntityJsonRequestResponse* reqResp) {
	executeHttpMethod(reqResp, REQUEST_POST);
}

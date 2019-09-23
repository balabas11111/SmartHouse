/*
 * Entity.cpp
 *
 *  Created on: Aug 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <Entity.h>
#include <ArduinoJson.h>

Entity::Entity(const char* group, const char* name, char* descr,
		std::function<void(void)> selfEventProcessFunction,
bool hasGet, bool hasPost, bool dispatcher,
bool canLoad, bool canSave) {
	this->changed = false;
	this->id = -1;
	this->group = group;
	this->name = name;
	this->descr = descr;

	this->hasGet = hasGet;
	this->hasPost = hasPost;
	this->dispatcher = dispatcher;

	this->canLoad = canLoad;
	this->canSave = canSave;

	this->selfEventProcessFunction = selfEventProcessFunction;
}

void Entity::preInitialize(int id, std::function<void(int)> eventProcessFunction) {
	this->id = id;
	if (this->canDispatchChangeEvent()) {
		this->eventProcessFunction = eventProcessFunction;
	} else {
		this->eventProcessFunction = nullptr;
	}
}

bool Entity::isChanged() {
	return this->changed;
}

void Entity::setChanged(bool changed) {
	this->changed = changed;
}

bool Entity::isSaveRequired(){
	return this->saveRequired;
}

void Entity::setSaveRequired(bool saveRequired){
	this->saveRequired = saveRequired;
}

const char* Entity::getName() {
	return name;
}

char* Entity::getDescr() {
	return descr;
}

const char* Entity::getGroup() {
	return group;
}

bool Entity::hasGetMethod() {
	return this->hasGet;
}

bool Entity::hasPostMethod() {
	return this->hasPost;
}

bool Entity::canDispatchChangeEvent() {
	return this->dispatcher;
}

bool Entity::canLoadState() {
	return this->canLoad;
}

bool Entity::canSaveState() {
	return this->canSave;
}

bool Entity::isTarget(const char* group, const char* name) {
	return (strcmp(group, this->group) == 0) && (strcmp(name, this->name) == 0);
}

void Entity::print() {
	Serial.print(FPSTR("Entity id="));
	Serial.print(id);
	Serial.print(FPSTR(" group="));
	Serial.print(group);
	Serial.print(FPSTR(" name="));
	Serial.print(name);
	Serial.print(FPSTR(" descr="));
	Serial.println(descr);
}

void Entity::dispatchChangeEventIfChanged() {
	if(this->changed){
		if(selfEventProcessFunction!=nullptr){
			selfEventProcessFunction();
		}
		if (eventProcessFunction != nullptr) {
			eventProcessFunction(id);
		}
	}
}

void Entity::markEntityAsChangedIfTrue(bool value){
	if(value){
		this->setChanged(true);
	}
}

void Entity::markEntityAsSaveRequiredIfTrue(bool value){
	if(value){
		this->saveRequired = saveRequired;
	}
}

void Entity::executeGet(JsonObject& params, JsonObject& response) {
	setJsonField(response, DESCR, this->descr);

	doGet(params, response);
}

void Entity::executePost(JsonObject& params, JsonObject& response) {
	/*if (isKeyExistsInJsonAndNotEqValue(params, this->descrField, this->descr)) {
		this->descr = strdup(getJsonField<const char*>(params, this->descrField));
		setChanged(true);
	}*/
	bool descrChanged = getKeyValueIfExistsAndNotEquals(params, DESCR, &this->descr);
	markEntityAsChangedIfTrue(descrChanged);
	markEntityAsSaveRequiredIfTrue(descrChanged);

	doPost(params, response);

	executeGet(params, response);
}

void Entity::setJsonToEntity(JsonObject& jsonFromFile) {
	/*if (isKeyExistsInJsonAndNotEqValue(jsonFromFile, this->descrField, this->descr)) {
		this->descr = strdup(getJsonField<const char*>(jsonFromFile, this->descrField));
	}*/
	getKeyValueIfExistsAndNotEquals(jsonFromFile, DESCR, &this->descr);
	doLoad(jsonFromFile);
}

void Entity::getJsonToSave(JsonObject& jsonToFile) {
	setKeyValueIfNotExistOrNotEqual(jsonToFile, DESCR, this->descr);

	doSave(jsonToFile);
}

bool Entity::getKeyValueIfExistsAndNotEquals(JsonObject& json, const char* key,	char** val) {
	if (isKeyExistsInJsonAndNotEqValue(json, key, *val)) {
		*val = strdup(getJsonField<const char*>(json, key));
		return true;
	}
	return false;
}

bool Entity::getKeyValueIfExistsAndNotEquals(JsonObject& json, const char* key,	int* val) {
	if (isKeyExistsInJsonAndNotEqValue(json, key, *val)) {
		*val = getJsonField<int>(json, key);
		return true;
	}
	return false;
}

bool Entity::getKeyValueIfExistsAndNotEquals(JsonObject& json, const char* key,
		bool* val) {
	if (isKeyExistsInJsonAndNotEqValue(json, key, *val)) {
			*val = getJsonField<int>(json, key);
			return true;
		}
		return false;
}

bool Entity::getKeyValueIfExistsAndNotEquals(JsonObject& json, const char* key,
		uint16_t* val) {
	if (isKeyExistsInJsonAndNotEqValue(json, key, *val)) {
			*val = getJsonField<int>(json, key);
			return true;
		}
		return false;
}

bool Entity::getKeyValueIfExistsAndNotEquals(JsonObject& json, const char* key,
		uint8_t* val) {
	if (isKeyExistsInJsonAndNotEqValue(json, key, *val)) {
		*val = getJsonField<int>(json, key);
		return true;
	}
	return false;
}

bool Entity::getKeyValueIfExistsAndNotEquals(JsonObject& json, const char* key, IPAddress* val){
	bool chg = false;

	if(!json.containsKey(key)){
		chg = false;
	} else{
		chg = !CompareUtils::compareValues(json.get<JsonVariant>(key), val);
	}

	if(chg){
		const char* ipChar = strdup(getJsonField<const char*>(json, key));
		IPAddress addr = (new IPAddress())->fromString(ipChar);
		*val = addr;
		return true;
	}
	return false;
}

bool Entity::setKeyValueIfNotExistOrNotEqual(JsonObject& json, const char* key,	JsonVariant val) {
	if(JsonObjectUtil::getObjectFieldNotExistsOrNotEquals(json, key, val)){
		JsonObjectUtil::setField(json, key, val);
		return true;
	}

	return false;
}

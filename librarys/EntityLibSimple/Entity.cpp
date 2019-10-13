/*
 * Entity.cpp
 *
 *  Created on: Aug 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <Entity.h>
#include <ArduinoJson.h>

Entity::Entity(const char* group, const char* name, char* descr,
		std::function<void(void)> onSetChangedEventFunction,
		bool applicationDispatcher,
bool hasGet, bool hasPost,
bool canLoad, bool canSave) {
	this->changed = false;
	this->forceChanged = false;
	this->saveRequired = false;
	this->id = -1;
	this->group = group;
	this->name = name;
	this->descr = descr;

	this->hasGet = hasGet;
	this->hasPost = hasPost;
	this->applicationDispatcher = applicationDispatcher;

	this->canLoad = canLoad;
	this->canSave = canSave;

	this->onSetChangedEventFunction = onSetChangedEventFunction;
}

void Entity::preInitialize(int id, std::function<void(void)> eventProcessFunction,
		std::function<void(const char*, const char*, const char*, JsonVariant)> putToBuffer) {
	this->id = id;
	this->eventProcessFunction = eventProcessFunction;
	this->putToChangedBuffer = putToBuffer;
}

void Entity::putToBuffer(const char* key, JsonVariant value) {
	if(putToChangedBuffer!=nullptr){
		putToChangedBuffer(this->getGroup(), this->getName(), key, value);
	}
}

void Entity::setChanged(bool changed, bool forceChanged) {
	this->changed = changed;
	this->forceChanged = forceChanged;
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

bool Entity::isApplicationDispatcher() {
	return this->applicationDispatcher;
}

void Entity::setApplicationDispatcher(bool applicationDispatcher){
	this->applicationDispatcher = applicationDispatcher;
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

void Entity::dispatchEventProcessFunctionIfChanged() {
	if(isMarkedAsChanged() && eventProcessFunction != nullptr){
		eventProcessFunction();
	}
}

void Entity::markEntityAsChangedIfTrue(bool value, bool forceChanged){
	if(value || forceChanged){
		this->setChanged(value, forceChanged);

		if(onSetChangedEventFunction!=nullptr){
			onSetChangedEventFunction();
		}

		dispatchEventProcessFunctionIfChanged();
	}
}

void Entity::markEntityAsUnChanged(){
	this->setChanged(false, false);
}

bool Entity::isMarkedAsChanged(){
	return this->isApplicationDispatcher()?this->changed:this->forceChanged;
}

void Entity::markEntityAsSaveRequiredIfTrue(bool value){
	if(value){
		this->saveRequired = saveRequired;
	}
}

void Entity::executeGet(JsonObject& params, JsonObject& response) {
	setJsonField(response, ENTITY_FIELD_ID, this->id);
	setJsonField(response, ENTITY_FIELD_DESCRIPTION, this->descr);
	doGet(params, response);
}

void Entity::executePost(JsonObject& params, JsonObject& response) {
	//Serial.println(FPSTR("Execute post"));
	bool chg = getKeyValueIfExistsAndNotEquals(params, ENTITY_FIELD_DESCRIPTION, &this->descr);

	if(chg){
		putToBuffer(ENTITY_FIELD_DESCRIPTION, &this->descr);
	}
	/*Serial.print(FPSTR("chg"));
	Serial.print(chg);
	*/
	markEntityAsChangedIfTrue(chg, chg);
	markEntityAsSaveRequiredIfTrue(chg);
	/*Serial.println(FPSTR("Execute post"));
*/
	doPost(params, response);
	executeGet(params, response);
}

void Entity::setJsonToEntity(JsonObject& jsonFromFile) {
	/*if (isKeyExistsInJsonAndNotEqValue(jsonFromFile, this->descrField, this->descr)) {
		this->descr = strdup(getJsonField<const char*>(jsonFromFile, this->descrField));
	}*/
	getKeyValueIfExistsAndNotEquals(jsonFromFile, ENTITY_FIELD_DESCRIPTION, &this->descr);
	doLoad(jsonFromFile);
}

void Entity::getJsonToSave(JsonObject& jsonToFile) {
	setKeyValueIfNotExistOrNotEqual(jsonToFile, ENTITY_FIELD_DESCRIPTION, this->descr);

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

/*
 * Entity.cpp
 *
 *  Created on: Aug 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <Entity.h>

Entity::Entity(const char* group, const char* name, char* descr, const char* descrField,
bool hasGet, bool hasPost, bool dispatcher,
bool canLoad, bool canSave) {
	this->changed = false;
	this->id = -1;
	this->group = group;
	this->name = name;
	this->descr = descr;

	this->descrField = descrField;

	this->hasGet = hasGet;
	this->hasPost = hasPost;
	this->dispatcher = dispatcher;

	this->canLoad = canLoad;
	this->canSave = canSave;
}

void Entity::initialize(int id, std::function<void(int)> eventProcessFunction) {
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

void Entity::dispatchChangeEvent(bool clause) {
	if (clause && this->dispatcher && id > -1
			&& eventProcessFunction != nullptr) {
		eventProcessFunction(id);
	}
}

void Entity::executeGet(JsonObject& params, JsonObject& response) {
	setJsonField(response, this->descrField, this->descr);

	doGet(params, response);
}

void Entity::executePost(JsonObject& params, JsonObject& response) {
	if (isKeyExistsInJsonAndNotEqValue(params, this->descrField, this->descr)) {
		this->descr = strdup(getJsonField<const char*>(params, this->descrField));
		setChanged(true);
	}

	doPost(params, response);

	executeGet(params, response);
}

void Entity::executeLoad(JsonObject& jsonFromFile) {
	if (isKeyExistsInJsonAndNotEqValue(jsonFromFile, this->descrField, this->descr)) {
		this->descr = strdup(getJsonField<const char*>(jsonFromFile, this->descrField));
	}

	doLoad(jsonFromFile);
}

void Entity::executeSave(JsonObject& jsonToFile) {
	if(isKeyNotExistsInJsonOrNotEqValue(jsonToFile,this->descrField,this->descr)){
		setJsonField(jsonToFile, this->descrField, this->descr);
	}

	doSave(jsonToFile);
}

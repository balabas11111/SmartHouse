/*
 * Entity.cpp
 *
 *  Created on: Aug 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <Entity.h>

Entity::Entity(const char* group, const char* name, char* descr,
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
}

void Entity::initialize(int id, std::function<void(int)> eventProcessFunction) {
	this->id = id;
	if (this->canDispatchChangeEvent()) {
		this->eventProcessFunction = eventProcessFunction;
	} else {
		this->eventProcessFunction = nullptr;
	}
}

bool Entity::isChanged() const {
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
/*
void Entity::dispatchChangeEvent() {
	dispatchChangeEvent(this->changed);
	this->changed = false;
}

void Entity::executeGet(JsonObject& params, JsonObject& response) {
	UNUSED(params);
	JsonObjectUtil::setField(response, MESSAGE, NOT_IMPLEMENTED);
}

void Entity::executePost(JsonObject& params, JsonObject& response) {
	if (!hasPostMethod()) {
		JsonObjectUtil::setField(response, MESSAGE, NOT_ALLOWED);
	} else {
		executeGet(params, response);
	}
}
*/
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
	if (clause && this->dispatcher && id > -1 && eventProcessFunction != nullptr) {
		eventProcessFunction(id);
	}
}

/*
 * Entity.cpp
 *
 *  Created on: Aug 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <Entity.h>

Entity::Entity(const char* group, const char* name, const char* descr, const bool hasGet, const bool hasPost, const bool dispatcher = false) {
	this->id = -1;
	this->group = group;
	this->name = name;
	this->descr = descr;
	this->hasGet = hasGet;
	this->hasPost = hasPost;
}

void Entity::setId(int id) {
	this->id = id;
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

const bool Entity::hasGetMethod() {
	return this->hasGet;
}

const bool Entity::hasPostMethod() {
	return this->hasPost;
}

bool Entity::isTarget(const char* group, const char* name) {
	return (strcmp(group, this->group) == 0) && (strcmp(name, this->name) == 0);
}

void Entity::executeGet(JsonObject& params, JsonObject& response) {
	JsonObjectUtil::setField(response, MESSAGE, NOT_IMPLEMENTED);
}

void Entity::executePost(JsonObject& params, JsonObject& response) {
	if(!hasPostMethod()){
		JsonObjectUtil::setField(response, MESSAGE, NOT_ALLOWED);
	} else {
		executeGet(params, response);
	}
}

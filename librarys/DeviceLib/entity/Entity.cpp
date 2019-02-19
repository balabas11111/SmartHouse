/*
 * Entity.cpp
 *
 *  Created on: Feb 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <entity/Entity.h>

Entity::Entity(String name,String& dao, String& model) {
	this->name=name;
	this->dao=dao;
	this->model=model;
}

String Entity::getDao() {
	return dao;
}

String Entity::getModel() {
	return model;
}

int Entity::init(JsonObject& data, JsonObject& model) {
	return 0;
}

/*
 * AbstractItemDAO.cpp
 *
 *  Created on: 17 февр. 2019 г.
 *      Author: Vitaliy
 */

#include <entity/abstractItem/AbstractItemDAO.h>

AbstractItemDAO::AbstractItemDAO(String name,
		AbstractItem* daoItems[]) {
}

int AbstractItemDAO::init(int id) {
	this->id=id;
	return id;
}

AbstractItemList<AbstractItem>* AbstractItemDAO::getItems() {
	return this->items;
}

int AbstractItemDAO::startModel(JsonObject* model, JsonObject* descriptor) {
	this->model=model;
	this->descriptor=descriptor;

	return 1;
}

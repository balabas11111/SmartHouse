/*
 * AbstractItemService.cpp
 *
 *  Created on: 17 февр. 2019 г.
 *      Author: Vitaliy
 */

#include <entity/abstractItem/AbstractItemService.h>

AbstractItemService::AbstractItemService(AbstractItemDAO* daos[],
		std::function<void(JsonObject&,int)> sendResult,
		std::function<void(JsonObject&,int)> sendCache) {

	this->daos = AbstractItemDAOList(daos);
	this->sendResult=sendResult;
	this->sendCache=sendCache;
}

int AbstractItemService::processRequest(String str) {
}

boolean AbstractItemService::isCached(JsonObject& obj) {
}

void AbstractItemService::returnCachedUrl(JsonObject& obj) {
}

int AbstractItemService::initialize() {
	Serial.println(FPSTR("INIT AbstractItemService"));
	Serial.print(FPSTR("DAOS "));
	Serial.println(getDaos().getSize());

	Serial.println("Init model");
	jsonBuffer = DynamicJsonBuffer();
	model = jsonBuffer->parse(FPSTR(DEFAULT_MODEL)).asObject().get<JsonObject>("model");

	model->printTo(Serial);
	Serial.println(FPSTR("Model done"));
}

int AbstractItemService::startModel(JsonObject* model,JsonObject descriptor) {

}

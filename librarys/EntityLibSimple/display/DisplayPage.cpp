/*
 * DisplayPage.cpp
 *
 *  Created on: Oct 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <display/DisplayPage.h>

DisplayPage::DisplayPage(Entity* entity, const char* fields[], int fieldsSize,
		const char* header) {
	this->entity = entity;
	this->fields = fields;
	this->fieldsSize = fieldsSize;
	this->header = header;
	this->fieldsDescr = nullptr;
	this->fieldsMeasure = nullptr;
}

DisplayPage::DisplayPage(Entity* entity, const char* fields[],
		const char* fieldsDescr[], const char* fieldsMeasure[], int fieldsSize,
		const char* header) {
	this->entity = entity;
		this->fields = fields;
		this->fieldsSize = fieldsSize;
		this->header = header;
		this->fieldsDescr = fieldsDescr;
		this->fieldsMeasure = fieldsMeasure;
}

bool DisplayPage::init(PageToDisplayAdapter* adapter) {
	if(this->adapter == nullptr) {
		setAndInitDisplayAdapter(adapter);
	}

	this->initialized = this->entity != nullptr && this->fieldsSize >0 && adapter->isInitialized();
	return isInitialized();
}

JsonObject& DisplayPage::putDataToJson(JsonObject& parent, const char* key,
		const char* name, const char* measure) {

	JsonObject& dataEntityToDisplay = JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(parent, key);
	dataEntityToDisplay[COL_KEY] = key;
	dataEntityToDisplay[COL_NAME] = name;
	if(measure!=nullptr) {
		dataEntityToDisplay[COL_MEASURE] = measure;
	}

	return dataEntityToDisplay;
}

void DisplayPage::render() {
	if(adapter == nullptr){
		return;
	}

	unsigned long start = millis();

	EntityJsonRequestResponse* resp = new EntityJsonRequestResponse();
	JsonObject& dataToDisplay = resp->getRoot().createNestedObject(DISPLAY_DATA);

	if(!entity->hasGetMethod()) {

	} else {
		entity->doGet(resp->getRequest(), resp->getResponse());

		JsonObject& dataEntity = resp->getResponse();

		//JsonObjectUtil::print(dataEntity);
		//put needed keys
		if(dataEntity.size()>0){
			for(int i = 0; i < this->fieldsSize; i++){
				const char* key = this->fields[i];

				if(dataEntity.containsKey(key)){
					JsonObject& dataEntityToDisplay = putDataToJson(dataToDisplay, key, this->fieldsDescr[i], this->fieldsMeasure[i]);
					dataEntityToDisplay[COL_VALUE] = dataEntity[key];
				}
			}
		}

		if (dataToDisplay.size()<1){
			putDataToJson(dataToDisplay, ERROR_KEY, NO_DATA);
		}

	}

	adapter->renderPage((this->header != nullptr)?this->header:entity->getDescr(), dataToDisplay);

	delete resp;

	DeviceUtils::printlnTimeHeap(start);
}

bool DisplayPage::renderKey(const char* key) {
	for(int i = 0; i < this->fieldsSize; i++){
		if( strcmp(this->fields[i], key) == 0){
			return true;
		}
	}

	return false;
}

bool DisplayPage::isInitialized() {
	return this->initialized;
}

void DisplayPage::setAndInitDisplayAdapter(PageToDisplayAdapter* adapter) {
	this->adapter = adapter;
	this->adapter->init();
}

PageToDisplayAdapter* DisplayPage::getAdapter() {
	return this->adapter;
}

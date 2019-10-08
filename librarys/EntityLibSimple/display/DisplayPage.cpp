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
}

bool DisplayPage::init() {
	this->initDone = this->entity != nullptr && this->fieldsSize >0;
	return isInitDone();
}

void DisplayPage::render(PageToDisplayAdapter* adapter) {
	if(adapter == nullptr){
		return;
	}

	unsigned long start = millis();

	EntityJsonRequestResponse* resp = new EntityJsonRequestResponse();

	entity->doGet(resp->getRequest(), resp->getResponse());

	JsonObject& dataEntity = resp->getResponse();


		JsonObject& dataToDisplay = resp->getRoot().createNestedObject(DISPLAY_DATA);
		//put needed keys
		if(dataEntity.size()>0){
			for(int i = 0; i < this->fieldsSize; i++){
				const char* key = this->fields[i];

				if(dataEntity.containsKey(key)){
					dataToDisplay[key] = dataEntity[key];
				}
			}
		}

		if (dataToDisplay.size()<1){
			dataToDisplay[ERROR_KEY] = NO_DATA;
		}

		adapter->renderPage((this->header != nullptr)?this->header:entity->getName(), dataToDisplay);

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

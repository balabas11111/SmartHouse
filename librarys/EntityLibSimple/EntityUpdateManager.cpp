/*
 * EntityUpdateManager.cpp
 *
 *  Created on: Aug 27, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <EntityUpdateManager.h>

EntityUpdateManager::EntityUpdateManager(EntityUpdate* entities[], int count) {
	for (int i=0;i<count;i++) {
		EntityUpdate* entity = entities[i];
		if (entity->getInterval() == SETTINGS_BOX_INTERVAL) {
			this->entities.push_back(entity);
		}

		if (entity->isAutoupdate()) {
		}
	}

}

int EntityUpdateManager::init(int interval) {
	if(interval<1){
		Serial.println(FPSTR("No refresh interval specified"));
		return 0;
	}
	int totalCount = 0;

	Serial.println(FPSTR("Init entity Update manager"));
	this->entities = entities;

	for (EntityUpdate* entity:this->entities) {
		if (entity->getInterval() == SETTINGS_BOX_INTERVAL) {
			Serial.print(FPSTR("init entity"));
			Serial.println(entity->getInterval());
			entity->init(interval);

			totalCount++;
		}

		if (entity->isAutoupdate()) {
		}
	}

	return totalCount;
}

void EntityUpdateManager::loop() {
	for (EntityUpdate* entity : this->entities) {
			entity->update(true);
	}
}

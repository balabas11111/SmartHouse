/*
 * EntityUpdateManager.cpp
 *
 *  Created on: Aug 27, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <EntityUpdateManager.h>
/*
 EntityUpdateManager::EntityUpdateManager() {

 }
 */
int EntityUpdateManager::init(std::list<EntityUpdate*>& entities,
		int interval) {
	int count = 0;

	Serial.println(FPSTR("Init entity Update manager"));
	this->entities = entities;

	for (EntityUpdate* entity : entities) {
		if (entity->getInterval() == SETTINGS_BOX_INTERVAL) {
			Serial.print(FPSTR("init entity"));
			Serial.println(entity->getInterval());
			entity->init(interval);

			count++;
		}

		if (entity->isAutoupdate()) {
		}
	}

	return count;
}

void EntityUpdateManager::update() {
	for (EntityUpdate* entity : this->entities) {
			entity->update(true);
	}
}

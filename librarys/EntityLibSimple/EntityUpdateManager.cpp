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
		this->entities.push_back(entity);
	}
}

void EntityUpdateManager::registerEntity(EntityUpdate* entity) {
}

int EntityUpdateManager::init(int interval) {
	if(interval<30){
		interval = 31;
	}

	Serial.println(FPSTR("-----------------------------------"));
	Serial.print(FPSTR("init  EntityUpdateManager interval ="));
	Serial.println(interval);

	if(interval<1){
		Serial.println(FPSTR("No refresh interval specified"));
		return 0;
	}
	int totalCount = 0;

	this->entities = entities;

	int intervalCounts = 0;
	uint32_t intervals[this->entities.size()];

	unsigned long time = millis();

	for (EntityUpdate* entity:this->entities) {
		if (entity->getInterval() == SETTINGS_BOX_INTERVAL) {
			entity->init(interval, time);
			totalCount++;
		}

		if (entity->isAutoupdate()) {
			uint32_t curIntervalMs = entity->getInterval();

			bool found = false;

			for(int i = 0;i<intervalCounts; i++){
				if(intervals[i]==curIntervalMs){
					found = true;
					break;
				}
			}

			if(!found){
				intervals[intervalCounts] = curIntervalMs;
				intervalCounts++;
				/*Serial.print(FPSTR("Interval added "));
				Serial.println(curIntervalMs);
				*/
			}
		}
	}

	if(intervalCounts>0){
		/*
		for(int i = 0;i<intervalCounts; i++){
			uint32_t interval = intervals[i];
			Ticker* ticker = new Ticker();
			ticker->attach_msWithMaParam(intervals[i], [this](uint32_t intervalIn){executeTickUpdate(intervalIn);});
			ticker->attach_ms(interval,[this](void){loop{};});

		}
		*/
	}

	Serial.print(FPSTR("EntityUpdateManager totalCount="));
	Serial.println(totalCount);
	Serial.println(FPSTR("=================================="));

	return totalCount;
}

void EntityUpdateManager::updateEntities(bool force) {
	bool updated = false;
	unsigned long time = millis();

	for (EntityUpdate* entity : this->entities) {
			entity->loop();
			updated = entity->update(time, force) || updated;
	}

	if(updated){
		/*Serial.print(FPSTR("Entities updated force="));
		Serial.println(force);
		*/
	}
/*
	Serial.println(FPSTR("----------------------------------"));
	Serial.println(FPSTR("EntityUpdateManager loop DONE"));
	Serial.println(FPSTR("=================================="));
	*/
}

void EntityUpdateManager::loop(bool force) {
	updateEntities(force);
}

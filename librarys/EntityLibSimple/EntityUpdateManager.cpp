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

	int intervalCounts = 0;
	uint32_t intervals[this->entities.size()];

	for (EntityUpdate* entity:this->entities) {
		if (entity->getInterval() == SETTINGS_BOX_INTERVAL) {
			Serial.print(FPSTR("init entity"));
			Serial.println(entity->getInterval());
			entity->init(interval);

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
				Serial.print(FPSTR("Interval added "));
				Serial.println(curIntervalMs);
			}
		}
	}

	if(intervalCounts>0){
		for(int i = 0;i<intervalCounts; i++){
			uint32_t interval = intervals[i];
			//Ticker* ticker = new Ticker();
			//ticker->attach_msWithMaParam(intervals[i], [this](uint32_t intervalIn){executeTickUpdate(intervalIn);});
			//ticker->attach_ms(interval,[this](void){loop{};});

		}
	}

	return totalCount;
}

void EntityUpdateManager::loop() {
	for (EntityUpdate* entity : this->entities) {
			entity->update(true);
	}
}

void EntityUpdateManager::executeTickUpdate(uint32_t interval) {
}

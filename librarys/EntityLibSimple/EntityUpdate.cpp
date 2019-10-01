/*
 * EntityUpdate.cpp
 *
 *  Created on: Aug 27, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <EntityUpdate.h>

void EntityUpdate::init(long interv){
	this->interval = interv * 1000;
	Serial.print(FPSTR("EntityUpdate interval="));
	Serial.println(this->interval);
}

bool EntityUpdate::update(bool force){
	if(this->interval<1){
		//Serial.println(FPSTR("No update    is expected interval<1"));
		return false;
	}

	if((!force && !shouldUpdate())){
		//Serial.println(FPSTR("No update    is expected"));
		return false;
	}

	doUpdate();
	this->lastUpdate = millis();
	return true;
}


long EntityUpdate::getInterval(){
	return this->interval;
}

bool EntityUpdate::isAutoupdate(){
	return interval>0;
}

bool EntityUpdate::shouldUpdate(){
	return (lastUpdate + interval < millis());
}



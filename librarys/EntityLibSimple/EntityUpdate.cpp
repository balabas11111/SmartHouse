/*
 * EntityUpdate.cpp
 *
 *  Created on: Aug 27, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <EntityUpdate.h>

void EntityUpdate::init(long interv, unsigned long initTime){
	this->interval = interv * 1000;
	this->lastUpdate = initTime;
	Serial.print(FPSTR("EntityUpdate interval="));
	Serial.println(this->interval);
}

bool EntityUpdate::update(unsigned long time,bool force){
	if(this->interval<1){
		//Serial.println(FPSTR("No update    is expected interval<1"));
		return false;
	}

	if((!force && !shouldUpdate(time))){
		//Serial.println(FPSTR("No update    is expected"));
		return false;
	}

	doUpdate();
	this->lastUpdate = time;
	return true;
}


long EntityUpdate::getInterval(){
	return this->interval;
}

bool EntityUpdate::isAutoupdate(){
	return interval>0;
}

bool EntityUpdate::shouldUpdate(unsigned long time){
	return (lastUpdate + interval < time);
}



/*
 * EntityUpdate.cpp
 *
 *  Created on: Aug 27, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <EntityUpdate.h>

void EntityUpdate::init(long interv){
	this->interval = interv * 1000;
}

void EntityUpdate::update(bool withCheck){
	if(this->interval<1){
		//Serial.println(FPSTR("No update    is expected interval<1"));
		return;
	}

	if((withCheck && !shouldUpdate())){
		//Serial.println(FPSTR("No update    is expected"));
		return;
	}

	doUpdate();
	lastUpdate = millis();
}


long EntityUpdate::getInterval(){
	return interval;
}

bool EntityUpdate::isAutoupdate(){
	return interval>0;
}

bool EntityUpdate::shouldUpdate(){
	return (lastUpdate + interval < millis());
}



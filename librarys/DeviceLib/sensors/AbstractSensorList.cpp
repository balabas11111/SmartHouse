/*
 * AbstractSensorList.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <sensors/AbstractSensorList.h>

AbstractSensorList::AbstractSensorList() {
}

AbstractSensorList::AbstractSensorList(uint8_t initSize) {
	Serial.print(FPSTR("Init AbstractSensorList initSize="));
	Serial.print(initSize);

	if(initSize!=0){
		AbstractSensor* sensValues[] = new AbstractSensor[initSize];
		constructList(sensValues, initSize);
	}

	Serial.println(FPSTR("...done"));
}

AbstractSensor* AbstractSensorList::getItemById(int id) {
	if(getSize()==0){
		return NULL;
	}
	for(uint8_t i=0;i<getSize();i++){
		if(getItem(i)->getId()==id){
			return getItem(i);
		}
	}
	return NULL;
}

AbstractSensor* AbstractSensorList::getItemByName(String name) {
	if(getSize()==0){
		return NULL;
	}
	for(uint8_t i=0;i<getSize();i++){
		if(getItem(i)->getName()==name){
			return getItem(i);
		}
	}
	return NULL;
}

AbstractSensor* AbstractSensorList::getItemByHash(String hash) {
	if(getSize()==0){
		return NULL;
	}
	for(uint8_t i=0;i<getSize();i++){
		if(getItem(i)->getHash()==hash){
			return getItem(i);
		}
	}
	return NULL;
}

boolean AbstractSensorList::getChanged() {
	if(getSize()==0){
		return false;
	}
	for(uint8_t i=0;i<getSize();i++){
		if(getItem(i)->getChanged()){
			return true;
		}
	}
	return false;
}

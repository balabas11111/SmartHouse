/*
 * AbstractSensorValueList.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <sensors/AbstractSensorValueList.h>

AbstractSensorValueList::AbstractSensorValueList() {
}

AbstractSensorValueList::AbstractSensorValueList(uint8_t initSize) {
	Serial.print(FPSTR("Init AbstractSensorValueList initSize="));
	Serial.print(initSize);

	if(initSize!=0){
		AbstractSensorValue* sensValues[] = new AbstractSensorValue[initSize];
		constructList(sensValues, initSize);
	}

	Serial.println(FPSTR("...done"));
}

AbstractSensorValue* AbstractSensorValueList::getItemById(int id) {
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

AbstractSensorValue* AbstractSensorValueList::getItemByName(String name) {
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

AbstractSensorValue* AbstractSensorValueList::getItemByHash(String hash) {
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

boolean AbstractSensorValueList::getChanged(){
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

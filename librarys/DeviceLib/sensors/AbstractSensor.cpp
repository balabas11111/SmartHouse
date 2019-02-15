/*
 * AbstractSensor.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <sensors/AbstractSensor.h>
#include <ArduinoJson.h>

AbstractSensor::AbstractSensor(uint8_t id, String name, String type,
		String size, String descr, uint8_t childCount, float val) {

	Serial.print(FPSTR("Init sensor name="));
	Serial.print(name);
	Serial.print(FPSTR(" childCount="));
	Serial.print(childCount);

	curStatus=SensorStatus_init;

	this->header=new AbstractSensorValue(id,name,type,size,descr,val);
	this->childs=new AbstractSensorValueList(childCount);

	initChildren(childCount);

	curStatus=SensorStatus_initComplete;
}

uint8_t AbstractSensor::getId() {
	return getHeader()->getId();
}

String AbstractSensor::getName() {
	return getHeader()->getName();
}

boolean AbstractSensor::getChanged(){
	return ChangeDispatchable::getChanged() && getChilds()->getChanged();
}

AbstractSensorValue* AbstractSensor::getHeader() {
	return this->header;
}

AbstractSensorValue* AbstractSensor::getChild(int index) {
	if(getChilds()==NULL || index<0 || getChilds()->getSize()==0){
		return NULL;
	}
	return getChilds()->getItem(index);
}

AbstractSensorValueList* AbstractSensor::getChilds() {
	return this->childs;
}

uint8_t AbstractSensor::getStatus(){
	return this->curStatus;
}

String AbstractSensor::getHashableString() {
	return getName();
}

void AbstractSensor::initChildren(int childCount) {
}

int AbstractSensor::set(JsonObject& item) {
	return 0;
}

int AbstractSensor::get(JsonObject& item) {
	return 0;
}

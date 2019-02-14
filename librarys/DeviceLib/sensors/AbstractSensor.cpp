/*
 * AbstractSensor.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <sensors/AbstractSensor.h>

AbstractSensor::AbstractSensor(uint8_t id, String name, String type,
		String size, String descr, uint8_t childCount, float val) {

	Serial.print(FPSTR("Init sensor name="));
	Serial.print(name);
	Serial.print(FPSTR(" childCount="));
	Serial.print(childCount);

	this->status=SensorStatus_init;

	this->header=new AbstractSensorValue(id,name,type,size,descr,val);
	this->childs=new AbstractSensorValueList(childCount);

	initChildren(childCount);

	this->status=SensorStatus_initComplete;
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

uint8_t AbstractSensor::getStatus() const {
	return this->status;
}

String AbstractSensor::getHashableString() {
	return getName();
}

int AbstractSensor::set(JsonObject& item) {

	return false;
}

int AbstractSensor::get(JsonObject& item) {

	return false;
}

void AbstractSensor::initChildren(int childCount) {
}

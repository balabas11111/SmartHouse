/*
 * AbstractSensorValue.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <sensors/AbstractSensorValue.h>

AbstractSensorValue::AbstractSensorValue(uint8_t id, String name, String type,
		String size, String descr, float val) {
	this->id=id;
	this->name=name;
	this->type=type;
	this->size=size;
	this->descr=descr;
	this->val=0;
}

//getters

uint8_t AbstractSensorValue::getId(){
	return id;
}

String AbstractSensorValue::getName(){
	return name;
}

const String& AbstractSensorValue::getType() const {
	return type;
}

const String& AbstractSensorValue::getSize() const {
	return size;
}

const String& AbstractSensorValue::getDescr() const {
	return descr;
}

float AbstractSensorValue::getVal() const {
	return val;
}

boolean AbstractSensorValue::getSetValAllowed() const {
	return setValAllowed;
}

boolean AbstractSensorValue::getSetStaticAllowed() const {
	return setStaticAllowed;
}

//setters

boolean AbstractSensorValue::setDescr(String& descr) {
	if(this->descr==descr){return false;}

	setChanged(true);
	this->descr=descr;

	return true;
}

boolean AbstractSensorValue::setVal(float val) {
	if(!getSetValAllowed() || this->val==val){
		return false;
	}
	setChanged(true);
	this->val=val;

	return true;
}

//by fieldname operations

const char* const AbstractSensorValue::getFieldsAll() {
	return SENSOR_VALUE_ALL_FIELDS_GET;
}

const char* const AbstractSensorValue::getFieldsStatic() {
	return SENSOR_VALUE_STATIC_FIELDS_GET;
}

const char* const AbstractSensorValue::getFieldsVal() {
	return SENSOR_VALUE_VAL_FIELDS_GET;
}

const char* const AbstractSensorValue::getFieldsSetAvailable() {
	return SENSOR_VALUE_FIELDS_SET;
}

const String& AbstractSensorValue::getByFieldName(String fieldName){

	if(fieldName==FPSTR(SENSOR_FIELD_descr)){
			return this->getDescr();
	}else
	if(fieldName==FPSTR(SENSOR_FIELD_val)){
		return this->getVal();
	}
	else
	if(fieldName==FPSTR(SENSOR_FIELD_hash)){
		return this->getHash();
	}else

	if(fieldName==FPSTR(SENSOR_FIELD_id)){
		return this->getId();
	}else
	if(fieldName==FPSTR(SENSOR_FIELD_name)){
		return this->getName();
	}else
	if(fieldName==FPSTR(SENSOR_FIELD_type)){
		return this->getType();
	}else
	if(fieldName==FPSTR(SENSOR_FIELD_size)){
		return this->getSize();
	}

	return NULL;
}

boolean AbstractSensorValue::setByFieldName(String fieldName, String fieldVal) {
	if(fieldName==NULL || fieldName==nullptr || fieldName.length()==0){
		return false;
	}
	if(fieldName==FPSTR(SENSOR_FIELD_descr)){
		setDescr(fieldVal);
		return true;
	}
	if(getSetValAllowed()){
		if(fieldName==FPSTR(SENSOR_FIELD_val)){
			setVal(fieldVal.toFloat());
			return true;
		}
	}
	if(fieldName==FPSTR(SENSOR_FIELD_hash)){
		setHash(fieldVal);
		return true;
	}

	if(getSetStaticAllowed()){
		if(fieldName==FPSTR(SENSOR_FIELD_id)){
			setId(fieldVal.toInt());
			return true;
		}else
		if(fieldName==FPSTR(SENSOR_FIELD_name)){
			setName(fieldVal);
			return true;
		}else
		if(fieldName==FPSTR(SENSOR_FIELD_type)){
			setType(fieldVal);
			return true;
		}else
		if(fieldName==FPSTR(SENSOR_FIELD_size)){
			setSize(fieldVal);
			return true;
		}
	}

	return false;
}

//HashAble impl
String AbstractSensorValue::getHashableString() {
	return getName();
}
//JsonSetGetAble implementations
int AbstractSensorValue::set(JsonObject& item) {
	return setValByFieldName(item, getFieldsAll());
}

int AbstractSensorValue::get(JsonObject& item) {
	return getValByFieldName(item, getFieldsAll());
}

int AbstractSensorValue::setVal(JsonObject& item) {
	if(!getSetValAllowed()){	return false;}

	return setValByFieldName(item, getFieldsSetAvailable());
}

int AbstractSensorValue::getVal(JsonObject& item) {
	return getValByFieldName(item, getFieldsVal());
}

int AbstractSensorValue::setStatic(JsonObject& item) {
	if(!getSetStaticAllowed()){return false;}

	return setValByFieldName(item, getFieldsStatic());
}

int AbstractSensorValue::getStatic(JsonObject& item) {
	return getValByFieldName(item, getFieldsStatic());
}
//protected methods
boolean AbstractSensorValue::setId(uint8_t id) {
	if(this->id==id){return false;}

	setChanged(true);
	this->id=id;
	return true;
}

boolean AbstractSensorValue::setName(String& name) {
	if(this->name==name){return false;}

	setChanged(true);
	this->name=name;
	return true;
}

boolean AbstractSensorValue::setType(String& type) {
	if(this->type==type){return false;}

	setChanged(true);
	this->type=type;
	return true;
}

boolean AbstractSensorValue::setSize(String& size) {
	if(this->size==size){return false;}

	setChanged(true);
	this->size=size;
	return true;
}

int AbstractSensorValue::setValByFieldName(JsonObject& item,const char* const fields) {
	uint8_t total=0;

	for(uint8_t i=0;i<sizeof(fields);i++){
		setByFieldName(FPSTR(fields[i]),item[FPSTR(fields[i])]);
		total++;
	}
	return total;
}

int AbstractSensorValue::getValByFieldName(JsonObject& item,const char* const fields) {
	uint8_t total=0;
	for(uint8_t i=0;i<sizeof(fields);i++){
		item[FPSTR(fields[i])]=getByFieldName(FPSTR(fields[i]));
		total++;
	}
	return total;
}

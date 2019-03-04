/*
 * EntityJson.h
 *
 *  Created on: 28 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIB_ENTITYJSON_H_
#define LIBRARIES_ENTITYLIB_ENTITYJSON_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <functional>

#include "EntityModelDataProvider.h"

class EntityJson {
public:
	EntityJson(const char* group,const char* name,const char* descr,const char* descriptor){
		this->id=-1;
		this->group=group;
		this->name=name;
		this->descr=strdup(descr);
		this->descriptor = descriptor;
	};
	virtual ~EntityJson(){};

	void attachParams(int id,EntityModelDataProvider* modelDataProvider){
		this->id=id;
		this->modelDataProvider=modelDataProvider;
	}

	virtual void init(){

	}

	int getId(){
		return this->id;
	}

	bool isChanged() {
		return this->changed;
	}

	bool setDescr(char* descr){
		if(!strcmp(this->descr,descr)==0){
			this->changed=true;
			this->descr=descr;
		}
		return this->changed;
	}

	char* getDescr() {
		return this->descr;
	}

	const char* getGroup() {
		return this->group;
	}

	const char* getName() {
		return this->name;
	}

	void setChanged(bool changed = false) {
		this->changed = changed;
	}

	void print(){
		Serial.print(FPSTR("  - Entity(id="));
		Serial.print(id);
		Serial.print(FPSTR(", group="));
		Serial.print(group);
		Serial.print(FPSTR(", name="));
		Serial.print(name);
		Serial.print(FPSTR(", descr="));
		Serial.print(descr);
		Serial.print(FPSTR(", descriptor="));
		Serial.print(descriptor);
		Serial.println(FPSTR(")"));
	}

	const char* getDescriptor() const {
		return descriptor;
	}

	EntityModelDataProvider* getModelDataProvider() {
		return modelDataProvider;
	}

protected:
	EntityModelDataProvider* modelDataProvider;

	bool changed=false;
	int id;
	const char* group;
	const char* name;
	char* descr;
	const char* descriptor;


};

#endif /* LIBRARIES_ENTITYLIB_ENTITYJSON_H_ */

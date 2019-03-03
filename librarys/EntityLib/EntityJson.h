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

	void init(){

	}

	int getId(){
		return this->id;
	}

	bool isChanged() const {
		return changed;
	}

	bool setDescr(char* descr){
		if(!strcmp(this->descr,descr)==0){
			changed=true;
			this->descr=descr;
		}
		return changed;
	}

	char* getDescr() {
		return this->descr;
	}

	const char* getGroup() {
		return group;
	}

	const char* getName() {
		return name;
	}

	void setChanged(bool changed = false) {
		this->changed = changed;
	}

	void setId(int id) {
		this->id = id;
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

protected:
	//std::function<void(EntityJson*,const char*)> registerEntityModelFunction;

	bool changed=false;
	int id;
	const char* group;
	const char* name;
	char* descr;
	const char* descriptor;


};

#endif /* LIBRARIES_ENTITYLIB_ENTITYJSON_H_ */

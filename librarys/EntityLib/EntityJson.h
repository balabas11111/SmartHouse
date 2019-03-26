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
#include <ObjectUtils.h>

#include "EntityModelDataProvider.h"

class EntityJson {
public:
	EntityJson(const char* group,const char* name,const char* descr,const char* descriptor){
		this->id=-1;
		this->group=group;
		this->name=name;
		this->descr=strdup(descr);
		this->descriptor = descriptor;
		this->modelDataProvider=nullptr;
	};
	virtual ~EntityJson(){};

	void attachParams(int id,EntityModelDataProvider* modelDataProvider){
		this->id=id;
		this->modelDataProvider=modelDataProvider;
	}

	virtual void init(){

	}

	virtual void postModelDataInit(){

	}

	virtual void postDataPosted(){

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
		Serial.println(descriptor);

		if(modelDataProvider!=nullptr){
			JsonObjectUtil::print("Data --->", modelDataProvider->getEntityData(getId()));
			JsonObjectUtil::print("Model --->", modelDataProvider->getEntityModel(getId()));
		}else{
			//Serial.println(FPSTR("NO data provider bind"));
		}
	}

	const char* getDescriptor() const {
		return descriptor;
	}

	bool hasPrimFields() const {
		return primFields;
	}

	bool isDeployedAsTemplate() const {
		return deployedAsTemplate;
	}

	bool isDeployedAsData() const {
		return deployedAsData;
	}

	JsonObject& getData(){
		return getModelDataProvider()->getEntityData(getId());
	}
	JsonObject& getModel(){
		return getModelDataProvider()->getEntityModel(getId());
	}
	JsonObject& getDictionary(){
		return getModelDataProvider()->getEntityDictionary(getId());
	}

	bool hasDictionaryValue(const char* key){
		return getDictionary().containsKey(key);
	}
	const char* getDictionaryValue(const char* key){
		JsonObject& dict = getDictionary();
		if(!dict.containsKey(key)){
			setDictionaryValue(key,key);
		}
		return dict.get<const char*>(key);
	}
	bool setDictionaryValue(const char* key,const char* value){
		return getDictionary().set(key, value);
	}

	template<typename T>
	bool hasFieldOfType(const char* key,T value){
		return getData().containsKey(key) && getData().is<T>(key);
	}

	template<typename T>
	bool setField(const char* key,T value){
		return getModelDataProvider()->setField(getId(), key, value);
	}

	template<typename T>
	T getField(const char* key){
		return getData().get<T>(key);
	}

	void sendAsEventSourceEntity(){
		modelDataProvider->sendEntityAsEventSourceNow(id);
	}

	virtual const char* validateField(const char* key,const String& value){
		UNUSED(key);UNUSED(value);
		return "";
	}
	virtual bool processFieldPreSave(const char* key,const char* value){
		UNUSED(key);UNUSED(value);
		return true;
	}

protected:
	EntityModelDataProvider* modelDataProvider;

	bool changed=false;
	int id;
	const char* group;
	const char* name;
	char* descr;
	const char* descriptor;

	bool primFields=true;
	bool deployedAsTemplate = true;
	bool deployedAsData = true;

	EntityModelDataProvider* getModelDataProvider() {
		return modelDataProvider;
	}
};

#endif /* LIBRARIES_ENTITYLIB_ENTITYJSON_H_ */

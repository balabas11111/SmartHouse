/*
 * EntityFieldHolder.h
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYFIELDHOLDER_H_
#define LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYFIELDHOLDER_H_

#include <Arduino.h>
#include <string>
#include <sstream>

#include <utils/CompareUtils.h>

#define PERSENT_STR "%"

class EntityField{
public:
	virtual ~EntityField(){};

	virtual void print()=0;

	virtual String getValStr()=0;

	virtual int getEntityFieldIndex(){
		return this->entFieldIndex;
	}

	virtual int getEntityId()=0;

	virtual const char* getKey(){
		return this->key;
	}

	virtual const char* getKeyTemplate(){
		return this->keyTemplate;
	}

	virtual bool isChangeAble(){
		return this->ch;
	}

	virtual void getFromField(JsonObject& target)=0;
	virtual void getFromFieldTemplateKey(JsonObject& target)=0;
	virtual void setToField(JsonObject& source)=0;

protected:
	bool ch;
	int entFieldIndex;
	int entityId;
	const char* key;
	const char* keyTemplate;
};

template<typename T>
class EntityFieldHolder: public EntityField {
public:
	EntityFieldHolder(int entFieldIndex,int entityId,const char* key,T value,bool changeAbleField){
		this->ch;
		this->entFieldIndex=entFieldIndex;
		this->entityId=entityId;
		this->key=key;
		this->value=value;

		std::ostringstream sstream;
		sstream << PERSENT_STR << entFieldIndex << PERSENT_STR;
		keyTemplate=sstream.str().c_str();
		sstream.clear();

		/*std::string query = sstream.str();
		std::string buf= PERSENT_STR;
		buf.append(entFieldIndex);
		buf.append(PERSENT_STR);
		*/
	};
	virtual ~EntityFieldHolder() {}

	T getValue() const {
		return this->value;
	}

	bool setValue(T value) {
		bool changed = this->value!=value;
		this->value = value;
		return changed;
	}

	virtual String getValStr(){
		return String(value);
	}

	void print() override{
		Serial.print(FPSTR("holder ("));
		Serial.print(entityId);
		Serial.print(FPSTR(" : "));
		Serial.print(value);
		Serial.print(FPSTR(") "));
	}

	virtual void getFromField(JsonObject& target) override{
		target.set(this->key, this->value);
	}
	virtual void getFromFieldTemplateKey(JsonObject& target) override{
		target.set(this->key, this->keyTemplate);
	}
	virtual void setToField(JsonObject& source) override{
		if(source.is<T>(this->key) && isChangeAble()){
			if(source.get<T>(this->key)!=this->getValue()){
				setValue(source.get<T>(this->key));
			}
		}
	}

	virtual int getEntityId() override{
		return this->entityId;
	}

	virtual bool isChangeAble() override{
		return this->ch;
	}

	virtual bool compare(T val){
		return CompareUtils::compareValues<T>(val,this->getValue());
	}

protected:
	T value;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYFIELDHOLDER_H_ */

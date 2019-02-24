/*
 * EntityField.h
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYFIELD_H_
#define LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYFIELD_H_

#include <string>

class EntityField {
public:
	EntityField(const char* key,int entityIndexId){
		this->key=key;
		this->entityIndexId=entityIndexId;

		int size= sizeof(key);
		size+=2;

		std::string buf(percent);
		buf.append(key);
		buf.append(percent);

		this->keyTmpl = buf.c_str();
	};

	virtual ~EntityField() {
	}
	int getEntityIndexId() const {
		return entityIndexId;
	}

	const char* getKey() const {
		return key;
	}

	const char* getTemplateValue(){
		return this->keyTmpl;
	}

	void print(){
		Serial.print(FPSTR("EntityField ("));
		Serial.print(FPSTR(" key="));
		Serial.print(key);
		Serial.print(FPSTR("; index="));
		Serial.print(key);
		Serial.print(FPSTR("; keyTmpl="));
		Serial.print(keyTmpl);
	}

protected:
	void generateKeyTmpl(){

	}

	const char* percent = "%";
	const char* keyTmpl;
	const char* key;
	int entityIndexId;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYFIELD_H_ */

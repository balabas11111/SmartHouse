/*
 * ESPExtraSettingsBox.h
 *
 *  Created on: 22 дек. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_ESPEXTRASETTINGSBOX_H_
#define LIBRARIES_ESPSETTINGSBOX_ESPEXTRASETTINGSBOX_H_

#include "Arduino.h"

class ESPExtraSettingsBox {
public:
	ESPExtraSettingsBox(){};

	virtual ~ESPExtraSettingsBox(){};

	virtual boolean init(){
		Serial.print(FPSTR("-----Init Extra Box---"));
		Serial.println(getName());

		/*
		Serial.print(FPSTR("keySize="));
		Serial.println(keySize);
		*/
		if(values==nullptr){
			values=new String[keySize];
		}

		const char* const* defaults=getDefaults();

		for(uint8_t i=0;i<keySize;i++){
			values[i]=String(defaults[i]);

			/*Serial.print(String(defaults[i]));
			Serial.print(FPSTR(";("));

			Serial.print(values[i]);
			Serial.print(FPSTR("):"));
			*/
		}

		Serial.println();

		Serial.println(FPSTR("...init done"));
		//Serial.println();
		return true;
	}

	virtual const char* const* getDefaults()=0;
	virtual const char* const*  getKeys()=0;

	virtual String* getValues(){
		return values;
	}

	String getName(){
		return name;
	}

	boolean hasKey(String key){
		return getKeyIndex(key)!=-1;
	}

	int getKeyIndex(String key){
		const char* const* keys=getKeys();
		for(uint8_t i=0;i<keySize;i++){
			if(String(keys[i])==key){
				return i;
			}
		}
		return -1;
	}

	String getValue(String key){
		int index=getKeyIndex(key);
		return getValue(index);
	}

	int getValueInt(String key){
		return getValue(key).toInt();
	}

	float getValueFloat(String key){
		return getValue(key).toFloat();
	}

	String getValue(int index){
		if(index==-1 || index>keySize){ return String("");}

		return values[index];
	}

	int getValueInt(int index){
		return getValue(index).toInt();
	}

	float getValueFloat(int index){
		return getValue(index).toFloat();
	}

	boolean setValue(String key,String value){
		int index=getKeyIndex(key);
		return setValue(index,value);
	}

	boolean setValue(int index,String value){
		if(index!=-1 && index<=keySize){
			values[index]=value;
			return true;
		}
		return false;
	}

	String getKey(int index){
		return String(getKeys()[index]);
	}

	void printDetails(){
		Serial.println(FPSTR("-------------------------"));
		Serial.print(FPSTR("keySize="));
		Serial.println(keySize);
		Serial.println(FPSTR("-----------KEYS----------"));

		const char* const* keys=getKeys();

		for(uint8_t i=0;i<keySize;i++){
			Serial.print(String(keys[i]));
			Serial.print(FPSTR(";"));
		}

		Serial.println();
		Serial.println(FPSTR("---------HTML Names------------"));

		for(uint8_t i=0;i<keySize;i++){
			Serial.print(getName());
			Serial.print(FPSTR("_"));
			Serial.println(String(keys[i]));
		}

		Serial.println(FPSTR("-----------VALUES----------"));

		String* vals=getValues();
/*
		Serial.print(FPSTR("keySize="));
		Serial.println(keySize);
*/
		for(uint8_t i=0;i<keySize;i++){
			Serial.print(vals[i]);
			Serial.print(FPSTR(";"));
		}

		Serial.println();
		Serial.println(FPSTR("---------------------"));
	}

	uint8_t getKeySize(){
		return keySize;
	}

	String getJson(){

		return "";
	}

protected:
	String name;
	String* values;
	uint8_t keySize=0;
};

#endif /* LIBRARIES_ESPSETTINGSBOX_ESPEXTRASETTINGSBOX_H_ */

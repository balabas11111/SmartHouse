/*
 * ESPExtraSettingsBox.h
 *
 *  Created on: 22 дек. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_ESPEXTRASETTINGSBOX_H_
#define LIBRARIES_ESPSETTINGSBOX_ESPEXTRASETTINGSBOX_H_

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define VAR_NAME(var) #var

#include "Arduino.h"
//added as suffix in EspSettingsBox getJson
const char HTML_LABEL_PREFFIX[]             PROGMEM ="lbl_";
const char SETTINGS_BOX_BASE_NAME[]         PROGMEM ="espSettingsBox_";

const char ALARM_SETTINGS_BOX_NAME[]        PROGMEM ="ALARM";
const char DISPLAY_SETTINGS_BOX_NAME[]      PROGMEM ="DISPLAY";
const char NTP_SETTINGS_BOX_NAME[]          PROGMEM ="NTP";
const char TELEGRAM_SETTINGS_BOX_NAME[]     PROGMEM ="TEL";

const char SETTINGS_KIND_all[]   PROGMEM ="all";
const char SETTINGS_KIND_device[]   PROGMEM ="device";
const char SETTINGS_KIND_net[]      PROGMEM ="net";
const char SETTINGS_KIND_sensors[]  PROGMEM ="sensors";
const char SETTINGS_KIND_publish[]  PROGMEM ="publish";
const char SETTINGS_KIND_intervals[]PROGMEM ="intervals";
const char SETTINGS_KIND_manage[]   PROGMEM ="manage";

class ESPExtraSettingsBox {
public:
	ESPExtraSettingsBox(){};

	virtual ~ESPExtraSettingsBox(){};

	virtual boolean init(){
		Serial.print(FPSTR("-----Init Extra Box---"));
		Serial.print(getName());

		if(values==nullptr){
			values=new String[keySize];
		}

		Serial.println(FPSTR("...done"));
		return true;
	}

	virtual const char* const* getDefaults()=0;
	virtual const char* const*  getKeys()=0;
	virtual String getDescription()=0;
	virtual String getSettingsKind()=0;

	virtual int fillDefaultValues(){
		Serial.println(FPSTR("fill default values"));

		const char* const* defaults=getDefaults();
		const char* const* keys=getKeys();

		for(uint8_t i=0;i<keySize;i++){
			values[i]=String(defaults[i]);

			printKeyDetails(String(keys[i]), String(values[i]), String(defaults[i]));
		}
		Serial.println(FPSTR("fill default values...done"));
		return keySize-1;
	}

	virtual String* getValues(){
		return values;
	}

	String getName(){
		return name;
	}

	boolean hasKey(String key){
		return getKeyIndex(key)!=-1;
	}

	String getKey(int index){
		return String(getKeys()[index]);
	}

	String getDefaultValue(int index){
		return String(getDefaults()[index]);
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

	String getValue(int index){
		if(index==-1 || index>=keySize){ return String("");}

		return values[index];
	}

	boolean setValue(int index,String value){
		if(index>-1 && index<keySize){
			values[index]=value;
			return true;
		}
		return false;
	}

	void printKeyDetails(String key,String value,String defaultVal){
		Serial.print(FPSTR("key="));
		Serial.print(key);
		Serial.print(FPSTR(" def="));
		Serial.print(defaultVal);
		Serial.print(FPSTR(" val="));
		Serial.println(value);
	}

	void printDetails(){
		Serial.println(FPSTR(""));
		Serial.print(FPSTR("-----------KEYS ("));
		Serial.print(keySize);
		Serial.print(FPSTR(") name="));
		Serial.print(getName());
		Serial.print(FPSTR(" descr="));
		Serial.print(getDescription());
		Serial.println(FPSTR("------------------"));

		//const char* const* keys=getKeys();
		for(uint8_t i=0;i<keySize;i++){
			//Serial.print(String(keys[i]));
			//-------------------------------
			uint expLen=28;
			String result=String(getKeys()[i]);
			if(result.length()>=expLen){
				result=result.substring(0, expLen);
			}
			if(result.length()<expLen){

				for(uint8_t j=0;j<expLen-result.length();j++){
					result+=" ";
				}
			}
			Serial.print(result);
			//-------------------------------
			Serial.print(FPSTR(" = "));
			Serial.println(getValues()[i]);
		}

		Serial.println(FPSTR("---------HTML Labels------------"));

		for(uint8_t i=0;i<keySize;i++){
			Serial.print(FPSTR(HTML_LABEL_PREFFIX));
			Serial.println(getKey(i));
		}
		Serial.println(FPSTR("---------HTML Names------------"));

		for(uint8_t i=0;i<keySize;i++){
			Serial.print(FPSTR(SETTINGS_BOX_BASE_NAME));
			Serial.println(getKeyHtmlName(i));
		}

		Serial.println(FPSTR("----------getJson()-----------"));
		Serial.println(getJson());
		Serial.println(FPSTR("------------------------------"));
	}

	uint8_t getKeySize(){
		return keySize;
	}

	int isTargetOfSettingsValue(String keyWithPreffix){
		String key="";
		int keyIndex=-1;

		if(keyWithPreffix.startsWith(getKeyPreffix())){
			key=keyWithPreffix.substring(getKeyPreffix().length()+1);
			keyIndex=getKeyIndex(key);
		}

		Serial.print(FPSTR("check is target keyWithPreffix="));
		Serial.print(keyWithPreffix);
		Serial.print(FPSTR(" key="));
		Serial.print(key);
		Serial.print(FPSTR(" keyIndex="));
		Serial.println(keyIndex);

		return keyIndex;
	}

	String getKeyPreffix(){
		return getName()+"_";
	}

	String getKeyHtmlName(int index){
		return getKeyPreffix()+getKey(index);
	}

	String getJson(){
		String result="";

		for(uint8_t i=0;i<getKeySize();i++){
			result+="{\"name\":\""+getKeyHtmlName(i)+"\",\"val\":\""+getValue(i)+"\"}";
			if(i!=getKeySize()-1){
				result+=",";
			}
		}
		return result;
	}

protected:
	String name;
	String* values;
	uint8_t keySize=0;
};

#endif /* LIBRARIES_ESPSETTINGSBOX_ESPEXTRASETTINGSBOX_H_ */

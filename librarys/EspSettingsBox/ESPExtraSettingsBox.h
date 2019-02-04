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
#include "interfaces/Nameable.h"
#include "EspSettingsUtil.h"

//added as suffix in EspSettingsBox getJson
const char HTML_LABEL_PREFFIX[]             PROGMEM ="lbl_";
const char SETTINGS_BOX_BASE_NAME[]         PROGMEM ="espSettingsBox_";

#define EXTRA_SETT_BOX_NAME_LENGTH 3

const char ALARM_SETTINGS_BOX_NAME[]        PROGMEM ="ALM";
const char DISPLAY_SETTINGS_BOX_NAME[]      PROGMEM ="DIS";
const char NTP_SETTINGS_BOX_NAME[]          PROGMEM ="NTP";
const char TELEGRAM_SETTINGS_BOX_NAME[]     PROGMEM ="TEL";

const char SETTINGS_KIND_all[]   PROGMEM ="all";
const char SETTINGS_KIND_device[]   PROGMEM ="device";
const char SETTINGS_KIND_net[]      PROGMEM ="net";
const char SETTINGS_KIND_sensors[]  PROGMEM ="sensors";
const char SETTINGS_KIND_publish[]  PROGMEM ="publish";
const char SETTINGS_KIND_intervals[]PROGMEM ="intervals";
const char SETTINGS_KIND_manage[]   PROGMEM ="manage";

const char* const SETTINGS_KINDS_SAVE_ENABLED[] PROGMEM=
{
		SETTINGS_KIND_device,
		SETTINGS_KIND_net,
		SETTINGS_KIND_publish
};

class ESPExtraSettingsBox:public Nameable {
public:
	ESPExtraSettingsBox(){};

	virtual ~ESPExtraSettingsBox(){};

	virtual boolean init(){
		Serial.print(FPSTR("-----Init Extra Box---"));
		Serial.print(getName());

		if(isInMemory()){
			if(vals==nullptr){
				vals=new String[getKeySize()];
			}
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

		String* values=(isInMemory())?vals:(new String[getKeySize()]);

		for(uint8_t i=0;i<getKeySize();i++){
			values[i]=String(defaults[i]);

			printKeyDetails(String(keys[i]), String(values[i]), String(defaults[i]));
		}
		Serial.println(FPSTR("fill default values...done"));
		return getKeySize()-1;
	}

	virtual String* getValues(){
		if(isInMemory()){
			return vals;
		}

		return getValuesFromFile();
	}

	virtual String getName()=0;

	virtual boolean isInMemory(){
		return true;
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
		for(uint8_t i=0;i<getKeySize();i++){
			if(String(keys[i])==key){
				return i;
			}
		}
		return -1;
	}

	String* getValuesFromFile(){
		return nullptr;
	}

	String getValueFromFile(int index){
		String fileName=EspSettingsUtil::getSettingsFilePath(getName());

		return EspSettingsUtil::getFieldValueFromFile(fileName, getKey(index));
	}

	boolean saveValueToFile(int index,String fieldValue){
		String fileName=EspSettingsUtil::getSettingsFilePath(getName());

		return EspSettingsUtil::updateFieldValueInFile(fileName, getKey(index), fieldValue);
	}

	String getValue(int index){
		if(index==-1 || index>=getKeySize()){ return String("");}

		if(isInMemory()){
			return vals[index];
		}
		return getValueFromFile(index);
	}

	boolean setValue(int index,String value){
		if(index>-1 && index<getKeySize()){

			if(isInMemory()){
				vals[index]=value;
				return true;
			}

			return saveValueToFile(index,value);
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
		Serial.println(FPSTR("-----------KEYS ----------"));
		Serial.print(FPSTR(" size="));
		Serial.print(getKeySize());
		Serial.print(FPSTR(") name="));
		Serial.print(getName());
		Serial.print(FPSTR(" descr="));
		Serial.print(getDescription());
		Serial.print(FPSTR(" inMemory="));
		Serial.println(isInMemory());
		Serial.println(FPSTR("---------------------------"));

		//const char* const* keys=getKeys();
		for(uint8_t i=0;i<getKeySize();i++){
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

		for(uint8_t i=0;i<getKeySize();i++){
			Serial.print(FPSTR(HTML_LABEL_PREFFIX));
			Serial.println(getKey(i));
		}
		Serial.println(FPSTR("---------HTML Names------------"));

		for(uint8_t i=0;i<getKeySize();i++){
			Serial.print(FPSTR(SETTINGS_BOX_BASE_NAME));
			Serial.println(getKeyHtmlName(i));
		}

		Serial.println(FPSTR("----------getJson()-----------"));
		Serial.println(getJson());
		Serial.println(FPSTR("------------------------------"));
	}

	uint8_t getKeySize(){
		return ARRAY_SIZE(getKeys());
	}

	int isTargetOfSettingsValue(String keyWithPreffix){
		String key="";
		int keyIndex=-1;

		if(keyWithPreffix.startsWith(getKeyPreffix())){
			key=keyWithPreffix.substring(getKeyPreffix().length());
			keyIndex=getKeyIndex(key);
		}
/*
		Serial.print(FPSTR("check is target keyWithPreffix="));
		Serial.print(keyWithPreffix);
		Serial.print(FPSTR(" key="));
		Serial.print(key);
		Serial.print(FPSTR(" keyIndex="));
		Serial.println(keyIndex);
*/
		return keyIndex;
	}

	String getKeyPreffix(){
		return getName()+"_";
	}

	String getKeyHtmlName(int index){
		return getKeyPreffix()+getKey(index);
	}

	String getJsonForNonMemoryBox(){
		String fileName=EspSettingsUtil::getSettingsFilePath(getName());
		String result=EspSettingsUtil::loadStringFromFile(fileName);

		return result;
	}

	String getJson(){

		if(!isInMemory()){
			return getJsonForNonMemoryBox();
		}
		String result="";

		for(uint8_t i=0;i<getKeySize();i++){
			result+="{\"name\":\""+getKeyHtmlName(i)+"\",\"val\":\""+getValue(i)+"\"}";
			if(i!=getKeySize()-1){
				result+=",";
			}
		}
		return result;
	}

	void setSaveRequired(boolean val){
		saveRequired=val;
	}

	boolean getSaveRequired(){
		return saveRequired;
	}

protected:
	String* vals;
	boolean saveRequired=false;
};

#endif /* LIBRARIES_ESPSETTINGSBOX_ESPEXTRASETTINGSBOX_H_ */

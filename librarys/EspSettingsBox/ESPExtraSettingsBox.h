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
#include "interfaces/Identifiable.h"
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
const char DEVICE_SETTINGS_BOX_NAME[]       PROGMEM ="DEV";
const char MQTT_SETTINGS_BOX_NAME[]         PROGMEM ="MQT";
const char THINGSPEAK_SETTINGS_BOX_NAME[]   PROGMEM ="TSP";
const char SMARTHOUSE_SETTINGS_BOX_NAME[]   PROGMEM ="SMH";

const char SETTINGS_KIND_all[]   	PROGMEM ="all";
const char SETTINGS_KIND_device[]   PROGMEM ="device";
const char SETTINGS_KIND_net[]      PROGMEM ="net";
const char SETTINGS_KIND_sensors[]  PROGMEM ="sensors";
const char SETTINGS_KIND_publish[]  PROGMEM ="publish";
const char SETTINGS_KIND_intervals[]PROGMEM ="intervals";
const char SETTINGS_KIND_manage[]   PROGMEM ="manage";

enum ExtraSettingsBoxIds: uint8_t{
	ExtraBox_Alarm, ExtraBox_Display, ExtraBox_Ntp, ExtraBox_Telegram, ExtraBox_Device, ExtraBox_mqtt, ExtraBox_thingSpeak, ExtraBox_SmartHouse
};

const char* const EXTRA_SETTINGS_BOX_NAMES[] PROGMEM=
{
		"ALM",
		"DIS",
		"NTP",
		"TEL",
		"DEV",
		"TSP",
		"MQT",
		"SMH"
};

const char* const SETTINGS_KINDS_SAVE_ENABLED[] PROGMEM=
{
		SETTINGS_KIND_device,
		SETTINGS_KIND_net,
		SETTINGS_KIND_publish
};

class ESPExtraSettingsBox:public Nameable, public Identifiable {
public:
	ESPExtraSettingsBox(){};
	virtual ~ESPExtraSettingsBox(){};

	virtual String getPath(){return FPSTR(ESPSETTINGSBOX_EXTRA_SETTINGS_BOX_PATH);}
	virtual boolean isInMemory(){return false;}
	virtual const char* const* getDefaults()=0;
	virtual const char* const*  getKeys()=0;
	virtual String getDescription()=0;
	virtual String getKind()=0;

	virtual boolean init(){
		Serial.print(FPSTR("-----Init Extra Box "));
		Serial.print(getName());
		Serial.print(FPSTR(" getKeySize="));
		Serial.println(getKeySize());

		if(isInMemory()){
			if(vals==nullptr){
				vals=new String[getKeySize()];
			}
		}

		validateFile();

		Serial.println(FPSTR("...done"));
		return true;
	}

	virtual void fillDefaultValues(){
		Serial.println(FPSTR("fill default values"));

		for(uint8_t i=0;i<getKeySize();i++){
			setValue(i, getDefaultValue(i));
		}
		Serial.println(FPSTR("fill default values...done"));
		//return getKeySize()-1;
	}

	String getBoxFilePath(){
		return getPath()+getName()+".txt";
	}

	String getKeyFilePath(uint8_t key){
		return getPath()+getName()+"/"+String(key)+".txt";
	}

	boolean load(){

	  if(!isInMemory()){
		  Serial.print(FPSTR("Box is not in memory Load disabled "));
		  Serial.println(getName());
		  return true;
	  }

	  String fileName=getBoxFilePath();
	  Serial.print(FPSTR("---Load Extra BOX from file "));
	  Serial.println(fileName);

	  if(!EspSettingsUtil::isFileExists(fileName)){
		   Serial.println(FPSTR("file NOT exists and will be recreated"));
		   fillDefaultValues();
		   save();
	   }

	  StaticJsonBuffer<1024> jsonBuffer;
	  delay(1);

	  File boxFile = SPIFFS.open(fileName, "r");
	  size_t size = boxFile.size();

	  std::unique_ptr<char[]> buf (new char[size]);
	  boxFile.readBytes(buf.get(), size);
	  JsonObject& root = jsonBuffer.parseObject(buf.get());
	  JsonArray& items= root["items"];

	  if (!root.success() || !items.success()) {
		  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
	  } else {
		  Serial.println();
		  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_READ_FROM_FILE_COMPLETE));

		for(uint8_t i=0;i<items.size();i++){
			uint8_t ind=getKeyIndex(items[i]["key"]);
			setValue(ind, items[i]["val"]);
		}
	  }
	  boxFile.close();
	  return true;
	}

	virtual boolean save(){
		String fileName=getBoxFilePath();
		return EspSettingsUtil::saveStringToFile(fileName, getJsonGenerated());
	}

	virtual boolean validateFile(){
		 Serial.println(FPSTR("-------Validate BOX file------"));

	   String boxFileName=getBoxFilePath();
	   boolean fileExists=EspSettingsUtil::isFileExists(boxFileName);

	   if(!fileExists){
		   Serial.println(FPSTR("file NOT exists and will be recreated"));
		   fillDefaultValues();
		   save();
	   }

	   File boxFile = SPIFFS.open(boxFileName, "r");
	   size_t size = boxFile.size();

		StaticJsonBuffer<1024> jsonBuffer;
		std::unique_ptr<char[]> buf (new char[size]);
		boxFile.readBytes(buf.get(), size);
		JsonObject& root = jsonBuffer.parseObject(buf.get());
		JsonObject& items= root["items"];

		boxFile.close();

		if (!root.success() || !items.success()) {
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
			return false;
		} else {
			Serial.println(FPSTR("Validate Keys"));

			boolean keysValid=true;

			uint8_t count=root["count"];

			if(count!=getKeySize() || count!=items.size()){
				Serial.println(FPSTR("Key size is invalid"));
				keysValid=false;
			}else{
				Serial.println(FPSTR("key size is valid"));
			}

			Serial.println(FPSTR("Compare cache and file"));

			JsonObject::iterator it;
			  for (it = items.begin(); it != items.end(); ++it ){
				  String key=it->key;
				  String val=it->value.asString();
				  uint8_t ind=getKeyIndex(key);

				  if(ind==-1){
					  Serial.print(FPSTR("Unknown key in file key="));
					  Serial.println(key);
					  keysValid=false;
					  break;
				  }

				  if(!isInMemory()){
					  String loaded=getValueFromFile(ind);
					  if(loaded!=val){
						  Serial.print(FPSTR("Cache and file diff key="));
						  Serial.println(key);
						  Serial.print(FPSTR(" cache="));
						  Serial.println(loaded);
						  Serial.print(FPSTR(" val="));
						  Serial.println(val);
						  keysValid=false;
						  break;
					  }
				  }

			  }

			if(keysValid){
				Serial.println(FPSTR("Check all keys are in file"));

				for(uint8_t i=0;i<getKeySize();i++){
					String key=getKey(i);
					keysValid=false;

					if(items.containsKey(key)){
						keysValid=true;
					}

					if(!keysValid){
						Serial.print(FPSTR("Missed in file key="));
						Serial.print(key);
						break;
					}
				}
			}

			if(!keysValid){
				Serial.println(FPSTR("RECREATE file"));
				fillDefaultValues();
				save();
			}else{
				Serial.println(FPSTR("All keys are presented in file"));
			}
		}

	   return true;
	}

	boolean hasKey(String key){
		return getKeyIndex(key)!=-1;
	}

	String getKey(int index){
		return String(getKeys()[index]);
	}

	virtual String getDefaultValue(int index){
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

	String getValueFromFile(int index){
		String fileName=getKeyFilePath(index);

		if(!EspSettingsUtil::isFileExists(fileName)){
			Serial.print(FPSTR("File not exists filename="));
			Serial.print(fileName);
			Serial.print(FPSTR(" return default value index="));
			Serial.println(index);

			return getDefaultValue(index);
		}

		return EspSettingsUtil::loadStringFromFile(fileName);
	}

	boolean saveFieldValueToFile(int index,String fieldValue){
		String fileName=getKeyFilePath(index);

		if(EspSettingsUtil::isFileExists(fileName)){
			String loaded=EspSettingsUtil::loadStringFromFile(fileName);
			Serial.print(FPSTR("Loaded="));
			Serial.println(loaded);
			if(loaded==fieldValue){
				Serial.print(FPSTR("File was not changed value="));
				Serial.println(fieldValue);
				saveRequired=false;
				return true;
			}
		}

		saveRequired=true;
		return EspSettingsUtil::saveStringToFile(fileName, fieldValue);
	}

	String getValue(int index){
		if(index==-1 || index>=getKeySize()){
			Serial.print(FPSTR("wrong index "));
			Serial.println(index);
			return String("");}

		if(isInMemory()){
			return vals[index];
		}
		return getValueFromFile(index);
	}

	virtual boolean validateSetValue(String fieldName,String value){
		return hasKey(fieldName);
	}

	boolean setValue(int index,String value){
		if(index>-1 && index<getKeySize()){

			if(isInMemory()){
				if(vals[index]!=value){
					saveRequired=true;
				}
				vals[index]=value;
				return true;
			}

			return saveFieldValueToFile(index,value);
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

		for(uint8_t i=0;i<getKeySize();i++){
			Serial.print(String(getKeys()[i]));

			Serial.print(FPSTR(" = "));
			Serial.println(getValue(i));
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
		//Serial.println(getJson());
		getJson();
		Serial.println(FPSTR("------------------------------"));
	}

	uint8_t getKeySize(){
		return keySize;
	}

	int isTargetOfSettingsValue(String keyWithPreffix){
		String key="";
		int keyIndex=-1;

		if(keyWithPreffix.startsWith(getKeyPreffix())){
			key=keyWithPreffix.substring(getKeyPreffix().length());
			keyIndex=getKeyIndex(key);
		}
		return keyIndex;
	}

	String getKeyPreffix(){
		return getName()+"_";
	}

	String getKeyHtmlName(int index){
		return getKeyPreffix()+getKey(index);
	}

	String getJson(){
		if(!isInMemory()){
			return getJsonFromFile();
		}

		return getJsonGenerated();
	}

	String getJsonFromFile(){
		String fileName=getBoxFilePath();
		String result=EspSettingsUtil::loadStringFromFile(fileName);

		return result;
	}

	String getJsonGenerated(){
		String result="{\"name\":\""+getName()+"\",\"count\":\""+getKeySize()+"\", \"kind\": \""+String(getKind())+"\", \"descr\": \""+getDescription()+"\", \"items\": {";

		for(uint8_t i=0;i<getKeySize();i++){
			result+="\""+getKey(i)+"\": \""+getValue(i)+"\"";
			if(i!=getKeySize()-1){
				result+=",";
			}
		}

		result+="}}";

		Serial.println(FPSTR("Json generated"));
		Serial.println(result);

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
	uint8_t keySize;
};

#endif /* LIBRARIES_ESPSETTINGSBOX_ESPEXTRASETTINGSBOX_H_ */

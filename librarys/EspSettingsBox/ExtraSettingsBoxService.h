/*
 * ExtraSettingsBoxService.h
 *
 *  Created on: 2 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_EXTRASETTINGSBOXSERVICE_H_
#define LIBRARIES_ESPSETTINGSBOX_EXTRASETTINGSBOXSERVICE_H_

#include "Arduino.h"
#include "ESP_Consts.h"
#include "ArduinoJson.h"
#include "FS.h"
#include "ESPExtraSettingsBox.h"
#include "EspSettingsUtil.h"

class ExtraSettingsBoxService {
public:
	ExtraSettingsBoxService(){
	};

	ExtraSettingsBoxService(ESPExtraSettingsBox** extraBoxes){
		this->extraBoxes=extraBoxes;
	};

	virtual ~ExtraSettingsBoxService(){};

	boolean initExtraBoxes(){
		if(getExtraBoxesCount()!=0){
			boolean result=true;
			Serial.println();
			Serial.println(FPSTR("----------------Init extra sett boxes---------------"));
			for(uint8_t i=0;i<getExtraBoxesCount();i++){
				result=extraBoxes[i]->init() && result;
				result=loadExtraBox(i) && result;
				extraBoxes[i]->printDetails();
			}

			Serial.print(FPSTR("Init extra sett boxes...result="));
			Serial.println(result);
			Serial.print(FPSTR("-----------------------------------------------------"));
			Serial.println();
			return result;
		}
		return false;
	}

	void beginSetExtraSettingsValue(String page){
		for(uint8_t i=0;i<getExtraBoxesCount();i++){
			if(extraBoxes[i]->getSettingsKind()==page){
				extraBoxes[i]->setSaveRequired(false);
			}
		}
	}

	boolean finishSetExtraSettingsValue(){
		boolean ebsr=false;

		for(uint8_t i=0;i<getExtraBoxesCount();i++){
			if(extraBoxes[i]->getSaveRequired()){
				Serial.print(extraBoxes[i]->getName());
				Serial.println(FPSTR(" saveRequired=1"));
				saveExtraBox(i);
				ebsr=true;
			}
			extraBoxes[i]->setSaveRequired(false);
		}

		if(!ebsr){
			Serial.println(FPSTR(" extraBoxes saveRequired=0"));
		}

		return ebsr;
	}

	boolean isSettingsFileExists(String settingsName){
	   String fileName=EspSettingsUtil::getSettingsFilePath(settingsName);
	   return EspSettingsUtil::isFileExists(fileName);
	}

	boolean deleteSettingsFile(String settingsName){
		String fileName=EspSettingsUtil::getSettingsFilePath(settingsName);
		return EspSettingsUtil::deleteFile(fileName);
	}

	String getDefaultExtraBoxJson(){
		return "{\"name\":\"extraBoxesCount\",\"val\":\""+String(getExtraBoxesCount())+"\"}";
	}

	String getExtraBoxJsonByKind(String extraBoxKind){
		String result="";
		for(uint8_t i=0;i<getExtraBoxesCount();i++){
			if(extraBoxKind==FPSTR(SETTINGS_KIND_all) || extraBoxes[i]->getSettingsKind()==extraBoxKind){
				result+=extraBoxes[i]->getJson();
				result+=",";
			}
		}

		result+=getDefaultExtraBoxJson();
		return result;
	}

	String getExtraBoxJson(String boxname){
		return getExtraBoxJson(getExtraBoxIndex(boxname));
	}

	String getExtraBoxJson(int boxIndex){
		if(boxIndex>getExtraBoxesCount() || boxIndex<0){
			return getDefaultExtraBoxJson();
		}

		return extraBoxes[boxIndex]->getJson();
	}

	boolean isBoxFileExists(uint8_t boxIndex){
		if(boxIndex>getExtraBoxesCount()){
			return false;
		}

		if(!isSettingsFileExists(extraBoxes[boxIndex]->getName())){
			saveExtraBox(boxIndex);
		}

		return true;
	}

	void saveExtraBoxes(){
		for(uint8_t i=0;i<getExtraBoxesCount();i++){
			saveExtraBox(i);
		}
	}

	void loadExtraBoxes(){
		for(uint8_t i=0;i<getExtraBoxesCount();i++){
			loadExtraBox(i);
		}
	}

	boolean saveExtraBox(uint8_t boxIndex){
	if(boxIndex>getExtraBoxesCount()){
		return false;
	}

		uint8_t boxKeySize=extraBoxes[boxIndex]->getKeySize();


		Serial.print(FPSTR("ExtraBox "));
		Serial.print(extraBoxes[boxIndex]->getName());
		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_BEGIN_SAVE));

		DynamicJsonBuffer jsonBuffer;

		JsonObject& root = jsonBuffer.createObject();

		for(uint8_t i=0;i<boxKeySize;i++){
			String key=extraBoxes[boxIndex]->getKeyHtmlName(i);
			String value=extraBoxes[boxIndex]->getValue(i);

			root[key] = value;
		}

		String vals="";
		root.printTo(vals);
		Serial.println(vals);
		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_BEGIN_SAVE));

		String boxFileName=EspSettingsUtil::getSettingsFilePath(extraBoxes[boxIndex]->getName());
		File boxFile = SPIFFS.open(boxFileName, "w");

		root.printTo(boxFile);
		boxFile.close();
		delay(1);

		Serial.println(FPSTR(MESSAGE_DONE));

		return true;
	}

	void deleteExtraBoxesFiles(){
		for(uint8_t i=0;i<getExtraBoxesCount();i++){
			deleteSettingsFile(extraBoxes[i]->getName());
		}
	}

	boolean validateExtraBoxFile(uint8_t boxIndex,boolean recreate){

	   Serial.println(FPSTR("-------Validate BOX file------"));

	   String boxFileName=EspSettingsUtil::getSettingsFilePath(extraBoxes[boxIndex]->getName());
	   boolean fileExists=isSettingsFileExists(extraBoxes[boxIndex]->getName());

	   if(!fileExists && recreate){
		   Serial.println(FPSTR("file NOT exists and will be recreated"));
		   extraBoxes[boxIndex]->fillDefaultValues();
		   saveExtraBox(boxIndex);
	   }else{
		   //Serial.println(FPSTR("file exists"));
	   }

	   File boxFile = SPIFFS.open(boxFileName, "r");
	   size_t size = boxFile.size();

		StaticJsonBuffer<1024> jsonBuffer;
		std::unique_ptr<char[]> buf (new char[size]);
		boxFile.readBytes(buf.get(), size);
		JsonObject& root = jsonBuffer.parseObject(buf.get());

		boxFile.close();

		if (!root.success()) {
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
			return false;
		} else {
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_LINES));
			root.printTo(Serial);
			Serial.println(FPSTR("Validate Keys"));

			uint8_t boxKeySize=extraBoxes[boxIndex]->getKeySize();
			boolean keysValid=true;

			for(uint8_t i=0;i<boxKeySize;i++){
				String key=extraBoxes[boxIndex]->getKey(i);
				boolean inFile=root.containsKey(key);

				if(!inFile){
					keysValid=false;
					Serial.print(FPSTR("key="));
					Serial.print(key);
					Serial.println(FPSTR(" - NOT in file"));
				}
			}

			if(!keysValid){
				Serial.println(FPSTR("RECREATE file"));
				deleteSettingsFile(extraBoxes[boxIndex]->getName());

				extraBoxes[boxIndex]->fillDefaultValues();

				saveExtraBox(boxIndex);
			}else{
				Serial.println(FPSTR("All keys are presented in file"));
			}
		}

	   return true;
	}

	boolean loadExtraBox(uint8_t boxIndex){

		  String boxFileName=EspSettingsUtil::getSettingsFilePath(extraBoxes[boxIndex]->getName());
		  Serial.print(FPSTR("---Load Extra BOX from file "));
		  Serial.println(boxFileName);

		  validateExtraBoxFile(boxIndex,true);

	      StaticJsonBuffer<1024> jsonBuffer;
	      delay(1);

	      File boxFile = SPIFFS.open(boxFileName, "r");
	      size_t size = boxFile.size();

	      std::unique_ptr<char[]> buf (new char[size]);
	      boxFile.readBytes(buf.get(), size);
	      JsonObject& root = jsonBuffer.parseObject(buf.get());
	      if (!root.success()) {
	    	  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
	      } else {
	    	  //Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_LINES));
	    	  //root.printTo(Serial);
	    	  Serial.println();
	    	  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_READ_FROM_FILE_COMPLETE));

	    	  uint8_t boxKeySize=extraBoxes[boxIndex]->getKeySize();

	    	  for(uint8_t i=0;i<boxKeySize;i++){
					String key=extraBoxes[boxIndex]->getKeyHtmlName(i);
					String value=root[key].as<char*>();

					uint8_t keyIndex=i;

					if(extraBoxes[boxIndex]->getKey(i)!=key){
						keyIndex=extraBoxes[boxIndex]->getKeyIndex(key);
					}

					extraBoxes[boxIndex]->setValue(keyIndex,value);
				}

	    	  	Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SETTINGS_TO_BOX_MEMORY));
	    	  	String vals="";
	    	  	root.printTo(vals);
	    	  	Serial.println(vals);
	    	  	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

	      	  }

	      boxFile.close();
		  return true;
		}

		boolean loadExtraBoxDefaultValues(uint8_t boxIndex){
			if(boxIndex>getExtraBoxesCount()){
				return false;
			}

			return extraBoxes[boxIndex]->init();
		}

		boolean hasExtraBoxes(){
			return getExtraBoxesCount()!=0;
		}

		boolean hasExtraBoxes(int boxIndex){
			return getExtraBoxesCount()!=0 && boxIndex>-1 && boxIndex<getExtraBoxesCount();
		}

		int getExtraBoxIndex(String boxName){
			if(hasExtraBoxes()){
				for(uint8_t i=0;i<getExtraBoxesCount();i++){
					if(extraBoxes[i]->getName()==boxName){
						return i;
					}
				}
			}

			return -1;
		}

		ESPExtraSettingsBox* getExtraBox(String boxName){
			int boxIndex=getExtraBoxIndex(boxName);

			if(boxIndex!=-1){
				return extraBoxes[boxIndex];
			}

			return nullptr;
		}

		int getExtraKeyIndex(int boxIndex,String key){
			if(!hasExtraBoxes(boxIndex)){return -1;}

			return extraBoxes[boxIndex]->getKeyIndex(key);
		}

		String getExtraKeyByIndex(int boxIndex,int keyIndex){
			if(!hasExtraBoxes(boxIndex)){return "";}

			return extraBoxes[boxIndex]->getKey(keyIndex);
		}

		boolean hasExtraKey(int boxIndex,String key){
			return getExtraKeyIndex(boxIndex, key)>-1;
		}

	//--------------------Get Extra values-----------------------
		String getExtraValue(int boxIndex,int keyIndex){
			if(!hasExtraBoxes(boxIndex)){return "";};

			return extraBoxes[boxIndex]->getValue(keyIndex);
		}

		String getExtraValue(String boxName,int index){
			int boxIndex=getExtraBoxIndex(boxName);

			return getExtraValue(boxIndex,index);
		}

		String getExtraValue(String boxName,String key){
			int boxIndex=getExtraBoxIndex(boxName);
			int keyIndex=getExtraKeyIndex(boxIndex, key);

			return getExtraValue(boxIndex, keyIndex);
		}

		int getExtraValueInt(int boxIndex,int index){
			return getExtraValue(boxIndex,index).toInt();
		}

		int getExtraValueInt(String boxName,String key){
			return getExtraValue(boxName,key).toInt();
		}

		int getExtraValueInt(String boxName,int index){
			return getExtraValue(boxName,index).toInt();
		}

		float getExtraValueFloat(int boxIndex,int index){
			return getExtraValue(boxIndex,index).toFloat();
		}

		float getExtraValueFloat(String boxName,String key){
			return getExtraValue(boxName,key).toFloat();
		}

		float getExtraValueFloat(String boxName,int index){
			return getExtraValue(boxName,index).toFloat();
		}

		boolean getExtraValueBoolean(int boxIndex,int key){
			return EspSettingsUtil::stringToBoolean(getExtraValue(boxIndex, key));
		}

		boolean getExtraValueBoolean(String boxName,String key){
			return EspSettingsUtil::stringToBoolean(getExtraValue(boxName, key));
		}

		boolean getExtraValueBoolean(String boxName,int index){
			return EspSettingsUtil::stringToBoolean(getExtraValue(boxName, index));
		}

	//-------------------set extra values--------------
	boolean setExtraValue(int boxIndex,int keyIndex,String value){
		if(!hasExtraBoxes(boxIndex)){return false;}

		return extraBoxes[boxIndex]->setValue(keyIndex, value);
	}

	boolean setExtraValue(int boxIndex,String key,String value){
		int keyIndex=getExtraKeyIndex(boxIndex, key);

		return setExtraValue(boxIndex,keyIndex,value);
	}


	boolean setExtraValue(String boxName,String key,String value){
		int boxIndex=getExtraBoxIndex(boxName);

		return setExtraValue(boxIndex,key,value);
	}

	boolean setExtraValue(String boxName,String key,int value){
		return setExtraValue(boxName,key,String(value));
	}

	boolean setExtraValue(String boxName,String key,float value){
		return setExtraValue(boxName,key,String(value));
	}

	boolean setExtraValue(String boxName,String key,boolean value){
		return setExtraValue(boxName,key,String(value));
	}

	void testExtraBoxFunctionality(){
		if(getExtraBoxesCount()==0){
			Serial.println(FPSTR("---Test FAILED---"));
			return;
		}

		Serial.println(FPSTR("-----------------Test extraBoxes started----------------"));

		int boxIndex=0;
		int keyIndex=0;

		ESPExtraSettingsBox* box=extraBoxes[boxIndex];

		String boxName=box->getName();

		Serial.println(FPSTR("--------------------------------------------------------"));

		box->printDetails();

		String key=box->getKey(keyIndex);
		String value=getExtraValue(boxName,keyIndex);
		String valueByStr=getExtraValue(boxName, key);

		String newValue="newValue";

		Serial.println("key="+key+" value="+value+" valueByStr="+valueByStr);

		Serial.println(FPSTR("-----------------SET NEW VALUE---------------------"));
		setExtraValue(boxName, key, newValue);

		box->printDetails();

		saveExtraBox(boxIndex);
		loadExtraBox(boxIndex);
		box->printDetails();

		Serial.println(FPSTR("-----------------SET OLD VALUE---------------------"));
		setExtraValue(boxName, key, value);

		saveExtraBox(boxIndex);
		loadExtraBox(boxIndex);
		box->printDetails();

		Serial.println(FPSTR("-----------------JSON---------------------"));
		//Serial.println(getJson(FPSTR(SETTINGS_KIND_device)));
	}

	boolean setSettingsValueIfExtra(String fieldName, String fieldValue){
		if(!hasExtraBoxes()){
			return false;
		}
		if(fieldName.length()<EXTRA_SETT_BOX_NAME_LENGTH ||
				fieldName.charAt(EXTRA_SETT_BOX_NAME_LENGTH)!='_'){
			return false;
		}

		for(uint8_t i=0;i<getExtraBoxesCount();i++){
			int keyIndex=extraBoxes[i]->isTargetOfSettingsValue(fieldName);

			if(keyIndex>-1){
				if(extraBoxes[i]->getValue(keyIndex)!=fieldValue){
					Serial.print(FPSTR(" - value updated key="));
					Serial.print(fieldName);
					Serial.print(FPSTR(" current="));
					Serial.print(extraBoxes[i]->getValue(keyIndex));
					Serial.print(FPSTR(" updated="));
					Serial.println(fieldValue);
					extraBoxes[i]->setValue(keyIndex, fieldValue);
					extraBoxes[i]->setSaveRequired(true);
				}
				return true;
			}
		}

		return false;
	}

	uint8_t getExtraBoxesCount(){
		if(extraBoxes==nullptr || extraBoxes==NULL){
			return 0;
		}
		return ARRAY_SIZE(extraBoxes);
	}

private:
	ESPExtraSettingsBox** extraBoxes;
};

#endif /* LIBRARIES_ESPSETTINGSBOX_EXTRASETTINGSBOXSERVICE_H_ */

/*
 * ExtraSettingsBoxService.h
 *
 *  Created on: 2 ����. 2019 �.
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

	ExtraSettingsBoxService(ESPExtraSettingsBox** extraBoxes,uint8_t boxCount){
		this->extraBoxes=extraBoxes;
		this->extraBoxCount=boxCount;
	};

	virtual ~ExtraSettingsBoxService(){};

	boolean initExtraBoxes(){
		if(getExtraBoxesCount()!=0){
			boolean result=true;
			Serial.println();
			Serial.print(FPSTR("----------------Init extra sett boxes count="));
			Serial.println(getExtraBoxesCount());

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
			if(extraBoxes[i]->getKind()==page){
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
	   String fileName=EspSettingsUtil::getExtraSettingsBoxFilePath(settingsName);
	   return EspSettingsUtil::isFileExists(fileName);
	}

	boolean deleteSettingsFile(String settingsName){
		String fileName=EspSettingsUtil::getExtraSettingsBoxFolderPath(settingsName);
		EspSettingsUtil::deleteFile(fileName);

		fileName=EspSettingsUtil::getExtraSettingsBoxFilePath(settingsName);
		return EspSettingsUtil::deleteFile(fileName);
	}

	String getExtraBoxJsonByKind(String extraBoxKind){
		uint8_t count=0;;

		String result="[";
		for(uint8_t i=0;i<getExtraBoxesCount();i++){
			if(extraBoxKind==FPSTR(SETTINGS_KIND_all) || extraBoxes[i]->getKind()==extraBoxKind){
				result+=extraBoxes[i]->getJson();
				result+=",";
				count++;
			}
		}
		if(count!=0){
			result.setCharAt(result.length()-1, ']');
		}else{
			result+="]";
		}

		return result;
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

		return extraBoxes[boxIndex]->save();
	}

	void deleteExtraBoxesFiles(){
		Serial.print(FPSTR("Delete extra boxes files count="));
		Serial.println(getExtraBoxesCount());

		for(uint8_t i=0;i<getExtraBoxesCount();i++){
			deleteSettingsFile(extraBoxes[i]->getName());
		}
	}

	boolean validateExtraBoxFile(uint8_t boxIndex,boolean recreate){

	   if(boxIndex>getExtraBoxesCount()){
			return false;
	   }

	   return extraBoxes[boxIndex]->validateFile();
	}

	boolean loadExtraBox(uint8_t boxIndex){
		if(boxIndex>getExtraBoxesCount()){
			return false;
		}

		return extraBoxes[boxIndex]->load();
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

	int getExtraBoxIndex(String name){
		if(hasExtraBoxes()){
			for(uint8_t i=0;i<getExtraBoxesCount();i++){
				if(extraBoxes[i]->getName()==name){
					return i;
				}
			}
		}

		return -1;
	}

	int getExtraBoxIndex(uint8_t id){
		if(hasExtraBoxes()){
			for(uint8_t i=0;i<getExtraBoxesCount();i++){
				if(extraBoxes[i]->getId()==id){
					return i;
				}
			}
		}

		return -1;
	}

	ESPExtraSettingsBox* getExtraBox(uint8_t id){
		int boxIndex=getExtraBoxIndex(id);

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
	String getExtraValueByBoxIndex(int boxIndex,int keyIndex){
		if(!hasExtraBoxes(boxIndex)){return "";};
		return extraBoxes[boxIndex]->getValue(keyIndex);
	}

	boolean getExtraValueBooleanByBoxIndex(int boxIndex,int keyIndex){
		return EspSettingsUtil::stringToBoolean(getExtraValueByBoxIndex(boxIndex, keyIndex));
	}

	int getExtraValueIntByBoxIndex(int boxIndex,int keyIndex){
		return getExtraValueByBoxIndex(boxIndex, keyIndex).toInt();
	}

	float getExtraValueFloatByBoxIndex(int boxIndex,int keyIndex){
		return getExtraValueByBoxIndex(boxIndex, keyIndex).toFloat();
	}

	IPAddress getExtraValueIpAdress(uint8_t boxId,int keyId){

	}

	String getExtraValue(uint8_t boxId,int keyId){
		int boxIndex=getExtraBoxIndex(boxId);

		return getExtraValueByBoxIndex(boxIndex,keyId);
	}

	String getExtraValue(String boxName,int keyId){
		int boxIndex=getExtraBoxIndex(boxName);

		return getExtraValueByBoxIndex(boxIndex,keyId);
	}

	String getExtraValue(String boxName,String keyName){
		int boxIndex=getExtraBoxIndex(boxName);
		int keyId=getExtraKeyIndex(boxIndex, keyName);

		return getExtraValueByBoxIndex(boxIndex,keyId);
	}

	int getExtraValueInt(uint8_t boxId,int keyId){
		return getExtraValue(boxId,keyId).toInt();
	}

	float getExtraValueFloat(String boxName,int index){
		return getExtraValue(boxName,index).toFloat();
	}



	boolean getExtraValueBoolean(uint8_t boxId,int keyId){
		return EspSettingsUtil::stringToBoolean(getExtraValue(boxId, keyId));
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

		if(boxIndex<0 || keyIndex<0 || !extraBoxes[boxIndex]->validateSetValue(key, value)){
			Serial.println(FPSTR("Invalid value"));
			return false;
		}

		return setExtraValue(boxIndex,keyIndex,value);
	}

	boolean setExtraValue(String boxName,String key,String value){
		int boxIndex=getExtraBoxIndex(boxName);

		return setExtraValue(boxIndex,key,value);
	}

	boolean setExtraValue(uint8_t boxid,uint8_t key,int value){
		return setExtraValue(boxid,key,String(value));
	}

	boolean setExtraValue(String boxName,String key,float value){
		return setExtraValue(boxName,key,String(value));
	}

	boolean setExtraValue(String boxName,String key,boolean value){
		return setExtraValue(boxName,key,String(value));
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
		return extraBoxCount;
	}
private:

	ESPExtraSettingsBox** extraBoxes;
	uint8_t extraBoxCount;
};

#endif /* LIBRARIES_ESPSETTINGSBOX_EXTRASETTINGSBOXSERVICE_H_ */

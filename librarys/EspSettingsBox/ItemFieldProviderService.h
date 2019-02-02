/*
 * ItemFieldProviderServ.h
 *
 *  Created on: 2 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ITEMFIELDPROVIDERSERV_H_
#define LIBRARIES_DEVICELIB_ITEMFIELDPROVIDERSERV_H_

#include <interfaces/ItemFieldDescriptor.h>
#include <ItemFIeldDescriptors/ItemFieldProviderMinMaxAlarm.h>
#include <ItemFIeldDescriptors/ItemFieldProviderThingSpeak.h>
#include "Arduino.h"
#include "ArduinoJson.h"
#include "ESP_Consts.h"
#include "EspSettingsUtil.h"

class ItemFieldProviderService {
public:

	ItemFieldProviderService(){
		this->itemFieldsDescriptorSize=2;

		ItemFieldDescriptor* pr0=new ItemFieldProviderThingSpeak();
		ItemFieldDescriptor* pr1=new ItemFieldProviderMinMaxAlarm();

		ItemFieldDescriptor* array[2]={pr0,pr1};

		this->itemFieldsDescriptors=array;
	}

	virtual ~ItemFieldProviderService(){};

	virtual String getItemFieldProviderJson(const uint8_t* pIds,String aName,String sName){
		String result="";

		uint8_t pCount=ARRAY_SIZE(pIds);

		if(pCount>0){
			result+=FPSTR(",\"ItemFieldsProviders\": {");

			for(uint8_t i=0;i<pCount;i++){
				String pName=getItemFieldsDescriptorById(pIds[i])->getName();
				result+=loadItemFieldProviderJson(pName,aName,sName);
				result+=",";
			}

			result.setCharAt(result.length(), '}');
		}

		return result;
	}

	static String getAbstractItemExtraSettFileFileName(String pName,String aName,String sName){
		String jsonFileName=FPSTR(ESPSETTINGSBOX_SETTINGS_PATH);
				jsonFileName+=aName;
				jsonFileName+="/";
				jsonFileName+=sName;
				jsonFileName+="/";
				jsonFileName+=pName;
				jsonFileName+=FPSTR(ESPSETTINGSBOX_SETTINGS_TXT_EXT);
		return jsonFileName;
	}

	const char* getJsonFieldValue(String json,String field){
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(json);

		return root[field];
	}

	virtual void saveDefaultItemFieldProviderValuesIfMissing(uint8_t pId,String aName,String sName){
			String pName=itemFieldsDescriptors[pId]->getName();
			String fileName=getAbstractItemExtraSettFileFileName(pName,aName,sName);

			if(!EspSettingsUtil::isFileExists(fileName)){
				saveStringToFile(fileName,String(itemFieldsDescriptors[pId]->getDefaults()));
			}
	}

	virtual ItemFieldDescriptor* getItemFieldsDescriptorByName(String name){
		for(uint8_t i=0;i<itemFieldsDescriptorSize;i++){
			if(itemFieldsDescriptors[i]->getName()==name){
				return itemFieldsDescriptors[i];
			}
		}

		return NULL;
	}

	virtual ItemFieldDescriptor* getItemFieldsDescriptorById(uint8_t id){
		return itemFieldsDescriptors[id];
	}

	virtual void saveItemFieldProvider(uint8_t pId,String aName,String sName,uint8_t fieldId,String fieldValue){
		String pName=itemFieldsDescriptors[pId]->getName();
		String fileName=getAbstractItemExtraSettFileFileName(pName,aName,sName);

		String json=loadItemFieldProviderJson(pName, aName, sName);
		String fieldName=itemFieldsDescriptors[pId]->getKeys()[fieldId];

		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(json);

		if(root[fieldName]!=fieldValue){
			root[fieldName]=fieldValue;
		}

		root.printTo(json);

		saveStringToFile(fileName, json);
	}

	String loadItemFieldProviderJsonById(uint8_t pId,String aName,String sName){
		String pName=getItemFieldsDescriptorById(pId)->getName();
		String fileName=getAbstractItemExtraSettFileFileName(pName,aName,sName);
		return loadStringFromFile(fileName);
	}

	String loadItemFieldProviderJson(String pName,String aName,String sName){
		String fileName=getAbstractItemExtraSettFileFileName(pName,aName,sName);
		return loadStringFromFile(fileName);
	}

	const char* getItemFieldProviderValue(String aName,String sName,uint8_t providerId,uint8_t fieldId){
		String keyName=itemFieldsDescriptors[providerId]->getKeys()[fieldId];
		String fileName=getAbstractItemExtraSettFileFileName(itemFieldsDescriptors[providerId]->getName(),aName,sName);
		String json=loadStringFromFile(fileName);

		return getJsonFieldValue(json,keyName);
	}


	void saveItemFieldProviderJson(String pName,String aName,String sName,String json){
		String fileName=getAbstractItemExtraSettFileFileName(pName,aName,sName);
		saveStringToFile(fileName,json);
	}

	static String loadStringFromFile(String fileName){
		File file = SPIFFS.open(fileName, "r");

		String result=file.readString();
		file.close();

		delay(1);
		Serial.println("LOAD fileName="+fileName+" result="+result);
		return result;
	}

	boolean saveStringToFile(String fileName,String data){

		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_SETTINGS_SAVE_BEGIN));
		Serial.println(fileName);

		File file = SPIFFS.open(fileName, "w");
		if(!file){
			Serial.println(FPSTR("Error opening file for write"));
		}

		size_t fileSize=file.print(data);

		file.close();
		Serial.println(data);
		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_SAVED));
		Serial.println(fileSize);

		delay(1);
		return fileSize>0;
	}

	/*
	const char** loadExtraSettHeadArray(ItemFieldProvider* itemEs,String sName){
		uint8_t count=itemEs->getSize();

		String fileName=getAbstractItemExtraSettFileFileName(itemEs->getName(),sName);

		File file = SPIFFS.open(fileName, "r");

		if(!file){
			//saveAbstractItemToFile(item);
			file = SPIFFS.open(fileName, "r");
		}

		  if (!file){
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_FILE_NOT_EXISTS));
		  } else {
			size_t size = file.size();
			if ( size == 0 ) {
			  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_FILE_EMPTY));
			} else {

				StaticJsonBuffer<1024> jsonBuffer;
				delay(1);

				std::unique_ptr<char[]> buf (new char[size]);
				file.readBytes(buf.get(), size);

				JsonObject& root = jsonBuffer.parseObject(buf.get());

				if (!root.success()) {
					Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
				} else {

					const char* result[count];
					const char* const* keys= itemEs->getKeys();

					for(uint8_t i=0;i<count;i++){
						result[i]=root[keys[i]].asString();
					}

					file.close();
					return result;
				}
			}
		  }
		  file.close();

		}
	*/

protected:
	ItemFieldDescriptor** itemFieldsDescriptors;
	uint8_t itemFieldsDescriptorSize;
};

#endif /* LIBRARIES_DEVICELIB_ITEMFIELDPROVIDERSERV_H_ */

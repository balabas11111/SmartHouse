/*
 * EspSettingsUtil.h
 *
 *  Created on: 2 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_ESPSETTINGSUTIL_H_
#define LIBRARIES_ESPSETTINGSBOX_ESPSETTINGSUTIL_H_

#include "Arduino.h"
#include "FS.h"
#include "IPAddress.h"
#include "ArduinoJson.h"

#include "ESP_Consts.h"

class EspSettingsUtil {
public:

	static boolean isFileExists(String fileName){
		return SPIFFS.exists(fileName);
	/*
	   File file = SPIFFS.open(fileName, "r");

	   boolean exists=!(!file || file.size()==0);

	   if(exists){
		   Serial.print(FPSTR("File exists "));
		   Serial.print(fileName);
		   Serial.print(FPSTR(" size="));
		   Serial.println(file.size());
	   }else{
		   Serial.print(FPSTR("NOT exists "));
		   Serial.println(fileName);
	   }

	   file.close();

	   return exists;
	   */
	}

	static String getFieldValueFromFile(String fileName,String fieldName){
		String json=loadStringFromFile(fileName);

		if(json==""){
			return json;
		}

		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(json);

		String fieldValue=root[fieldName];

		Serial.print(fieldName);
		Serial.print(FPSTR("="));
		Serial.print(fieldValue);
		Serial.print(FPSTR(" f="));
		Serial.println(fileName);

		return fieldValue;

	}

	static boolean updateFieldValueInFile(String fileName,String fieldName,String fieldValue){
		String json=loadStringFromFile(fileName);

		if(json==""){
			return false;
		}
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(json);

		if(root[fieldName]!=fieldValue){
			root[fieldName]=fieldValue;
			json="";
			root.printTo(json);

			EspSettingsUtil::saveStringToFile(fileName, json);
		}

		return true;
	}

	static boolean saveStringToFile(String fileName,String data){

		File file = SPIFFS.open(fileName, "w");
		if(!file){
			Serial.println(FPSTR("Error opening file for write"));
		}

		size_t fileSize=file.print(data);

		file.close();
		Serial.print(FPSTR("SAVE file="));
		Serial.println(fileName);
		Serial.println(data);
		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_SAVED));
		Serial.println(fileSize);

		delay(1);
		return fileSize>0;
	}

	static String loadStringFromFile(String fileName){
		File file = SPIFFS.open(fileName, "r");

		if(!file || file.size()==0){
			Serial.print(FPSTR("not found "));
			Serial.println(fileName);
			return "";
		}

		String data=file.readString();
		file.close();

		delay(1);
		Serial.print(FPSTR("LOAD file="));
		Serial.println(fileName);
		Serial.println(data);
		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SAVED));

		return data;
	}

	static boolean deleteFile(String fileName){
		boolean result= SPIFFS.remove(fileName);

		Serial.print(FPSTR("DELETE file ="));
		Serial.print(fileName);
		Serial.print(FPSTR(" result ="));
		Serial.println(result);

		return result;
	}

	static String getAbstractItemSettingsFilePath(String abstractItemName){
		String jsonFileName=FPSTR(ESPSETTINGSBOX_EXTRA_SETTINGS_PATH);
				jsonFileName+=abstractItemName;
				jsonFileName+=FPSTR(ESPSETTINGSBOX_SETTINGS_TXT_EXT);
		return jsonFileName;
	}

	static String getExtraSettingsBoxFilePath(String settingsName){
		String jsonFileName=FPSTR(ESPSETTINGSBOX_EXTRA_SETTINGS_PATH);
				jsonFileName+=settingsName;
				jsonFileName+=FPSTR(ESPSETTINGSBOX_SETTINGS_TXT_EXT);
		return jsonFileName;
	}

	static String getExtraSettingsBoxFolderPath(String settingsName){
		String jsonFileName=FPSTR(ESPSETTINGSBOX_EXTRA_SETTINGS_PATH);
				jsonFileName+=settingsName;
				jsonFileName+=FPSTR("/");
		return jsonFileName;
	}

	static String getExtraSettingsBoxFieldPath(String settingsName,String fieldName){
		String jsonFileName=getExtraSettingsBoxFolderPath(settingsName);
						jsonFileName+=fieldName;
						jsonFileName+=FPSTR(ESPSETTINGSBOX_SETTINGS_TXT_EXT);
		return jsonFileName;
	}

	static boolean stringToBoolean(String str){
		return (str=="on" || str=="1" || str=="true" || str=="True");
	}

	static String booleanToString(boolean val){
		return val?"true":"false";
	}

	static String getStringArrayAsJson(String name,const char* const* array,uint maxIndex){
		String result="\""+name+"\":[";

		for(uint8_t i=0;i<maxIndex;i++){
			result+="\"";
			result+=array[i];
			result+="\",";
		}

		result.setCharAt(result.length()-1, ']');

		return result;
	}

	static void printHeap(){
		Serial.print(FPSTR(MESSAGE_DEVICE_FREE_HEAP));
		Serial.println(ESP.getFreeHeap());
	}

	static boolean validateIP(String str){
		IPAddress ip=stringToIp(str);
		boolean result=str==ip.toString();

		return result;
	}

	static IPAddress stringToIp(String str){
		uint8_t pos=0;
		String tmp="";

		uint8_t ip[4];

		for(unsigned int i=0;i<=str.length();i++){
			if(str.charAt(i)=='.' || i==(str.length())){
				ip[pos]=tmp.toInt();
				pos++;
				tmp="";
			}else{
				tmp+=str[i];
			}
		}
		IPAddress result=IPAddress(ip[0],ip[1],ip[2],ip[3]);

		return result;
	}

	static String clearNlFromString(String str){
		String result=str;
		int index=str.lastIndexOf('\r');

		if(index){
			result=str.substring(0, index);
		}

		return result;
	}
};

#endif /* LIBRARIES_ESPSETTINGSBOX_ESPSETTINGSUTIL_H_ */

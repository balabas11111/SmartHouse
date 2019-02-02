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

class EspSettingsUtil {
public:
	EspSettingsUtil(){};
	virtual ~EspSettingsUtil(){};

	static boolean isFileExists(String fileName){
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
	}

	static String getSettingsFileFileName(String settingsName){
		String jsonFileName=FPSTR(ESPSETTINGSBOX_SETTINGS_PATH);
				jsonFileName+=settingsName;
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

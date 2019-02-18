/*
 * FileUtils.cpp
 *
 *  Created on: Feb 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <utils/FileUtils.h>
#include <ArduinoJson.h>

FileUtils::FileUtils(){}

FileUtils::~FileUtils(){}

boolean FileUtils::saveStringToFile(String fileName, String value) {
	File file = SPIFFS.open(fileName, "w");

	if(!file){
		file.close();
		return false;
	}

	size_t fileSize=file.print(value);
	file.close();

	return fileSize>0;
}

String FileUtils::loadStringFromFile(String fileName){
	if(!SPIFFS.exists(fileName)){
		return String(NULL);
	}

	File file = SPIFFS.open(fileName, "r");

	if(!file || file.size()==0){
		return String(NULL);
	}

	String data=file.readString();
	file.close();

	return data;
}

boolean FileUtils::saveJsonToFile(String fileName, JsonObject* obj) {
	File file = SPIFFS.open(fileName, "w");
	if(!file){
		return false;
	}
	return (obj->printTo(file));
}

boolean FileUtils::loadJsonFromFile(String fileName, JsonObject* obj) {
	File file = SPIFFS.open(fileName, "r");

	DynamicJsonBuffer buf;

	JsonObject& loaded=buf.parse(file);

	if(!file){
		return false;
	}
	return (loaded.printTo(obj)>0);
}

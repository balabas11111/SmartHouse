/*
 * FileUtils.cpp
 *
 *  Created on: Feb 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#include "FileUtils.h"
#include "JsonObjectUtil.h"
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

boolean FileUtils::exists(String fileName) {
	return SPIFFS.exists(fileName);
}

String FileUtils::loadStringFromFile(String fileName){
	if(!SPIFFS.exists(fileName)){
		return String(false);
	}

	File file = SPIFFS.open(fileName, "r");

	if(!file || file.size()==0){
		return String(false);
	}

	String data=file.readString();
	file.close();

	return data;
}

boolean FileUtils::saveJsonToFile(String fileName,JsonObject& obj) {
	File file = SPIFFS.open(fileName, "w");
	if(!file){
		return false;
	}
	return (obj.printTo(file));
}

boolean FileUtils::loadJsonFromFile(String fileName, JsonObject& obj) {
	Serial.print(FPSTR("Load from file"));
	Serial.print(fileName);
	File file = SPIFFS.open(fileName, "r");

	Serial.print(FPSTR(" size"));
	Serial.print(file.size());

	DynamicJsonBuffer buf;
	JsonObject& loaded=buf.parse(file);

	Serial.println(FPSTR(" buf loaded"));

	if(!file || !loaded.success()){
		return false;
	}

	JsonObjectUtil::print(loaded);

	Serial.println();
	Serial.println(FPSTR("...clone"));

	JsonObjectUtil::clone(obj, loaded, "root");

	JsonObjectUtil::print(obj);
	Serial.println();

	return obj.success() && obj.size()>0;
}

boolean FileUtils::existsAndHasSize(String fileName) {
	File file = SPIFFS.open(fileName, "r");

	return file && file.size()>0;
}

boolean FileUtils::existsAndHasSizeChar(const char* fileName) {
	File file = SPIFFS.open(fileName, "r");

	return file && file.size()>0;
}

bool FileUtils::init() {
	return SPIFFS.begin();
}

File FileUtils::getFile(String fileName,const char* mode) {
	return SPIFFS.open(fileName, mode);
}

File FileUtils::getFileChar(const char* fileName,const char* mode) {
	return SPIFFS.open(fileName, mode);
}

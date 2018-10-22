/*
 * ConfigStorage.cpp
 *
 *  Created on: Oct 22, 2018
 *      Author: Vitaliy_Vlasov
 */

#include <ConfigStorage.h>
#include "FS.h"
#include "ArduinoJson.h"

ConfigStorage::ConfigStorage() { construct(DEFAULT_FILE_NAME, true, true); }
ConfigStorage::ConfigStorage(const char* _filename) {construct(_filename, true, true);}
ConfigStorage::ConfigStorage(const char* _filename, boolean forceload) {construct(_filename, forceload, true);}
ConfigStorage::ConfigStorage(boolean forceload) {construct(DEFAULT_FILE_NAME, forceload, true);}
ConfigStorage::~ConfigStorage() {}

boolean ConfigStorage::initialize(boolean _init) {
	if(!initialized){

		if(_init){
			load();
		}

		initialized=init;
	}

	print();

	return initialized;
}

boolean ConfigStorage::load() {
	File file = SPIFFS.open(fileName, "r");

	boolean result=false;

	if(!file){
		Serial.println("-----FILE not exists. Default settings save");
		fillDefaultValues();
		save();
		file = SPIFFS.open(fileName, "r");
	}

	if (!file){
		Serial.println("No settings file exists");
	}else{
		size_t size = file.size();

		if ( size == 0 ) {
		  Serial.println("Empty settings file");
		} else {

			StaticJsonBuffer<1024> jsonBuffer;
			delay(1);

			std::unique_ptr<char[]> buf (new char[size]);
			file.readBytes(buf.get(), size);
			JsonObject& root = jsonBuffer.parseObject(buf.get());

			if (!root.success()) {
				Serial.println("Impossible to read JSON file");
			}else{
				Serial.println("Settings parsed");

				//putSettingsToMemory(root);
				for(uint16_t i=0;i<ITEMS_COUNT;i++){
					char* key=const_cast<char*>(configNames[i]);
					values[i]=const_cast<char*>(root[key].as<char*>());
				}

				result=true;
			}
		}

		file.close();
		delay(1);

	}
	return result;
}

boolean ConfigStorage::save() {
	StaticJsonBuffer<1024> jsonBuffer;

	JsonObject& root = jsonBuffer.createObject();
	for(uint16_t i=0;i<ITEMS_COUNT;i++){
		char* key=const_cast<char*>(configNames[i]);
		root[key]=values[i];
	}

	Serial.println("Save device settings to file");

	File settFile = SPIFFS.open(fileName, "w");

	Serial.println("begin save");
	root.printTo(settFile);
	settFile.close();
	Serial.println(" saved");

	delay(1);

	Serial.println("Device settings saved to file");

	return true;
}

void ConfigStorage::print(){
	printMemory();
	printSettingsFile();
}

void ConfigStorage::printMemory(){
	Serial.println("----------------Properties from memory------------");

	for(uint16_t i=0;i<ITEMS_COUNT;i++){
		char* key=const_cast<char*>(configNames[i]);
		char* val=const_cast<char*>(values[i]);
		Serial.print(key);
		Serial.print('=');
		Serial.print(val);
		Serial.println(';');
	}

	Serial.println("-------------------------------------------");
}

void ConfigStorage::printSettingsFile(){

	Serial.println("----------------direct file read------------");
	delay(1);
	File fileSet = SPIFFS.open(fileName, "r");
	if(fileSet){
		String str;
		int i=0;

		 while(fileSet.available()){
			 i++;

			 str=fileSet.readStringUntil('\n');
			 Serial.println(str);

		 }
		 fileSet.close();

	}else{
		 Serial.println("settings.txt MISSED");
	}
	delay(1);

	Serial.println("-------------------------------------------");
}

void ConfigStorage::fillDefaultValues() {
	for(uint16_t i=0;i<ITEMS_COUNT;i++){
		values[i]=const_cast<char*> ( defaultValues[i]);
	}

	Serial.println("Default values put to current values");
}

char* ConfigStorage::getAsJson() {
	StaticJsonBuffer<1024> jsonBuffer;

	JsonObject& root = jsonBuffer.createObject();
	for(uint16_t i=0;i<ITEMS_COUNT;i++){
		char* key=const_cast<char*>(configNames[i]);
		root[key]=values[i];
	}

	//char* print;
	String print;
	root.prettyPrintTo(print);

	return const_cast<char*>(print.c_str());
}

int16_t ConfigStorage::getKeyIndex(char* key){
	for(uint16_t i=0;i<ITEMS_COUNT;i++){
		if(strcmp ( values[i],key)==0){
			return i;
		}
	}

	return -1;
}

char* ConfigStorage::getAsJson(char* key) {
	int16_t ind=getKeyIndex(key);
	String result="{\""+String(key)+"\":\"";

	if(ind!=-1){
		result+=values[ind];
	}else{
		result+=MESSAGE_PROPERTY_NOT_FOUND;
	}

	result+="\"}";

	return const_cast<char*>(result.c_str());
}

void ConfigStorage::construct(const char* _fileName, boolean init,
		boolean initSpiff) {
	fileName=_fileName;

	if(initSpiff){
		spiffInitialized=SPIFFS.begin();
		Serial.println("File system status"+String(spiffInitialized));
	}

	if(init){
		load();
		initialized=init;
	}

}
/*
boolean ConfigStorage::putSettingsToMemory(JsonObject& root){
	for(uint16_t i=0;i<ITEMS_COUNT;i++){
		char* key=const_cast<char*>(configNames[i]);
		values[i]=const_cast<char*>(root[key].as<char*>());
	}

	return true;
}*/

/*
 * ConfigStorage.cpp
 *
 *  Created on: Oct 22, 2018
 *      Author: Vitaliy_Vlasov
 */

#include <ConfigStorage.h>
#include "FS.h"
#include "ArduinoJson.h"
#include "ctype.h"

ConfigStorage::ConfigStorage() { construct(String(FPSTR(DEFAULT_FILE_NAME)), false, false); }
ConfigStorage::ConfigStorage(const char* _filename) {construct(_filename, true, true);}
ConfigStorage::ConfigStorage(const char* _filename, boolean forceload) {construct(_filename, forceload, true);}
ConfigStorage::ConfigStorage(boolean forceload) {construct(DEFAULT_FILE_NAME, forceload, true);}
ConfigStorage::~ConfigStorage() {}

boolean ConfigStorage::initialize(boolean _init) {
	Serial.println("-----Start initialize configStorage-----");
	if(!initialized){

		if(_init){
			load();
		}
		initialized=_init;
	}

	print();

	return initialized;
}

boolean ConfigStorage::load() {
	Serial.print("Load file ");
	Serial.println(fileName);

	File file = SPIFFS.open(fileName, "r");

	boolean result=false;

	if(!file){
		Serial.println("-----FILE not exists. Default settings save");
		fillDefaultValues();
		printMemory();
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
					char* key=const_cast<char*>(keys[i]);
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
		//char* key=const_cast<char*>(configNames[i]);
		root[keys[i]]=values[i];
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
		//char* key=const_cast<char*>(keys[i]);
		//char* val=const_cast<char*>(values[i]);
		Serial.print(FPSTR(keys[i]));
		Serial.print('=');
		Serial.print(FPSTR(values[i]));
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
	Serial.println("---default values");

	Serial.print("---len=");
	Serial.println(sizeof(values));

	for(uint16_t i=0;i<ITEMS_COUNT;i++){
		//char * ptr = (char *) pgm_read_word (&defaultValues [i]);
		//FPSTR(defaultValues[i]);
		//values[i]=String(FPSTR(defaultValues[i])).c_str();
		Serial.print("i=");
		Serial.print(i);
		Serial.print(" def= ");
		Serial.print(FPSTR(defaultValues[i]));
		Serial.print(" dest= ");
		char* dest=new char[20];

		strcpy_P(dest, defaultValues[i]);
		Serial.print(dest);
		values[i]=dest;
		Serial.print(" values= ");
		Serial.println(values[i]);
		//values[i]=const_cast<char*> (String(FPSTR(values[i])).c_str());
		//Serial.println(String(FPSTR(values[i])));
	}

	Serial.println("Default values put to current values");
}

char* ConfigStorage::getAsJson() {
	StaticJsonBuffer<1024> jsonBuffer;

	JsonObject& root = jsonBuffer.createObject();
	for(uint16_t i=0;i<ITEMS_COUNT;i++){
		char* key=const_cast<char*>(keys[i]);
		root[key]=values[i];
	}

	//char* print;
	String print;
	root.prettyPrintTo(print);

	return const_cast<char*>(print.c_str());
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

int16_t ConfigStorage::getKeyIndex(char* key){
	for(uint16_t i=0;i<ITEMS_COUNT;i++){
		if(strcmp ( keys[i],key)==0){
			return i;
		}
	}

	return -1;
}

char* ConfigStorage::get(char* key) {
	uint16_t ind=getKeyIndex(key);

	return get(ind);
}

int ConfigStorage::getInt(char* key) {
	uint16_t ind=getKeyIndex(key);

	return getInt(ind);
}

char* ConfigStorage::get(uint16_t keyId) {
	if(keyId>=ITEMS_COUNT){
		//return -32768;
	}
	return values[keyId];
}

int ConfigStorage::getInt(uint16_t keyId) {
	if(keyId>=ITEMS_COUNT){
		//return -32768;
	}
	char* c =values[keyId];
	return atoi(c);
}

void ConfigStorage::set(char* key, char* val) {
	uint16_t keyId=getKeyIndex(key);
	set(keyId,val);
}

void ConfigStorage::setInt(char* key, int val) {
	uint16_t keyId=getKeyIndex(key);

	setInt(keyId,val);
}

void ConfigStorage::set(uint16_t keyId, char* val) {
	if(keyId<ITEMS_COUNT){
		values[keyId]=val;
	}
}

void ConfigStorage::setInt(uint16_t keyId, int val) {
	if(keyId<ITEMS_COUNT){
		char s[13];
		itoa (val, s, 10);

		values[keyId]=s;
	}
}

float ConfigStorage::getFloat(char* key) {
	int16_t keyId=getKeyIndex(key);

	return getFloat(keyId);
}


void ConfigStorage::setFloat(char* key, float val) {
	int16_t keyId=getKeyIndex(key);
	setFloat(keyId,val);
}

float ConfigStorage::getFloat(uint16_t keyId) {
	char* c=values[keyId];
	return atof(c);
}

void ConfigStorage::setFloat(uint16_t keyId, float val) {
	char s[15];
	dtostrf (val, 10, 2, s);
	values[keyId]=s;
}

uint16_t ConfigStorage::getUint16t(char* key) {
	uint16_t keyId=getKeyIndex(key);
	return getUint16t(keyId);
}

uint16_t ConfigStorage::getUint16t(uint16_t keyId) {
	char* c =values[keyId];
	return (uint16_t)atoi(c);
}

void ConfigStorage::construct(String _fileName, boolean init,
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

char* ConfigStorage::get(const char* key) {
	char* keyC=(char*) key;
	return get(keyC);
};
/*
boolean ConfigStorage::putSettingsToMemory(JsonObject& root){
	for(uint16_t i=0;i<ITEMS_COUNT;i++){
		char* key=const_cast<char*>(configNames[i]);
		values[i]=const_cast<char*>(root[key].as<char*>());
	}

	return true;
}*/

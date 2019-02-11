/*
 * EspSettingsBox.cpp
 *
 *  Created on: 2 февр. 2018 г.
 *      Author: Vitaliy
 */
#include "EspSettingsBox.h"
#ifdef ESP8266
#include "ESP8266WiFi.h"
#endif
#ifdef ESP32
#include "WiFi.h"
#include "SPIFFS.h"
#endif

#include "ArduinoJson.h"
#include "FS.h"
#include "ESP_Consts.h"
#include "projectConsts.h"
#include "EspSettingsUtil.h"

//true values
const char BOOLEAN_ON[]             PROGMEM ="on";
const char BOOLEAN_1[]              PROGMEM ="1";
const char BOOLEAN_true[]           PROGMEM ="true";

const char BOOLEAN_false[]          PROGMEM ="false";

//Initializable
	boolean EspSettingsBox::initialize(boolean _init){
		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_BEGIN_INITIALIZE));

		if(!initialized && _init){

			if(!spiffInitialized){
				initSpiff();
			}else{
				Serial.println(FPSTR("SPIFF already initialized"));
			}

			initExtraBoxes();

			initialized=_init;
		}

		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_LOADED));
		Serial.println(loaded);
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

		initialized=_init;

		return initialized;
	}

boolean EspSettingsBox::isSpiffInitialized(){
	return spiffInitialized;
}

void EspSettingsBox::initSpiff(){
	if(isSpiffInitialized()){
		return;
	}

	Serial.println();
	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_SYSTEM_BEGIN_INIT));
#ifdef ESP8266
	spiffInitialized=SPIFFS.begin();
#endif
#ifdef ESP32
	spiffInitialized=SPIFFS.begin(1, "/spiffs", 10);
#endif
	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_SYSTEM_STATUS));
	Serial.println(spiffInitialized);
}

void EspSettingsBox::printSpiffsInfo(){
#ifdef ESP8266
	FSInfo fs_info;
	SPIFFS.info(fs_info);
#endif

	Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SPIFFS_INFO));
	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_INITIALIZED_EQ));
	Serial.println(initialized);
	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_SPIFF_INIT_EQ));
	Serial.println(spiffInitialized);
#ifdef ESP8266
	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_BLOCK_SIZE));
	Serial.println(fs_info.blockSize);
	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_MAX_OPEN_FILES));
	Serial.println(fs_info.maxOpenFiles);
	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_MAX_PATH_LENGTH));
	Serial.println(fs_info.maxPathLength);
	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_PAGE_SIZE));
	Serial.println(fs_info.pageSize);
	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_TOTAL_BYTES));
	Serial.println(fs_info.totalBytes);
	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_USED_BYTES));
	Serial.println(fs_info.usedBytes);
	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
#endif
#ifdef ESP8266
			Dir dir = SPIFFS.openDir("/");
			while (dir.next()) {
			  String fileName = dir.fileName();
			  size_t fileSize = dir.fileSize();
			  Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_NAME_EQ));
			  Serial.print(fileName);
			  Serial.print(FPSTR(" "));
			  Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_SIZE_EQ));
			  Serial.println(fileSize);
			}
#endif
#ifdef ESP32
			listDir(SPIFFS, "/", 10);
#endif
	Serial.println("---------------------------------------");
}

#ifdef ESP32
void EspSettingsBox::listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
		  Serial.print(FPSTR("Listing directory: "));
		  Serial.println(dirname);

		  File root = fs.open(dirname);
		  if (!root) {
			  Serial.println(FPSTR("Failed to open directory"));
		    return;
		  }
		  if (!root.isDirectory()) {
			  Serial.println(FPSTR("Not a directory"));
		    return;
		  }

		  File file = root.openNextFile();
		  while (file) {
		    if (file.isDirectory()) {
		      Serial.print(FPSTR("  DIR : "));
		      Serial.println(file.name());
		      if (levels) {
		        listDir(fs, file.name(), levels - 1);
		      }
		    } else {
		      Serial.print(FPSTR("  FILE: "));
		      Serial.print(file.name());
		      Serial.print(FPSTR("  SIZE: "));
		      Serial.println(file.size());
		    }
		    file = root.openNextFile();
		  }
		}
#endif

void EspSettingsBox::construct(boolean forceLoad,boolean _initSpiff){
	this->_fileName=FPSTR(ESPSETTINGSBOX_SETTINGS_PATH);
	this->_fileName+=FPSTR(ESPSETTINGSBOX_SETTINGS_DEFAULT_FILE);

	if(_initSpiff){
		initSpiff();
	}

	if(forceLoad){
		init();
	}

}

String EspSettingsBox::getJson(){
String result="{\"name\":\"espSettingsBox\",\"kind\":\"all\",\
\"entity\": "+getExtraBoxJsonByKind(SETTINGS_KIND_all)+"}";
return result;
}

String EspSettingsBox::getJson(String page){
	if(!page || page==""){
		return getJson();
	}

return getExtraBoxJsonByKind(SETTINGS_KIND_all);
}

boolean EspSettingsBox::setSettingsValue(String fieldName, String fieldValue) {
	String startTag=FPSTR(ESPSETTINGSBOX_START_TAG);
	int startIndex=startTag.length();

	if(fieldName==FPSTR(MESSAGE_SERVER_ARG_PAGE)){
		return true;
	}

	if(!fieldName.startsWith(startTag)){
		return false;
	}else{
		fieldName=fieldName.substring(startIndex);
	}

	if(setSettingsValueIfExtra(fieldName,fieldValue)){
		return true;
	}

	return false;
}

int EspSettingsBox::deleteWebFiles() {
	return deleteFilesByPreffix(FPSTR(ESPSETTINGSBOX_DEFAULT_WEB_FOLDER));
}

int EspSettingsBox::deleteSettingsFiles() {
	return deleteFilesByPreffix(FPSTR(ESPSETTINGSBOX_SETTINGS_PATH));
}

int EspSettingsBox::deleteFilesByPreffix(String preffix){
	Serial.print(FPSTR("Delete files by preffix"));
	Serial.print(preffix);
	int count=0;
#ifdef ESP8266
	Dir dir = SPIFFS.openDir("/");

	while (dir.next()) {
		String fileName=dir.fileName();
#endif
#ifdef ESP32
	File dir = SPIFFS.open("/");
	if(!dir.isDirectory()){
		Serial.println(FPSTR("ROOT FOLDER IS NOT DIRECTORY"));
		return count;
	}

	dir.rewindDirectory();

	for (int cnt = 0; true; ++cnt) {
		File entry = dir.openNextFile();

		if (!entry)
		break;

		String fileName=entry.name();
#endif

		if(fileName.startsWith(preffix)){
			SPIFFS.remove(fileName);
			count++;

			Serial.print(FPSTR(ESPSETTINGSBOX_SETTINGS_REMOVED));
			Serial.println(fileName);
		}


	}

	Serial.print(FPSTR(" removed="));
	Serial.println(count);

	return count;
}

JsonArray& EspSettingsBox::getAbstractItems(JsonArray& items, uint8_t pageId) {
	return items;
}

JsonArray& EspSettingsBox::postAbstractItems(JsonArray& items, uint8_t pageId) {
	return items;
}

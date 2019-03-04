/*
 * FileUtils.cpp
 *
 *  Created on: Feb 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#include "HashPrint.h"
#include "FileUtils.h"
#include "JsonObjectUtil.h"
#include <ArduinoJson.h>

FileUtils::FileUtils(){}

FileUtils::~FileUtils(){}

bool FileUtils::saveStringToFile(String fileName, String value) {
	File file = SPIFFS.open(fileName, "w");

	if(!file){
		file.close();
		return false;
	}

	size_t fileSize=file.print(value);
	file.close();

	return fileSize>0;
}

bool FileUtils::exists(String fileName) {
	return SPIFFS.exists(fileName);
}

String FileUtils::loadStringFromFile(String fileName){
	if(!SPIFFS.exists(fileName)){
		return String(false);
	}

	File file = SPIFFS.open(fileName, "r");

	if(!file || file.size()==0){
		file.close();
		return String(false);
	}

	String data=file.readString();
	file.close();

	return data;
}

bool FileUtils::saveJsonToFile(String fileName,JsonObject& obj) {
	File file = SPIFFS.open(fileName, "w");
	if(!file){
		Serial.println(FPSTR("Not opened for Write"));
		file.close();
		return false;
	}
	size_t bytes = (obj.printTo(file)>0);

	if(bytes==0){Serial.println(FPSTR("Data was not saved to file"));}
	file.close();

	return bytes>0;
}

bool FileUtils::loadJsonFromFile(String fileName, JsonObject& obj) {
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

bool FileUtils::existsAndHasSize(String fileName) {
	File file = SPIFFS.open(fileName, "r");
	bool res=(file && file.size()>0);
	file.close();

	return res;
}

bool FileUtils::existsAndHasSizeChar(const char* fileName) {
	File file = SPIFFS.open(fileName, "r");
	bool res=(file && file.size()>0);
	file.close();

	return res;
}

bool FileUtils::init() {
	Serial.println(FPSTR("======================================"));
	Serial.print(FPSTR("Init SPIFFS status="));
	bool result=false;

	#ifdef ESP8266
		result= SPIFFS.begin();
	#endif
	#ifdef ESP32
		result= SPIFFS.begin(1, "/spiffs", 10);
	#endif
	Serial.println(result);

	return result;
}

File FileUtils::getFile(String fileName,const char* mode) {
	return SPIFFS.open(fileName, mode);
}

File FileUtils::getFileChar(const char* fileName,const char* mode) {
	return SPIFFS.open(fileName, mode);
}

unsigned int FileUtils::getCrc(const char* fileName) {
	File file = SPIFFS.open(fileName, FILE_MODE_READ);

	if(!file){
		Serial.print(FPSTR("Error open file"));
		Serial.println(fileName);
		return 0;
	}
	HashPrint hashPrint;
	uint8_t ibuffer[64];
	while (file.available() > 0)
	{
	  int len = file.read(ibuffer, 64);
	  hashPrint.write(ibuffer, len);
	}
	file.close();

	return hashPrint.hash();
}

unsigned int FileUtils::getCrc(JsonObject& obj) {
	HashPrint hashPrint;
	obj.printTo(hashPrint);
	return hashPrint.hash();
}

bool FileUtils::compareCrs(const char* fileName, JsonObject& obj) {
	unsigned int fCrc= getCrc(fileName);
	unsigned int oCrc= getCrc(obj);

	return fCrc!=0 && fCrc==oCrc;
}

void FileUtils::dir(){
	dir("/");
}

void FileUtils::dir(const char* path){
#ifdef ESP8266
	FSInfo fs_info;
	SPIFFS.info(fs_info);
#endif
	Serial.println(FPSTR("--------------------------------------"));
	Serial.println(FPSTR("     Files"));
	Serial.println(FPSTR("--------------------------------------"));
#ifdef ESP8266
	int count=0;
	Dir dir = SPIFFS.openDir(path);
	while (dir.next()) {
	  Serial.print(dir.fileName());
	  Serial.print(FPSTR("   "));
	  Serial.println(dir.fileSize());
	  count++;
	}
	Serial.println(FPSTR("--------------------------------------"));
	Serial.print(FPSTR(" total files="));
	Serial.print(count);
	Serial.print(FPSTR(" bytes="));
	Serial.print(fs_info.totalBytes);
	Serial.print(FPSTR(" used="));
	Serial.println(fs_info.usedBytes);
#endif
#ifdef ESP32
			listDir(SPIFFS, "/", 10);
#endif
	Serial.println(FPSTR("---------------------------------------"));
}

bool FileUtils::saveRootJson(const char* fileName,const char* rootPath,JsonObject& root) {
	Serial.print(FPSTR(" save "));
	//Serial.print(rootPath);
	//Serial.print(FPSTR("..."));

	bool res=FileUtils::saveJsonToFile(fileName, root.get<JsonObject>(rootPath));
	if(res){
		Serial.println(FPSTR(" OK."));
	}else{
		Serial.println(FPSTR(" ERROR"));
	}
	return res;
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

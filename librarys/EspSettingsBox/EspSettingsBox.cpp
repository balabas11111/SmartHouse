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

			loadSettingsJson();

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

void EspSettingsBox::loadSettingsJson(){

	Serial.print(FPSTR("LoadSettingsJson file="));
	Serial.println(_fileName);

	File file = SPIFFS.open(_fileName, "r");

	if(!file || file.size()==0){
		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_FILE_NOT_EXISTS));
		saveSettingsJson();
		file = SPIFFS.open(_fileName, "r");
	}

	  if (!file){
	    Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_NO_SETTINGS_FILE_EXISTS));
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
	    	  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_VALUE_PARSED));
	    	  root.printTo(Serial);
	    	  Serial.println();
	    	  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_READ_FROM_FILE_COMPLETE));

	    	  	isAccesPoint = EspSettingsUtil::stringToBoolean(root["iAp"].as<char*>()) ;
	    	  	Serial.print("isAccesPoint=");
	    	  	Serial.println(isAccesPoint);
	    	  	refreshInterval = root["rin"];
	    	  	settingsUser = root["sur"].as<char*>();
	    	  	settingsPass = root["sps"].as<char*>();
	    	  	accessUser = root["aur"].as<char*>();
	    	  	accessPass = root["aps"].as<char*>();
	    	  	ssidAP = root["sap"].as<char*>();
	    	  	//passwordAP = root["pap"].asString();
	    	  	ssid = root["sid"].as<char*>();
	    	  	password = root["pas"].as<char*>();

	    	  	staticIp=EspSettingsUtil::stringToBoolean(root["staticIp"].as<char*>());
	    	  	localIp=EspSettingsUtil::stringToIp(String(root["lip"].as<char*>()));
	    	  	apIp=EspSettingsUtil::stringToIp(String(root["aip"].as<char*>()));
	    	  	gateIp=EspSettingsUtil::stringToIp(String(root["gip"].as<char*>()));
	    	  	subnetIp=EspSettingsUtil::stringToIp(String(root["sip"].as<char*>()));
	    	  	dnsIp=EspSettingsUtil::stringToIp(String(root["dip"].as<char*>()));
	    	  	dnsIp2=EspSettingsUtil::stringToIp(String(root["dip2"].as<char*>()));

	    	  	Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SETTINGS_TO_MEMORY));
	    	  	String vals="";
	    	  	root.printTo(vals);
	    	  	Serial.println(vals);
	    	  	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

	    	  	loaded=true;
	      }
	    }
	    file.close();
	    delay(1);

	    //printVariablesToSerial();
	  }
}

void EspSettingsBox::saveSettingsJson(){
	Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SAVE_DEVICE_SETTINGS_TO_FILE));

		DynamicJsonBuffer jsonBuffer;
		//StaticJsonBuffer<1024> jsonBuffer;

		JsonObject& root = jsonBuffer.createObject();

		root["iAp"] = isAccesPoint;

		root["rin"] = refreshInterval;

		root["sur"] = settingsUser;
		root["sps"] = settingsPass;
		root["aur"] = accessUser;
		root["aps"] = accessPass;
		root["sap"] = ssidAP;
		//root["pap"] = passwordAP;
		root["sid"] = ssid;
		root["pas"] = password;

		root["staticIp"] = staticIp;
		root["lip"] = localIp.toString();
		root["aip"] = apIp.toString();
		root["gip"] = gateIp.toString();
		root["sip"] = subnetIp.toString();
		root["dip"] = dnsIp.toString();
		root["dip2"] = dnsIp2.toString();

		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SETTINGS_FROM_MEMORY));
		String vals="";
		root.printTo(vals);
		Serial.println(vals);
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

		Serial.print(FPSTR("file="));
		Serial.println(_fileName);

		File settFile = SPIFFS.open(_fileName, "w");

		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_BEGIN_SAVE));
		root.printTo(settFile);
		settFile.close();
		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_SAVED));

	delay(1);
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

if(page==FPSTR(PAGE_DEVICE)){

return
"{\"name\":\"espSettingsBox\",\"kind\":\"device\",\"items\":{\
\"refreshInterval\": \""+String(refreshInterval)+"\",\
\"accessUser\": \""+String(accessUser)+"\",\
\"accessPass\": \"*****\",\
\"settingsUser\": \""+String(settingsUser)+"\",\
\"settingsPass\": \"*****\"},\
\"entity\": "+getExtraBoxJsonByKind(page)+"}";
}
if(page==FPSTR(PAGE_NET)){

return
"{\"name\":\"espSettingsBox\",\"kind\":\"net\",\"items\":{\
\"isAccesPoint\": \""+String(isAccesPoint)+"\",\
\"ssidAP\": \""+ssidAP+"\",\
\"ssid\": \""+ssid+"\",\
\"password\": \"*****\",\
\"staticIp\": \""+String(staticIp)+"\",\
\"localIp\": \""+localIp.toString()+"\",\
\"apIp\": \""+apIp.toString()+"\",\
\"gateIp\": \""+gateIp.toString()+"\",\
\"subnetIp\": \""+subnetIp.toString()+"\",\
\"dnsIp\": \""+dnsIp.toString()+"\",\
\"dnsIp2\": \""+dnsIp2.toString()+"\"},\
\"entity\": "+getExtraBoxJsonByKind(page)+"}";
}
if(page==FPSTR(PAGE_PUBLISH)){

return
"{\"name\":\"espSettingsBox\",\"kind\":\"publish\",\
\"entity\": "+getExtraBoxJsonByKind(page)+"}";
}

return "{}";
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

	if(fieldName==FPSTR(ESBOX_refreshInterval)){
		long val=fieldValue.toInt();
		if(refreshInterval!=val){
			refreshInterval=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_accessUser)){
		if(accessUser!=fieldValue){
			accessUser=fieldValue;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_accessPass)){
		if(fieldValue!=FPSTR(ESBOX_STARS) && accessPass!=fieldValue){
			accessPass=fieldValue;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_accessPassConfirm)
		|| fieldName==FPSTR(ESBOX_settingsPassConfirm)){
		return true;
	}
	if(fieldName==FPSTR(ESBOX_settingsUser)){
		if(settingsUser!=fieldValue){
			settingsUser=fieldValue;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_settingsPass)){
		if(fieldValue!=FPSTR(ESBOX_STARS) && settingsPass!=fieldValue){
			settingsPass=fieldValue;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_isAccesPoint)){
		boolean val=EspSettingsUtil::stringToBoolean(fieldValue);
		if(isAccesPoint!=val){
			isAccesPoint=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_ssidAP)){
		if(ssidAP!=fieldValue){
			ssidAP=fieldValue;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_ssid)){
		if(ssid!=fieldValue){
			ssid=fieldValue;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_password)){
		if(fieldValue!=FPSTR(ESBOX_STARS) && password!=fieldValue){
			password=fieldValue;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_passwordConfirm)){
		return true;
	}
	if(fieldName==FPSTR(ESBOX_staticIp)){
		boolean val=EspSettingsUtil::stringToBoolean(fieldValue);
		if(staticIp!=val){
			staticIp=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_localIp)){
		IPAddress val=EspSettingsUtil::stringToIp(fieldValue);
		if(localIp!=val){
			localIp=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_apIp)){
		IPAddress val=EspSettingsUtil::stringToIp(fieldValue);
		if(apIp!=val){
			apIp=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_gateIp)){
		IPAddress val=EspSettingsUtil::stringToIp(fieldValue);
		if(gateIp!=val){
			gateIp=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_subnetIp)){
		IPAddress val=EspSettingsUtil::stringToIp(fieldValue);
		if(subnetIp!=val){
			subnetIp=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_dnsIp)){
		IPAddress val=EspSettingsUtil::stringToIp(fieldValue);
		if(dnsIp!=val){
			dnsIp=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_dnsIp2)){
		IPAddress val=EspSettingsUtil::stringToIp(fieldValue);
		if(dnsIp2!=val){
			dnsIp2=val;
			saveRequired=true;
		}
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

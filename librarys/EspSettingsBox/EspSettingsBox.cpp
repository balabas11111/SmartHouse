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
#include "AbstractItem.h"
#include "ESP_Consts.h"

//true values
const char BOOLEAN_ON[]             PROGMEM ="on";
const char BOOLEAN_1[]              PROGMEM ="1";
const char BOOLEAN_true[]           PROGMEM ="true";

const char BOOLEAN_false[]          PROGMEM ="false";

EspSettingsBox::EspSettingsBox(String deviceKind){
	construct(deviceKind,false,false);
}

EspSettingsBox::EspSettingsBox(String deviceKind,ESPExtraSettingsBox** boxes,uint8_t boxesCount){
	construct(deviceKind,false,false);
	this->extraBoxes=boxes;
	this->extraBoxesCount=boxesCount;
}

/*EspSettingsBox::EspSettingsBox(boolean forceLoad){
	construct(forceLoad,false);
}*/

EspSettingsBox::EspSettingsBox(String deviceKind,boolean forceLoad,boolean _initSpiff){
	construct(deviceKind,forceLoad,_initSpiff);
}

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

boolean EspSettingsBox::initExtraBoxes(){
	if(extraBoxesCount!=0){
		boolean result=true;
		Serial.println();
		Serial.println(FPSTR("----------------Init extra sett boxes---------------"));
		for(uint8_t i=0;i<extraBoxesCount;i++){
			result=extraBoxes[i]->init() && result;
			result=loadExtraBox(i) && result;
			extraBoxes[i]->printDetails();
		}

		Serial.print(FPSTR("Init extra sett boxes...result="));
		Serial.println(result);
		Serial.print(FPSTR("-----------------------------------------------------"));
		Serial.println();
		return result;
	}
	return false;
}

boolean EspSettingsBox::isSpiffInitialized(){
	return spiffInitialized;
}

void EspSettingsBox::loadSettingsJson(){

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

	    	    DeviceId = root["dId"].as<char*>();
	    	  	DeviceKind = root["DeviceKind"].as<char*>();
	    	  	DeviceDescription =root["DeviceDescription"].as<char*>();
	    	  	DeviceLocation=root["DeviceLocation"].as<char*>();
	    	  	isAccesPoint = stringToBoolean(root["iAp"].as<char*>()) ;
	    	  	Serial.print("isAccesPoint=");
	    	  	Serial.println(isAccesPoint);
	    	  	refreshInterval = root["rin"];
	    	  	displayAlvaysOn = root["dao"];
	    	  	displayAutochange = root["dac"];
	    	  	settingsUser = root["sur"].as<char*>();
	    	  	settingsPass = root["sps"].as<char*>();
	    	  	accessUser = root["aur"].as<char*>();
	    	  	accessPass = root["aps"].as<char*>();
	    	  	ssidAP = root["sap"].as<char*>();
	    	  	//passwordAP = root["pap"].asString();
	    	  	ssid = root["sid"].as<char*>();
	    	  	password = root["pas"].as<char*>();

	    	  	staticIp=stringToBoolean(root["staticIp"].as<char*>());
	    	  	localIp=stringToIp(String(root["lip"].as<char*>()));
	    	  	apIp=stringToIp(String(root["aip"].as<char*>()));
	    	  	gateIp=stringToIp(String(root["gip"].as<char*>()));
	    	  	subnetIp=stringToIp(String(root["sip"].as<char*>()));
	    	  	dnsIp=stringToIp(String(root["dip"].as<char*>()));
	    	  	dnsIp2=stringToIp(String(root["dip2"].as<char*>()));
	    	  	serverIp=stringToIp(String(root["serverip"].as<char*>()));

	    	  	isThingSpeakEnabled=root["ptTsEnabled"];
	    	  	postDataToTSInterval = root["pdTs"];
	    	  	thSkUsrKey=root["sPk"].as<char*>();
	    	  	thSkWKey = root["sWk"].as<char*>();
	    	  	thSkRKey = root["sRk"].as<char*>();
	    	  	thSkTKey = root["sTk"].as<char*>();
	    	  	thSkChId = root["sCi"];

	    	  	isMqttEnabled=stringToBoolean(root["isMqttEnabled"]);
	    	  	postDataToMqttInterval=root["postDataToMqttInterval"];
	    	  	mqtt_server=root["mqtt_server"].as<char*>();
	    	  	mqtt_port=root["mqtt_port"];
	    	  	mqtt_user=root["mqtt_user"].as<char*>();
	    	  	mqtt_pass=root["mqtt_pass"].as<char*>();

	    	  	isHttpPostEnabled=stringToBoolean(root["isHttpSendEnabled"]);
	    	  	postDataToHttpInterval=root["postDataToHttpInterval"];
	    	  	httpPostIp=stringToIp(String(root["httpPostIp"].as<char*>()));

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

	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_BEGIN_SAVE));

		DynamicJsonBuffer jsonBuffer;
		//StaticJsonBuffer<1024> jsonBuffer;

		JsonObject& root = jsonBuffer.createObject();

		root["dId"] = DeviceId;

		root["DeviceKind"] = DeviceKind;
		root["DeviceDescription"] =  DeviceDescription;
		root["DeviceLocation"] =  DeviceLocation;
		root["iAp"] = isAccesPoint;

		root["rin"] = refreshInterval;
		root["dao"] = displayAlvaysOn;
		root["dac"] = displayAutochange;

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
		root["serverip"] = serverIp.toString();

		root["ptTsEnabled"]=isThingSpeakEnabled;
		root["pdTs"] = postDataToTSInterval;
		root["sPk"] = thSkUsrKey;
		root["sWk"] = thSkWKey;
		root["sRk"] = thSkRKey;
		root["sCi"] = thSkChId;
		root["sTk"] = thSkTKey;

		root["isMqttEnabled"]=isMqttEnabled;
		root["postDataToMqttInterval"]=postDataToMqttInterval;
		root["mqtt_server"]=mqtt_server;
		root["mqtt_port"]=mqtt_port;
		root["mqtt_user"]=mqtt_user;
		root["mqtt_pass"]=mqtt_pass;

		root["isHttpSendEnabled"]=isHttpPostEnabled;
		root["postDataToHttpInterval"]=postDataToHttpInterval;
		root["httpPostIp"]=httpPostIp.toString();

		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SETTINGS_FROM_MEMORY));
		String vals="";
		root.printTo(vals);
		Serial.println(vals);
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SAVE_DEVICE_SETTINGS_TO_FILE));

		File settFile = SPIFFS.open(_fileName, "w");

		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_BEGIN_SAVE));
		root.printTo(settFile);
		settFile.close();
		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_SAVED));

		delay(1);

		Serial.println(FPSTR(MESSAGE_DONE));
	settFile.close();

	delay(1);

	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_SAVED));
	Serial.println(FPSTR(MESSAGE_DONE));
}

void EspSettingsBox::loadAbstractItemFromFile(AbstractItem* item){

	String fileName=getFileName(item);

	File file = SPIFFS.open(fileName, "r");

	if(!file){
		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_DEFAULT_VALUES_SAVED));
		saveAbstractItemToFile(item);
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
				//Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_VALUE_PARSED));
				//root.printTo(Serial);

				item->setDescr(root["descr"].as<char*>());

				uint8_t itemCountJson=root["itemCount"];

				if(item->getItemCount()>0 && itemCountJson>0){
					//JsonArray& arrayJson=root["items"].asArray();
					//Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_TOTAL_CHILDS_EQ));
					//Serial.println(itemCountJson);

					for(uint8_t i=0;i<itemCountJson;i++){

						uint8_t index=item->getChildItemIndexByName(root["items"][i]["name"]);

						item->setDescr(index, root["items"][i]["descr"]);
						item->setFieldId(index, root["items"][i]["fieldId"]);
						item->setMinVal(index, root["items"][i]["minVal"]);
						item->setMaxVal(index, root["items"][i]["maxVal"]);
						item->setQueue(index, root["items"][i]["queue"]);

						//Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_ITEM_SETTINGS_LOADED));
						//Serial.println(item->getJson(index));
					}
				}
				//-----------------------------------------
			}
		}
	  }

	  //Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}

void EspSettingsBox::saveAbstractItemToFile(AbstractItem* item){

	//String itemStr=item->getJson();
	String fileName=getFileName(item);

	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_ABSTRACT_ITEM_SAVE_BEGIN));
	Serial.print(fileName);

	File file = SPIFFS.open(fileName, "w");

	file.println(item->getJson());

	file.close();
	Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SAVED));

	delay(1);
}

void EspSettingsBox::initSpiff(){
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

void EspSettingsBox::construct(String deviceKind,boolean forceLoad,boolean _initSpiff){
	this->DeviceKind=deviceKind;
	this->_fileName=FPSTR(ESPSETTINGSBOX_SETTINGS_PATH);
	this->_fileName+=FPSTR(ESPSETTINGSBOX_SETTINGS_DEFAULT_FILE);

	if(_initSpiff){
		initSpiff();
	}

	if(forceLoad){
		init();
	}

}

boolean EspSettingsBox::validateIP(String str){
	IPAddress ip=stringToIp(str);
	boolean result=str==ip.toString();

	return result;
}

IPAddress EspSettingsBox::stringToIp(String str){
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

String EspSettingsBox::clearNlFromString(String str){
	String result=str;
	int index=str.lastIndexOf('\r');

	if(index){
		result=str.substring(0, index);
	}

	return result;
}

String EspSettingsBox::getFileName(AbstractItem* item){
	return getSettingsFileFileName(item->getName());
}

String EspSettingsBox::getThingSpeakChannelUrl(){
	String result=FPSTR(MESSAGE_THINGSPEAK_CURRENT_CHANNEL_URL);
			result+=String(thSkChId);
			result+=FPSTR(MESSAGE_THINGSPEAK_PRIVATE_SHOW);
	return result;
}

String EspSettingsBox::getSimpleJson(){
	String result="{\"name\":\"espSettingsBox\",\"itemCount\":\"8\",\"items\":[\
						{\"name\":\"deviceFirmWareVersion\",\"val\":\""+deviceFirmWareVersion+"\",\"descr\":\"Версия прошивки\"},\
						{\"name\":\"DeviceId\",\"val\":\""+DeviceId+"\",\"descr\":\"ID устройства\"},\
						{\"name\":\"DeviceKind\",\"val\":\""+DeviceKind+"\",\"descr\":\"Тип устройства\"},\
						{\"name\":\"DeviceDescription\",\"val\":\""+DeviceDescription+"\",\"descr\":\"Описание устройства\"},\
						{\"name\":\"DeviceLocation\",\"val\":\""+DeviceLocation+"\",\"descr\":\"Размещение устройства\"},\
						\
						{\"name\":\"thSkChId\",\"val\":\""+thSkChId+"\"},\
						{\"name\":\"currentLocalIp\",\"val\":\""+String(WiFi.localIP())+"\"},\
						{\"name\":\"thingSpeakChannelUrl\",\"val\":\""+getThingSpeakChannelUrl()+"\"},"
						+getExtraBoxJsonByKind(SETTINGS_KIND_all)+"],\
						\"DeviceId\":\""+DeviceId+"\",\
						\"DeviceDescription\":\""+DeviceDescription+"\",\
						\"DeviceLocation\":\""+DeviceLocation+"\",\
						\"thingSpeakChannelUrl\":\""+getThingSpeakChannelUrl()+"\"\
						}";
return result;
}

String EspSettingsBox::getJson(String page){

	String result="{}";

	if(page==FPSTR(SETTINGS_KIND_device)){
			result="{\"name\":\"espSettingsBox\",\"itemCount\":\"17\",\"items\":[\
						{\"name\":\"deviceFirmWareVersion\",\"val\":\""+deviceFirmWareVersion+"\"},\
						{\"name\":\"DeviceId\",\"val\":\""+DeviceId+"\"},\
						{\"name\":\"DeviceKind\",\"val\":\""+DeviceKind+"\"},\
						{\"name\":\"DeviceDescription\",\"val\":\""+DeviceDescription+"\"},\
						{\"name\":\"DeviceLocation\",\"val\":\""+DeviceLocation+"\"},\
						{\"name\":\"displayAlvaysOn\",\"val\":\""+String(displayAlvaysOn)+"\"},\
						{\"name\":\"displayAutochange\",\"val\":\""+String(displayAutochange)+"\"},\
						{\"name\":\"refreshInterval\",\"val\":\""+String(refreshInterval)+"\"},\
						{\"name\":\"accessUser\",\"val\":\""+String(accessUser)+"\"},\
						{\"name\":\"accessPass\",\"val\":\"*****\"},\
						{\"name\":\"settingsUser\",\"val\":\""+String(settingsUser)+"\"},\
						{\"name\":\"settingsPass\",\"val\":\"*****\"},"
						+getExtraBoxJsonByKind(page)+"]}";
	}
	if(page==FPSTR(SETTINGS_KIND_net)){
			result="{\"name\":\"espSettingsBox\",\"itemCount\":\"48\",\"items\":[\
						{\"name\":\"isAccesPoint\",\"val\":\""+String(isAccesPoint)+"\"},\
						{\"name\":\"ssidAP\",\"val\":\""+ssidAP+"\"},\
						{\"name\":\"ssid\",\"val\":\""+ssid+"\"},\
						{\"name\":\"password\",\"val\":\"*****\"},\
						{\"name\":\"staticIp\",\"val\":\""+String(staticIp)+"\"},\
						{\"name\":\"localIp\",\"val\":\""+localIp.toString()+"\"},\
						{\"name\":\"apIp\",\"val\":\""+apIp.toString()+"\"},\
						{\"name\":\"gateIp\",\"val\":\""+gateIp.toString()+"\"},\
						{\"name\":\"subnetIp\",\"val\":\""+subnetIp.toString()+"\"},\
						{\"name\":\"dnsIp\",\"val\":\""+dnsIp.toString()+"\"},\
						{\"name\":\"dnsIp2\",\"val\":\""+dnsIp2.toString()+"\"},\
						{\"name\":\"serverIp\",\"val\":\""+serverIp.toString()+"\","
						+getExtraBoxJsonByKind(page)+"]}";
	}
	if(page==FPSTR(SETTINGS_KIND_publish)){
			result="{\"name\":\"espSettingsBox\",\"itemCount\":\"48\",\"items\":[\
					{\"name\":\"isThingSpeakEnabled\",\"val\":\""+String(isThingSpeakEnabled)+"\"},\
					{\"name\":\"postDataToTSInterval\",\"val\":\""+String(postDataToTSInterval)+"\"},\
					{\"name\":\"thSkUsrKey\",\"val\":\""+thSkUsrKey+"\"},\
					{\"name\":\"thSkWKey\",\"val\":\""+thSkWKey+"\"},\
					{\"name\":\"thSkRKey\",\"val\":\""+thSkRKey+"\"},\
					{\"name\":\"thSkChId\",\"val\":\""+String(thSkChId)+"\"},\
					{\"name\":\"thSkTKey\",\"val\":\""+thSkTKey+"\"},\
					{\"name\":\"isMqttEnabled\",\"val\":\""+String(isMqttEnabled)+"\"},\
					{\"name\":\"postDataToMqttInterval\",\"val\":\""+String(postDataToMqttInterval)+"\"},\
					{\"name\":\"mqtt_server\",\"val\":\""+mqtt_server+"\"},\
					{\"name\":\"mqtt_user\",\"val\":\""+mqtt_user+"\"},\
					{\"name\":\"mqtt_pass\",\"val\":\"*****\"},\
					{\"name\":\"mqtt_port\",\"val\":\""+String(mqtt_port)+"\"},\
					{\"name\":\"isHttpPostEnabled\",\"val\":\""+String(isHttpPostEnabled)+"\"},\
					{\"name\":\"postDataToHttpInterval\",\"val\":\""+String(postDataToHttpInterval)+"\"},\
					{\"name\":\"httpPostIp\",\"val\":\""+httpPostIp.toString()+"\"},\
					{\"name\":\"currentLocalIp\",\"val\":\""+String(WiFi.localIP())+"\"},\
					{\"name\":\"thingSpeakChannelUrl\",\"val\":\"https://thingspeak.com/channels/"+thSkChId+"/private_show\","
						+getExtraBoxJsonByKind(page)+"]}";

			/*
			 	 	{\"name\":\"thSkWManageKey\",\"val\":\""+thSkWManageKey+"\"},\
					{\"name\":\"thSkRManageKey\",\"val\":\""+thSkRManageKey+"\"},\
					{\"name\":\"thSkManageChId\",\"val\":\""+thSkManageChId+"\"},\
					{\"name\":\"sendItemsToBaseQUeue\",\"val\":\""+String(sendItemsToBaseQUeue)+"\"},\
					{\"name\":\"mqtt_TStopic\",\"val\":\""+mqtt_TStopic+"\"},\
			 */
	}

	return result;
}

boolean EspSettingsBox::setSettingsValueIfExtra(String fieldName, String fieldValue){
	if(!hasExtraBoxes()){
		return false;
	}

	for(uint8_t i=0;i<extraBoxesCount;i++){
		int keyIndex=extraBoxes[i]->isTargetOfSettingsValue(fieldName);

		if(keyIndex>-1){
			extraBoxes[i]->setValue(keyIndex, fieldValue);
			return true;
		}
	}

	return false;
}

boolean EspSettingsBox::setSettingsValue(String fieldName, String fieldValue) {
	String startTag=FPSTR(ESPSETTINGSBOX_START_TAG);
	int startIndex=startTag.length();

	Serial.print(fieldName);
	Serial.print(FPSTR(MESSAGE_EQUALS));
	Serial.print(fieldValue);
	Serial.print(FPSTR(MESSAGE_OPEN_BRACE));
	Serial.print(fieldName.substring(startIndex));
	Serial.println(FPSTR(MESSAGE_CLOSE_BRACE));

	if(!fieldName.startsWith(startTag)){
		return false;
	}else{
		fieldName=fieldName.substring(startIndex);
	}

	if(setSettingsValueIfExtra(fieldName,fieldValue)){
		return true;
	}

	if(fieldName==FPSTR(ESBOX_deviceFirmWareVersion)){
		deviceFirmWareVersion=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_DeviceId)){
		DeviceId=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_DeviceKind)){
		DeviceKind=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_DeviceDescription)){
		DeviceDescription=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_DeviceLocation)){
		DeviceLocation=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_displayAlvaysOn)){
		displayAlvaysOn=stringToBoolean(fieldValue);
		return true;
	}
	if(fieldName==FPSTR(ESBOX_displayAutochange)){
		displayAutochange=fieldValue.toInt();
		return true;
	}
	if(fieldName==FPSTR(ESBOX_refreshInterval)){
		refreshInterval=fieldValue.toInt();
		return true;
	}
	if(fieldName==FPSTR(ESBOX_accessUser)){
		accessUser=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_accessPass) && fieldValue!=FPSTR(ESBOX_STARS)){
		accessPass=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_settingsUser)){
		settingsUser=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_settingsPass) && fieldValue!=FPSTR(ESBOX_STARS)){
		settingsPass=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_isAccesPoint)){
		isAccesPoint=stringToBoolean(fieldValue);
		return true;
	}
	if(fieldName==FPSTR(ESBOX_ssidAP)){
		ssidAP=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_ssid)){
		ssid=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_password) && fieldValue!=FPSTR(ESBOX_STARS)){
		password=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_staticIp)){
		staticIp=stringToIp(fieldValue);
		return true;
	}
	if(fieldName==FPSTR(ESBOX_localIp)){
		localIp=stringToIp(fieldValue);
		return true;
	}
	if(fieldName==FPSTR(ESBOX_apIp)){
		apIp=stringToIp(fieldValue);
		return true;
	}
	if(fieldName==FPSTR(ESBOX_gateIp)){
		gateIp=stringToIp(fieldValue);
		return true;
	}
	if(fieldName==FPSTR(ESBOX_subnetIp)){
		subnetIp=stringToIp(fieldValue);
		return true;
	}
	if(fieldName==FPSTR(ESBOX_dnsIp)){
		dnsIp=stringToIp(fieldValue);
		return true;
	}
	if(fieldName==FPSTR(ESBOX_dnsIp2)){
		dnsIp2=stringToIp(fieldValue);
		return true;
	}
	if(fieldName==FPSTR(ESBOX_serverIp)){
		serverIp=stringToIp(fieldValue);
		return true;
	}
	if(fieldName==FPSTR(ESBOX_isThingSpeakEnabled)){
		isThingSpeakEnabled=stringToBoolean(fieldValue);
		return true;
	}
	if(fieldName==FPSTR(ESBOX_postDataToTSInterval)){
		postDataToTSInterval=fieldValue.toInt();
		return true;
	}
	if(fieldName==FPSTR(ESBOX_thSkUsrKey)){
		thSkUsrKey=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_thSkWKey)){
		thSkWKey=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_thSkRKey)){
		thSkRKey=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_thSkChId)){
		thSkChId=fieldValue.toInt();
		return true;
	}
	if(fieldName==FPSTR(ESBOX_thSkTKey)){
		thSkTKey=fieldValue;
		return true;
	}

	if(fieldName==FPSTR(ESBOX_isMqttEnabled)){
		isMqttEnabled=stringToBoolean(fieldValue);
		return true;
	}
	if(fieldName==FPSTR(ESBOX_postDataToMqttInterval)){
		postDataToMqttInterval=fieldValue.toInt();
		return true;
	}
	if(fieldName==FPSTR(ESBOX_mqtt_server)){
		mqtt_server=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_mqtt_user)){
		mqtt_user=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_mqtt_pass) && fieldValue!=FPSTR(ESBOX_STARS)){
		mqtt_pass=fieldValue;
		return true;
	}
	if(fieldName==FPSTR(ESBOX_mqtt_port)){
		mqtt_port=fieldValue.toInt();
		return true;
	}
	if(fieldName==FPSTR(ESBOX_isHttpPostEnabled)){
		isHttpPostEnabled=stringToBoolean(fieldValue);
		return true;
	}
	if(fieldName==FPSTR(ESBOX_postDataToHttpInterval)){
		postDataToHttpInterval=fieldValue.toInt();
		return true;
	}
	if(fieldName==FPSTR(ESBOX_httpPostIp)){
		httpPostIp=stringToIp(fieldValue);
		return true;
	}
	return false;
}

int EspSettingsBox::deleteSettingsFiles() {
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

		if(fileName.startsWith(FPSTR(ESPSETTINGSBOX_SETTINGS_PATH))){
			SPIFFS.remove(fileName);
			count++;

			Serial.print(FPSTR(ESPSETTINGSBOX_SETTINGS_REMOVED));
			Serial.println(fileName);
		}


	}

	return count;
}

boolean EspSettingsBox::saveThingSpeakChannelCreation(String response/*,
		boolean manageChannel*/) {

		Serial.println(FPSTR(ESPSETTINGSBOX_THINGSPEAK_PARSE_CHCREATION));
		Serial.print(FPSTR(ESPSETTINGSBOX_THINGSPEAK_CHANNEL_JSON));
		Serial.println(response);

		DynamicJsonBuffer jsonBuffer;

		JsonObject& root = jsonBuffer.parseObject(response);

		if (!root.success()) {
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
			return false;
		} else {
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_VALUE_PARSED));
			root.printTo(Serial);

			int channelId=root["id"];
			String flag1=root["api_keys"][0]["write_flag"];

			flag1.toLowerCase();
			uint8_t writeKeyFlag=0;
			uint8_t readKeyFlag=1;

			if(flag1!="true"){
				writeKeyFlag=1;
				readKeyFlag=0;
			}
			String writeKey=root["api_keys"][writeKeyFlag]["api_key"];
			String readKey=root["api_keys"][readKeyFlag]["api_key"];

			Serial.print(FPSTR(ESPSETTINGSBOX_THINGSPEAK_CHANNELID));
			Serial.println(channelId);
			Serial.print(FPSTR(ESPSETTINGSBOX_THINGSPEAK_WRITEKEY));
			Serial.println(writeKey);
			Serial.print(FPSTR(ESPSETTINGSBOX_THINGSPEAK_READKEY));
			Serial.println(readKey);

				if(channelId>0 && writeKey!="" && readKey!=""){
					/*if(manageChannel){
						Serial.println(FPSTR(ESPSETTINGSBOX_THINGSPEAK_UPDATE_MANAGECHANNEL));
						thSkManageChId=channelId;
						thSkWManageKey=writeKey;
						thSkRManageKey=readKey;
					}else{*/
						Serial.println(FPSTR(ESPSETTINGSBOX_THINGSPEAK_UPDATE_CHANNEL));
						thSkChId=channelId;
						thSkWKey=writeKey;
						thSkRKey=readKey;
					//}
					//mqtt_TStopic="channels/"+String(thSkChId)+"/subscribe/json/"+thSkRKey;

					saveSettingsJson();
				}
			}

		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

		return true;
}

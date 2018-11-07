/*
 * EspSettingsBox.cpp
 *
 *  Created on: 2 февр. 2018 г.
 *      Author: Vitaliy
 */
#include "EspSettingsBox.h"
#include "ESP8266WiFi.h"
#include "ArduinoJson.h"
#include "FS.h"
#include "AbstractItem.h"
#include "ESP_Consts.h"

EspSettingsBox::EspSettingsBox(){
	construct("",false,false);
}

EspSettingsBox::EspSettingsBox(boolean forceLoad){
	construct("",forceLoad,false);
}

EspSettingsBox::EspSettingsBox(String extValuesFileName,boolean forceLoad){
	construct(extValuesFileName,forceLoad,false);
}

EspSettingsBox::EspSettingsBox(String extValuesFileName,boolean forceLoad,boolean _initSpiff){
	construct(extValuesFileName,forceLoad,_initSpiff);
}

//Initializable
	boolean EspSettingsBox::initialize(boolean _init){
		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_BEGIN_INITIALIZE));

		if(!initialized && _init){
			loadSettingsJson();
			initialized=_init;
		}else{
			if(initialized)
				Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ALREADY_INITIALIZE));
		}

		return initialized;
	}

boolean EspSettingsBox::isSpiffInitialized(){
	return spiffInitialized;
}

void EspSettingsBox::saveSettingsJson(){
	Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SAVE_DEVICE_SETTINGS_TO_FILE));

	File settFile = SPIFFS.open(_fileName, "w");

	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_BEGIN_SAVE));
	getSettingsFromMemory(false).printTo(settFile);
	settFile.close();

	delay(1);

	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_SAVED));
	Serial.println(FPSTR(MESSAGE_DONE));
}

void EspSettingsBox::loadSettingsJson(){

	//loadExternalFile();

	File file = SPIFFS.open(_fileName, "r");

	if(!file){
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

	        putSettingsToMemory(root);

	      }
	    }
	    file.close();
	    delay(1);

	    //printVariablesToSerial();
	  }
}


JsonObject& EspSettingsBox::getSettingsFromMemory(boolean doSave){
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

	root["bOnA"] = beepOnAlert;
	root["AlSAs"]=alarmSendNotifAlertStart;
	root["AlSn"]=alarmSendNotifAlertStop;
	root["AlPs"]=alarmPlaySound;
	root["AlSi"]=alamSoundInterval;

	root["ptTsEnabled"]=isThingSpeakEnabled;
	root["pdTs"] = postDataToTSInterval;
	root["sPk"] = thSkUsrKey;
	root["sWk"] = thSkWKey;
	root["sRk"] = thSkRKey;
	root["sCi"] = thSkChId;
	root["sTk"] = thSkTKey;
	//root["AlNi"]=alamNotificationInterval;

	root["isMqttEnabled"]=isMqttEnabled;
	root["mqtt_server"]=mqtt_server;
	root["mqtt_port"]=mqtt_port;
	root["mqtt_user"]=mqtt_user;
	root["mqtt_pass"]=mqtt_pass;
	root["mqtt_topic"]=mqtt_topic;

	root["isHttpSendEnabled"]=isHttpPostEnabled;
	root["httpPostIp"]=httpPostIp.toString();

	root["ntpEnabled"]=ntpEnabled;
	root["NTP_poolServerName"]=NTP_poolServerName;
	root["NTP_timeOffset"]=NTP_timeOffset;
	root["NTP_timeTriggerInterval"]=NTP_timeTriggerInterval;
	root["NTP_updateInterval"]=NTP_updateInterval;


	Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SETTINGS_FROM_MEMORY));
	String vals="";
	root.printTo(vals);
	Serial.println(vals);
	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

	if(doSave){
		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SAVE_DEVICE_SETTINGS_TO_FILE));

		File settFile = SPIFFS.open(_fileName, "w");

		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_BEGIN_SAVE));
		root.printTo(settFile);
		settFile.close();
		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_SAVED));

		delay(1);

		Serial.println(FPSTR(MESSAGE_DONE));
	}

	return root;
}

boolean EspSettingsBox::putSettingsToMemory(JsonObject& root){
	DeviceId = root["dId"].as<char*>();
	DeviceKind = root["DeviceKind"].as<char*>();
	DeviceDescription =root["DeviceDescription"].as<char*>();
	DeviceLocation=root["DeviceLocation"].as<char*>();
	isAccesPoint = isTrue(root["iAp"].as<char*>()) ;
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

	staticIp=isTrue(root["staticIp"].as<char*>());
	localIp=stringToIp(String(root["lip"].as<char*>()));
	apIp=stringToIp(String(root["aip"].as<char*>()));
	gateIp=stringToIp(String(root["gip"].as<char*>()));
	subnetIp=stringToIp(String(root["sip"].as<char*>()));
	dnsIp=stringToIp(String(root["dip"].as<char*>()));
	dnsIp2=stringToIp(String(root["dip2"].as<char*>()));
	serverIp=stringToIp(String(root["serverip"].as<char*>()));

	beepOnAlert=root["bOnA"];

	isThingSpeakEnabled=root["ptTsEnabled"];
	postDataToTSInterval = root["pdTs"];
	thSkUsrKey=root["sPk"].as<char*>();
	thSkWKey = root["sWk"].as<char*>();
	thSkRKey = root["sRk"].as<char*>();
	thSkTKey = root["sTk"].as<char*>();
	thSkChId = root["sCi"];

	isMqttEnabled=isTrue(root["isMqttEnabled"]);
	mqtt_server=root["mqtt_server"].as<char*>();
	mqtt_port=root["mqtt_port"];
	mqtt_user=root["mqtt_user"].as<char*>();
	mqtt_pass=root["mqtt_pass"].as<char*>();
	mqtt_topic=root["mqtt_topic"].as<char*>();

	isHttpPostEnabled=isTrue(root["isHttpSendEnabled"]);
	httpPostIp=stringToIp(String(root["httpPostIp"].as<char*>()));

	alarmSendNotifAlertStart=isTrue(root["AlSAs"].as<char*>());
	alarmSendNotifAlertStop=isTrue(root["AlSn"].as<char*>());
	alarmPlaySound=isTrue(root["AlPs"].as<char*>());
	alamSoundInterval=root["AlSi"];
	//alamNotificationInterval=root["AlNi"];

	ntpEnabled=isTrue(root["ntpEnabled"]);
	NTP_poolServerName=root["NTP_poolServerName"].as<char*>();
	NTP_timeOffset=root["NTP_timeOffset"];
	NTP_timeTriggerInterval=root["NTP_timeTriggerInterval"];
	NTP_updateInterval=root["NTP_updateInterval"];

	Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SETTINGS_TO_MEMORY));
	String vals="";
	root.printTo(vals);
	Serial.println(vals);
	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

	return true;
}

void EspSettingsBox::printSettingsFile(){
		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_PRINT_SETTINGS_FILE));
		//loadExternalFile();
		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_CURRENT_MEMORY_STATE));
		getSettingsFromMemory();
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		File file = SPIFFS.open(_fileName, "r");

		if(!file){
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_DEFAULT_VALUES_SAVED));
			saveSettingsJson();
			file = SPIFFS.open(_fileName, "r");
		}
		/*
		  if (!file){
		    Serial.println("No settings file exists");
		  } else {
		    size_t size = file.size();
		    if ( size == 0 ) {
		      Serial.println("Empty settings file");
		    } else {
		    	Serial.println("Begin json parse");
		    	StaticJsonBuffer<1024> jsonBuffer;
		    	delay(1);

		      std::unique_ptr<char[]> buf (new char[size]);
		      file.readBytes(buf.get(), size);
		      JsonObject& root = jsonBuffer.parseObject(buf.get());

		      if (!root.success()) {
		        Serial.println("Impossible to read JSON file");
		      } else {
		        Serial.println("Settings parsed");

		        putSettingsToMemory(root);
		      }
		    }

		    file.close();
		  }
*/
		    Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_DIRECT_FILE_READ));
		    File fileSet = SPIFFS.open(_fileName, "r");
		    	if(fileSet){
		    		String str;
		    		int i=0;

		    		 while(fileSet.available()){
		    			 i++;
		    			 //Serial.print(i);
		    			 str=fileSet.readStringUntil('\n');
		    			 Serial.println(str);
		    			 //Serial.println("---");
		    		 }
		    		 file.close();

		    		 Serial.print(_fileName);
		    		 Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_EXISTS));
		    	}else{
		    		Serial.print(_fileName);
		    		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_MISSED));
		    	}
		    delay(1);

		    //printVariablesToSerial();
		    Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}

void EspSettingsBox::printSpiffsInfo(){
	FSInfo fs_info;
	SPIFFS.info(fs_info);

	Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SPIFFS_INFO));
	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_INITIALIZED_EQ));
	Serial.println(initialized);
	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_SPIFF_INIT_EQ));
	Serial.println(spiffInitialized);
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
			#ifdef ESP8266
			Dir dir = SPIFFS.openDir("/");
			while (dir.next()) {
			  String fileName = dir.fileName();
			  size_t fileSize = dir.fileSize();
			  Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_NAME_EQ));
			  Serial.print(fileName);
			  Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_SIZE_EQ));
			  Serial.println(fileSize);
			}
		#else
			listDir(SPIFFS, "/", 0);
		#endif
	Serial.println("---------------------------------------");
}

void EspSettingsBox::construct(String extValuesFileName,boolean forceLoad,boolean _initSpiff){
	_fileName=FPSTR(ESPSETTINGSBOX_SETTINGS_PATH);
	_fileName+=FPSTR(ESPSETTINGSBOX_SETTINGS_DEFAULT_FILE);
	_extFileName=extValuesFileName;

	if(_initSpiff){
		initSpiff();
	}

	if(forceLoad){
		loadSettingsJson();
	}

	initialized=true;
}

void EspSettingsBox::initSpiff(){
	spiffInitialized=SPIFFS.begin();

	Serial.println("File system status"+String(spiffInitialized));
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

void EspSettingsBox::loadExternalFile(){
	File file = SPIFFS.open(_fileName, "r");

		if(!file){
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_DEFAULT_VALUES_SAVED));
			saveSettingsJson();
			file = SPIFFS.open(_fileName, "r");
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
				  //JsonObject& root = jsonBuffer.parseObject(buf.get());

				  //extRoot=&jsonBuffer.parseObject(buf.get());

				  if (!extRoot->success()) {
					  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
				  } else {
					  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_VALUE_PARSED));
					extRoot->printTo(Serial);
				  }
		    }
		  }
}

String EspSettingsBox::getSettingsAsJsonString(){
	/*String result="";

	getSettingsFromMemory().prettyPrintTo(result);

	return result;
	*/
	return "";
}

boolean EspSettingsBox::isTrue(String str){
	String tmp=String(str);

	return (tmp.toInt()==1) ;
}

String EspSettingsBox::getSimpleJson(){
	String result="{\"name\":\"espSettingsBox\",\"childCount\":\"8\",\"items\":[\
						{\"name\":\"deviceFirmWareVersion\",\"val\":\""+deviceFirmWareVersion+"\",\
						{\"name\":\"DeviceId\",\"val\":\""+DeviceId+"\",\"label\":\"ID устройства\"},\
						{\"name\":\"DeviceKind\",\"val\":\""+DeviceKind+"\",\"label\":\"Тип устройства\"},\
						{\"name\":\"DeviceDescription\",\"val\":\""+DeviceDescription+"\"},\
						{\"name\":\"DeviceLocation\",\"val\":\""+DeviceLocation+"\"},\
						\
						{\"name\":\"thSkChId\",\"val\":\""+thSkChId+"\"},\
						{\"name\":\"currentLocalIp\",\"val\":\""+String(WiFi.localIP())+"\"}\
						{\"name\":\"thinkSpeakChannelUrl\",\"val\":\"https://thingspeak.com/channels/"+thSkChId+"/private_show\"}]}";
		return result;


return result;
}

String EspSettingsBox::getJson(){

	String result="{\"name\":\"espSettingsBox\",\"val\":\"48\",\"items\":[\
					{\"name\":\"deviceFirmWareVersion\",\"val\":\""+deviceFirmWareVersion+"\"},\
					{\"name\":\"DeviceId\",\"val\":\""+DeviceId+"\"},\
					{\"name\":\"DeviceKind\",\"val\":\""+DeviceKind+"\"},\
					{\"name\":\"DeviceDescription\",\"val\":\""+DeviceDescription+"\"},\
					{\"name\":\"DeviceLocation\",\"val\":\""+DeviceLocation+"\"},\
					\
					{\"name\":\"displayAlvaysOn\",\"val\":\""+String(displayAlvaysOn)+"\"},\
					{\"name\":\"displayAutochange\",\"val\":\""+String(displayAutochange)+"\"},\
					{\"name\":\"refreshInterval\",\"val\":\""+String(refreshInterval)+"\"},\
					\
					{\"name\":\"accessUser\",\"val\":\""+String(accessUser)+"\"},\
					{\"name\":\"accessPass\",\"val\":\"******\"},\
					{\"name\":\"settingsUser\",\"val\":\""+String(settingsUser)+"\"},\
					{\"name\":\"settingsPass\",\"val\":\"******\"},\
					\
					{\"name\":\"isAccesPoint\",\"val\":\""+String(isAccesPoint)+"\"},\
					{\"name\":\"ssidAP\",\"val\":\""+ssidAP+"\"},\
					{\"name\":\"ssid\",\"val\":\""+ssid+"\"},\
					{\"name\":\"password\",\"val\":\"*****\"},\
					{\"name\":\"serverPort\",\"val\":\""+String(serverPort)+"\"},\
					{\"name\":\"staticIp\",\"val\":\""+String(staticIp)+"\"},\
					{\"name\":\"localIp\",\"val\":\""+String(localIp)+"\"},\
					{\"name\":\"apIp\",\"val\":\""+String(apIp)+"\"},\
					{\"name\":\"gateIp\",\"val\":\""+String(gateIp)+"\"},\
					{\"name\":\"subnetIp\",\"val\":\""+String(subnetIp)+"\"},\
					{\"name\":\"dnsIp\",\"val\":\""+String(dnsIp)+"\"},\
					{\"name\":\"dnsIp2\",\"val\":\""+String(dnsIp2)+"\"},\
					{\"name\":\"serverIp\",\"val\":\""+String(serverIp)+"\"},\
					\
					{\"name\":\"isThingSpeakEnabled\",\"val\":\""+String(isThingSpeakEnabled)+"\"},\
					{\"name\":\"postDataToTSInterval\",\"val\":\""+String(postDataToTSInterval)+"\"},\
					{\"name\":\"thSkUsrKey\",\"val\":\""+thSkUsrKey+"\"},\
					{\"name\":\"thSkWKey\",\"val\":\""+thSkWKey+"\"},\
					{\"name\":\"thSkRKey\",\"val\":\""+thSkRKey+"\"},\
					{\"name\":\"thSkChId\",\"val\":\""+String(thSkChId)+"\"},\
					{\"name\":\"thSkTKey\",\"val\":\""+thSkTKey+"\"},\
					\
					{\"name\":\"isMqttEnabled\",\"val\":\""+String(isMqttEnabled)+"\"},\
					{\"name\":\"mqtt_server\",\"val\":\""+mqtt_server+"\"},\
					{\"name\":\"mqtt_user\",\"val\":\""+mqtt_user+"\"},\
					{\"name\":\"mqtt_pass\",\"val\":\"*****\"},\
					{\"name\":\"mqtt_topic\",\"val\":\""+mqtt_topic+"\"},\
					{\"name\":\"mqtt_port\",\"val\":\""+String(mqtt_port)+"\"},\
					\
					{\"name\":\"isHttpPostEnabled\",\"val\":\""+String(isHttpPostEnabled)+"\"},\
					{\"name\":\"httpPostIp\",\"val\":\""+String(httpPostIp)+"\"},\
					\
					{\"name\":\"ntpEnabled\",\"val\":\""+String(ntpEnabled)+"\"},\
					{\"name\":\"NTP_poolServerName\",\"val\":\""+String(NTP_poolServerName)+"\"},\
					{\"name\":\"NTP_timeOffset\",\"val\":\""+NTP_timeOffset+"\"},\
					{\"name\":\"NTP_updateInterval\",\"val\":\""+NTP_updateInterval+"\"},\
					{\"name\":\"NTP_timeTriggerInterval\",\"val\":\""+NTP_timeTriggerInterval+"\"},\
					\
					{\"name\":\"thSkChId\",\"val\":\""+thSkChId+"\"},\
					{\"name\":\"currentLocalIp\",\"val\":\""+String(WiFi.localIP())+"\"}\
					{\"name\":\"thinkSpeakChannelUrl\",\"val\":\"https://thingspeak.com/channels/"+thSkChId+"/private_show\"}]}";
	return result;
}

String EspSettingsBox::getFileName(AbstractItem* item){
	String result=FPSTR(ESPSETTINGSBOX_SETTINGS_PATH);
		   result+=item->getName();
		   result+=FPSTR(ESPSETTINGSBOX_SETTINGS_TXT_EXT);

	return result;
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
				Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_VALUE_PARSED));
				root.printTo(Serial);

				item->setDescr(root["descr"].as<char*>());

				uint8_t childCountJson=root["childCount"];

				if(item->getChildCount()>0 && childCountJson>0){
					//JsonArray& arrayJson=root["items"].asArray();
					Serial.print("totalChilds=");
					Serial.println(childCountJson);

					for(uint8_t i=0;i<childCountJson;i++){

						String name=root["items"][i]["name"];
						String descr=root["items"][i]["descr"];
						uint8_t fieldId=root["items"][i]["fieldId"];
						float minVal=root["items"][i]["minVal"];
						float maxVal=root["items"][i]["maxVal"];
						String queue=root["items"][i]["queue"];

						Serial.println("name="+name+" descr="+descr
								+" fieldId="+String(fieldId)
								+" minVal="+String(minVal)
								+" maxVal="+String(maxVal)
								+" queue="+queue);

						uint8_t child=item->getChildItemIndexByName(name);

						item->setDescr(child, descr);
						item->setFieldId(child, fieldId);
						item->setMinVal(child, minVal);
						item->setMaxVal(child, maxVal);
						item->setQueue(child, queue);

						Serial.println(item->getJson(child));
					}
				}
				//-----------------------------------------
			}
		}
	  }

	  Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}

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

EspSettingsBox::EspSettingsBox(String fileName){
	construct(fileName,"",false,false);
}

EspSettingsBox::EspSettingsBox(String fileName,boolean forceLoad){
	construct(fileName,"",forceLoad,false);
}

EspSettingsBox::EspSettingsBox(String fileName,String extValuesFileName,boolean forceLoad){
	construct(fileName,extValuesFileName,forceLoad,false);
}

EspSettingsBox::EspSettingsBox(String fileName,String extValuesFileName,boolean forceLoad,boolean _initSpiff){
	construct(fileName,extValuesFileName,forceLoad,_initSpiff);
}

//Initializable
	boolean EspSettingsBox::initialize(boolean _init){
		Serial.println("Begin initialize of EspSettingsBox");

		if(!initialized && _init){
			loadSettingsJson();
			initialized=_init;
		}else{
			if(initialized)
				Serial.println("EspSettingsBox already initialized");
		}

		return initialized;
	}

boolean EspSettingsBox::isSpiffInitialized(){
	return spiffInitialized;
}

void EspSettingsBox::saveSettingsJson(){
	Serial.println("Save device settings to file");

	saveExternalFile();

	File settFile = SPIFFS.open(_fileName, "w");

	Serial.println(" startSave");
	getSettingsFromMemory(false).printTo(settFile);
	settFile.close();

	delay(1);

	Serial.println("Device settings saved to file");
}

void EspSettingsBox::loadSettingsJson(){

	//loadExternalFile();

	File file = SPIFFS.open(_fileName, "r");

	if(!file){
		Serial.println("-----FILE not exists. Default settings save");
		saveSettingsJson();
		file = SPIFFS.open(_fileName, "r");
	}

	  if (!file){
	    Serial.println("No settings file exists");
	  } else {
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
	      } else {
	        Serial.println("Settings parsed");

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
	root["pac"] = pageAutochange;
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

	root["bOnA"] = beepOnAlert;
	root["pdTs"] = postDataToTSInterval;
	root["sPk"] = thSkUsrKey;
	root["sWk"] = thSkWKey;
	root["sRk"] = thSkRKey;
	root["sCi"] = thSkChId;
	root["sTk"] = thSkTKey;

	root["AlSAs"]=alarmSendNotifAlertStart;
	root["AlSn"]=alarmSendNotifAlertStop;
	root["AlPs"]=alarmPlaySound;
	root["AlSi"]=alamSoundInterval;
	//root["AlNi"]=alamNotificationInterval;

	root["mxDT"]=maxDHtTemp;
	root["mnDT"]=minDHtTemp;
	root["mxDH"]=maxDHtHum;
	root["mnDH"]=minDHtHum;

	root["isMqttEnabled"]=isMqttEnabled;
	root["mqtt_server"]=mqtt_server;
	root["mqtt_port"]=mqtt_port;
	root["mqtt_user"]=mqtt_user;
	root["mqtt_pass"]=mqtt_pass;
	root["mqtt_topic"]=mqtt_topic;

	root["webRoot"]=webRoot;
	root["webDefPage"]=webDefPage;
	root["webDepExt"]=webDepExt;

	root["NTP_poolServerName"]=NTP_poolServerName;
	root["NTP_timeOffset"]=NTP_timeOffset;
	root["NTP_timeTriggerInterval"]=NTP_timeTriggerInterval;
	root["NTP_updateInterval"]=NTP_updateInterval;


	Serial.println("-----------------Settings from memory-------");
	String vals="";
	root.printTo(vals);
	Serial.println(vals);
	Serial.println("--------------------------------------------");

	if(doSave){
		Serial.println("Save device settings to file");

		File settFile = SPIFFS.open(_fileName, "w");

		Serial.println("begin save");
		root.printTo(settFile);
		settFile.close();
		Serial.println(" saved");

		delay(1);

		Serial.println("Device settings saved to file");
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
	pageAutochange = root["pac"];
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

	beepOnAlert=root["bOnA"];
	postDataToTSInterval = root["pdTs"];
	thSkUsrKey=root["sPk"].as<char*>();
	thSkWKey = root["sWk"].as<char*>();
	thSkRKey = root["sRk"].as<char*>();
	thSkTKey = root["sTk"].as<char*>();
	thSkChId = root["sCi"];

	alarmSendNotifAlertStart=isTrue(root["AlSAs"].as<char*>());
	alarmSendNotifAlertStop=isTrue(root["AlSn"].as<char*>());
	alarmPlaySound=isTrue(root["AlPs"].as<char*>());
	alamSoundInterval=root["AlSi"];
	//alamNotificationInterval=root["AlNi"];

	maxDHtTemp=root["mxDT"];
	minDHtTemp=root["mnDT"];
	maxDHtHum=root["mxDH"];
	minDHtHum=root["mnDH"];

	isMqttEnabled=isTrue(root["isMqttEnabled"]);
	mqtt_server=root["mqtt_server"].as<char*>();
	mqtt_port=root["mqtt_port"];
	mqtt_user=root["mqtt_user"].as<char*>();
	mqtt_pass=root["mqtt_pass"].as<char*>();
	mqtt_topic=root["mqtt_topic"].as<char*>();

	webRoot=root["webRoot"].as<char*>();
	webDefPage=root["webDefPage"].as<char*>();
	webDepExt=root["webDepExt"].as<char*>();

	NTP_poolServerName=root["NTP_poolServerName"].as<char*>();
	NTP_timeOffset=root["NTP_timeOffset"];
	NTP_timeTriggerInterval=root["NTP_timeTriggerInterval"];
	NTP_updateInterval=root["NTP_updateInterval"];

	Serial.println("-----------------Settings loaded to memory-------");
	String vals="";
	root.printTo(vals);
	Serial.println(vals);
	Serial.println("--------------------------------------------");

	return true;
}

void EspSettingsBox::printSettingsFile(){
		Serial.println("-----------Print settings file----------------");
		//loadExternalFile();
		Serial.println("Current memory state");
		getSettingsFromMemory();
		Serial.println("-------------------------------------");
		File file = SPIFFS.open(_fileName, "r");

		if(!file){
			Serial.println("-----FILE not exists. Default settings save");
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
		    Serial.println("----------------direct file read------------");
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

		    		 Serial.println("settings.txt exists");
		    	}else{
		    		 Serial.println("settings.txt MISSED");
		    	}
		    delay(1);

		    //printVariablesToSerial();
		    Serial.println("-------------------------------------------");
}

void EspSettingsBox::printSpiffsInfo(){
	FSInfo fs_info;
	SPIFFS.info(fs_info);

	Serial.println("--------------SPIFFS INFO--------------");
	Serial.println("Initialized="+String(initialized));
	Serial.println("spiffInitialized="+String(spiffInitialized));

	Serial.println("blockSize="+String(fs_info.blockSize));
	Serial.println("maxOpenFiles="+String(fs_info.maxOpenFiles));
	Serial.println("maxPathLength="+String(fs_info.maxPathLength));
	Serial.println("pageSize="+String(fs_info.pageSize));
	Serial.println("totalBytes="+String(fs_info.totalBytes));
	Serial.println("usedBytes="+String(fs_info.usedBytes));
	Serial.println("---------------------------------------");
			#ifdef ESP8266
			Dir dir = SPIFFS.openDir("/");
			while (dir.next()) {
			  String fileName = dir.fileName();
			  size_t fileSize = dir.fileSize();
			  Serial.println("name="+fileName+" size="+String(fileSize));
			}
		#else
			listDir(SPIFFS, "/", 0);
		#endif
	Serial.println("---------------------------------------");
}

void EspSettingsBox::construct(String fileName,String extValuesFileName,boolean forceLoad,boolean _initSpiff){
	_fileName=fileName;
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
			Serial.println("-----FILE not exists. Default settings save");
			saveSettingsJson();
			file = SPIFFS.open(_fileName, "r");
		}

		  if (!file){
		    Serial.println("No settings file exists");
		  } else {
		    size_t size = file.size();
		    if ( size == 0 ) {
		      Serial.println("Empty settings file");
		    } else {

					StaticJsonBuffer<1024> jsonBuffer;
					delay(1);

				  std::unique_ptr<char[]> buf (new char[size]);
				  file.readBytes(buf.get(), size);
				  //JsonObject& root = jsonBuffer.parseObject(buf.get());

				  //extRoot=&jsonBuffer.parseObject(buf.get());

				  if (!extRoot->success()) {
					Serial.println("Impossible to read JSON file");
				  } else {
					Serial.println("Settings parsed");
					extRoot->printTo(Serial);
				  }
		    }
		  }
}

void EspSettingsBox::saveExternalFile(){

}

String EspSettingsBox::getExternalVal(String key){
	if(extRoot->containsKey(key)){
		String keyName="123";

		return extRoot->operator [](key).as<char*>();
	}

	return "";
}


String EspSettingsBox::getParamVal(String key){
	JsonObject& root=getSettingsFromMemory(false);
	//if(root.containsKey(key)){
		return root[key].as<char*>();
	//}
	//return "";
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

String EspSettingsBox::getJson(){

	String result="{\"name\":\"espSettingsBox\",\"val\":\"3\",\"fieldId\":\"0\",\"queueName\":\"\",\"items\":[\
					{\"name\":\"DeviceId\",\"val\":\""+DeviceId+"\"},\
					{\"name\":\"DeviceLocation\",\"val\":\""+DeviceLocation+"\"},\
					{\"name\":\"thSkChId\",\"val\":\""+thSkChId+"\"},\
					{\"name\":\"thinkSpeakChannelUrl\",\"val\":\"https://thingspeak.com/channels/"+thSkChId+"/private_show\"}]}";
	return result;
}

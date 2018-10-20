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

void EspSettingsBox::printVariablesToSerial(){
	Serial.print("isAccesPoint=");Serial.print(isAccesPoint);
	Serial.print("; DeviceId=");Serial.print(DeviceId);
	Serial.print("; refreshInterval=");Serial.print(refreshInterval);
	Serial.print("; displayAlvaysOn=");Serial.println(displayAlvaysOn);
	Serial.print("; displayAutochange=");Serial.println(displayAutochange);
	Serial.print("; pageAutochange=");Serial.println(pageAutochange);

	Serial.print("; settingsUser=");Serial.print(settingsUser);
	Serial.print("; settingsPass=");Serial.print(settingsPass);

	Serial.print("; accessUser=");Serial.print(accessUser);
	Serial.print("; accessPass=");Serial.print(accessPass);

	Serial.print("; ssidAP=");Serial.print(ssidAP);
	//Serial.print("; passwordAP=");Serial.print(passwordAP);
	Serial.print("; apIp=");Serial.print(apIp);

	Serial.print("; ssid=");Serial.print(ssid);
	Serial.print("; password=");Serial.print(password);
	Serial.print("; staticIp=");Serial.print(staticIp);
	Serial.print("; localIp=");Serial.print(localIp);
	Serial.print("; gateIp=");Serial.print(gateIp);
	Serial.print("; subnetIp=");Serial.print(subnetIp);
	Serial.print("; dnsIp=");Serial.print(dnsIp);
	Serial.print("; dnsIp2=");Serial.println(dnsIp2);
	Serial.print("; postDataToTSInterval=");Serial.print(postDataToTSInterval);
	Serial.print("; thingSpeakProfileKey");Serial.println(thSkUsrKey);
	Serial.print("; thingSpeakWriteKey=");Serial.println(thSkWKey);
	Serial.print("; thingSpeakReadKey=");Serial.println(thSkRKey);
	Serial.print("; thingSpeakChannelId=");Serial.println(thSkChId);

	Serial.print("; isMqttEnabled=");Serial.println(isMqttEnabled);
	Serial.print("; mqtt_server=");Serial.println(mqtt_server);
	Serial.print("; mqtt_port=");Serial.println(mqtt_port);
	Serial.print("; mqtt_user=");Serial.println(mqtt_user);
	Serial.print("; mqtt_pass=");Serial.println(mqtt_pass);
	Serial.print("; mqtt_topic=");Serial.println(mqtt_topic);

	Serial.println();}

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

				  extRoot=&jsonBuffer.parseObject(buf.get());

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

void EspSettingsBox::saveSettingsJson(){
	Serial.println("Save device settings to file");

	saveExternalFile();

	StaticJsonBuffer<1024> jsonBuffer;

	JsonObject& root = jsonBuffer.createObject();
	root["dId"] = DeviceId;
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

	root.printTo(Serial);

	  File settFile = SPIFFS.open(_fileName, "w");
	  root.printTo(settFile);
	  settFile.close();

	  delay(1);

	  Serial.println("Device settings saved to file");
}
void EspSettingsBox::loadSettingsJson(){

	loadExternalFile();

	printFileToDIsc();

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
	        root.printTo(Serial);


	        DeviceId = root["dId"].asString();
	        isAccesPoint = isTrue(root["iAp"].asString()) ;
	        Serial.print("isAccesPoint=");
	        Serial.println(isAccesPoint);
	        refreshInterval = root["rin"];
			displayAlvaysOn = root["dao"];
			displayAutochange = root["dac"];
			pageAutochange = root["pac"];
			settingsUser = root["sur"].asString();
			settingsPass = root["sps"].asString();
			accessUser = root["aur"].asString();
			accessPass = root["aps"].asString();
			ssidAP = root["sap"].asString();
			//passwordAP = root["pap"].asString();
			ssid = root["sid"].asString();
			password = root["pas"].asString();

			staticIp=isTrue(root["staticIp"].asString());
			localIp=stringToIp(String(root["lip"].asString()));
			apIp=stringToIp(String(root["aip"].asString()));
			gateIp=stringToIp(String(root["gip"].asString()));
			subnetIp=stringToIp(String(root["sip"].asString()));
			dnsIp=stringToIp(String(root["dip"].asString()));
			dnsIp2=stringToIp(String(root["dip2"].asString()));

			beepOnAlert=root["bOnA"];
			postDataToTSInterval = root["pdTs"];
			thSkUsrKey=root["sPk"].asString();
			thSkWKey = root["sWk"].asString();
			thSkRKey = root["sRk"].asString();
			thSkTKey = root["sTk"].asString();
			thSkChId = root["sCi"];

			alarmSendNotifAlertStart=isTrue(root["AlSAs"].asString());
			alarmSendNotifAlertStop=isTrue(root["AlSn"].asString());
			alarmPlaySound=isTrue(root["AlPs"].asString());
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

	      }
	    }
	    file.close();
	    delay(1);

	    printVariablesToSerial();
	  }
}

boolean EspSettingsBox::isTrue(String str){
	String tmp=String(str);

	return (tmp.toInt()==1) ;
}

void EspSettingsBox::printFileToDIsc(){

	Dir d=SPIFFS.openDir("/");

	while(d.next()){
		Serial.println(d.fileName());
	}

  String cssFileName="/css/w3.css";
  boolean cssFileExists=SPIFFS.exists(cssFileName);

  if(cssFileExists){
	  File cssFile=SPIFFS.open(cssFileName, "r");

		 size_t size =cssFile.size();

		Serial.println("---v3.CSS file available");
		Serial.print("size=");
		Serial.println(size);

		//std::unique_ptr<char[]> buf(new char[size]);

		//Serial.println(cssFile.readBytes(buf.get(), size));
		//Serial.println(String(buf));

		Serial.println("----css file readLines");
		String str;
		int i=0;

		 while(cssFile.available()){
			 i++;
			 //Serial.print(i);
			 str=cssFile.readStringUntil('\n');
			 //Serial.println(str);
			 //Serial.println("---");
		 }


		 Serial.print("css file loaded lines=");
		 Serial.println(i);

		cssFile.close();
	}else{
		Serial.println("css FIleNot found");
	}

  String jsFileName="/js/main.js";
    boolean jsFileExists=SPIFFS.exists(jsFileName);

    if(jsFileExists){
  	  File jsFile=SPIFFS.open(jsFileName, "r");

  		 size_t size =jsFile.size();

  		Serial.println("---js file available");
  		Serial.print("size=");
  		Serial.println(size);

  		//std::unique_ptr<char[]> buf(new char[size]);

  		//Serial.println(cssFile.readBytes(buf.get(), size));
  		//Serial.println(String(buf));

  		Serial.println("----js file readLines");
  		String str;
  		int i=0;

  		 while(jsFile.available()){
  			 i++;
  			 //Serial.print(i);
  			 str=jsFile.readStringUntil('\n');
  			 //Serial.println(str);
  			 //Serial.println("---");
  		 }


  		 Serial.print("css file loaded lines=");
  		 Serial.println(i);

  		jsFile.close();
  	}else{
  		Serial.println("FIleNot found");
  	}
}

String EspSettingsBox::getHtmlVal(String key){
	if(key.equals("DeviceId")){
		return "<h1>"+DeviceId+"</h1>";
	}
	return "";
}

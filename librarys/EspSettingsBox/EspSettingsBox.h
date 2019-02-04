/*
 * EspSettingsBox.h
 *
 *  Created on: 2 Ñ„ÐµÐ²Ñ€. 2018 Ð³.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_
#define LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_

#include <interfaces/ItemFieldDescriptor.h>
#include "Arduino.h"
//#include "ESP8266WiFi.h"
#include "FS.h"
#include "ArduinoJson.h"
#include "ESP_Consts.h"
#include "IPAddress.h"
#include "ESPExtraSettingsBox.h"
#include "projectConsts.h"
#include "Consts/PagesConsts.h"
#include "Consts/CommandsConsts.h"

#include "AbstractItem.h"
#include "interfaces/DeviceLibable.h"
#include "interfaces/Initializable.h"
#include "interfaces/JSONprocessor.h"
#include "interfaces/JSONprovider.h"

#include "ItemFieldProviderService.h"
#include "ExtraSettingsBoxService.h"
#include "AbstractItemSettingsService.h"

const PROGMEM char EspSettingsBox_NAME[] = "espSettingsBox";

class EspSettingsBox: public Initializable, public JSONprocessor , public JSONprovider,
	public ItemFieldProviderService, public ExtraSettingsBoxService, public AbstractItemSettingsService, public DeviceLibable {

public:
	EspSettingsBox(){
		construct(false,false);
	}

	EspSettingsBox(ESPExtraSettingsBox** extraBoxes)
				:ExtraSettingsBoxService(extraBoxes){
			construct(false,false);
	}

	EspSettingsBox(boolean forceLoad,boolean _initSpiff){
			construct(forceLoad,_initSpiff);
	}

	virtual boolean initialize(boolean _init) override;

	void loadSettingsJson();
	void saveSettingsJson();

	boolean setSettingsValue(String fieldName,String fieldValue);

	boolean getSaveRequired(){
		return saveRequired;
	}

	String getName(){
		return FPSTR(EspSettingsBox_NAME);
	}

	StatusMessage processJson(String page,String json){
		printProcessParams(page, json);

		if(page==FPSTR(PAGE_MANAGE) && json==FPSTR(COMMAND_DELETE_SETTINGS)){
			int count=deleteSettingsFiles();

			return StatusMessage(STATUS_OK_DELETED_INT,String(count));
		}

		if(page==FPSTR(PAGE_DEVICE) || page==FPSTR(PAGE_NET) || page==FPSTR(PAGE_PUBLISH)){
			beginSetSettingsValue(page);

			DynamicJsonBuffer jsonBuffer;
			JsonObject& root = jsonBuffer.parseObject(json);

			if(!root.success()){
				Serial.println(FPSTR("Error parse JSOn"));
				return StatusMessage(STATUS_PARSE_ERROR_INT);
			}

			JsonObject::iterator it;
			  for (it = root.begin(); it != root.end(); ++it ){
				  String argName=it->key;
				  String argVal=it->value.asString();

				  if(!setSettingsValue(argName,argVal)){
				  			  				Serial.print(FPSTR("FAILED find target for argName="));
				  			  				Serial.print(argName);
				  			  				Serial.print(FPSTR(" argVal="));
				  			  				Serial.println(argVal);
  	  			  }
			  }

			finishSetSettingsValue(page);

			return StatusMessage(STATUS_OK_SAVED_INT,page,getJson(page));
		}

		return StatusMessage(STATUS_NOT_FOUND_INT);
	}

	void beginSetSettingsValue(String page){
		Serial.print(FPSTR("--- page="));
		Serial.print(page);
		Serial.println(FPSTR(" save Started"));

		saveRequired=false;

		beginSetExtraSettingsValue(page);
	}

	void finishSetSettingsValue(String page){
		Serial.print(FPSTR("EspSettingsBox saveRequired="));
		Serial.print(saveRequired);
		Serial.print(FPSTR("  "));

		if(saveRequired){
			saveSettingsJson();
		}

		saveRequired=false;

		boolean ebsr=finishSetExtraSettingsValue();

		Serial.print(FPSTR("--- page="));
		Serial.print(page);
		Serial.print(FPSTR(" save finished"));
	}

	String getJson();
	String getJson(String page);

	void resetToAp(){
		Serial.println(FPSTR("Device will be started as AP"));
		isAccesPoint=true;
		password="password";
		saveSettingsJson();
		Serial.println(FPSTR("---Saved as AP---"));
	}

	boolean saveSettingToFile(String settingsName,String str){
		String fileName=EspSettingsUtil::getSettingsFilePath(settingsName);
		return EspSettingsUtil::saveStringToFile(fileName, str);
	}

	String getDeviceNameFull(){
		String result=FPSTR(MESSAGE_THINGSPEAK_NAME_FOR_REQUEST_EQ);
		result+=DeviceLocation;
		result+=FPSTR(MESSAGE_SPACE);
		result+=DeviceId;

		return result;
	}

	String getDeviceDescriptionFull(){
		String result= FPSTR(MESSAGE_THINGSPEAK_DESCRIPTION_FOR_REQUEST_EQ);
				result+=DeviceDescription;
				result+=FPSTR(MESSAGE_SPACE);
				result+=DEVICE_KIND;
		return result;
	}

	String getThingSpeakReadChannelName(String fieldId){
		String result=FPSTR(MESSAGE_THINGSPEAK_CHANNELS_PREF);
				result+=String(thSkChId);
				result+=FPSTR(MESSAGE_THINGSPEAK_SUBSCRIBE_FIELDS_FIELD);
				result+=fieldId;
				result+=FPSTR(MESSAGE_DIVIDE);
				result+=thSkRKey;
		return  result;
	}

	boolean isSpiffInitialized();

	void printSpiffsInfo();

	int deleteSettingsFiles();

	boolean saveThingSpeakChannelCreation(String response/*,boolean manageChannel*/);
	String getThingSpeakChannelUrl();

	virtual void saveDefaultItemFieldProviderValuesIfMissing(uint8_t pId,String aName,String sName) override{
		ItemFieldProviderService::saveDefaultItemFieldProviderValuesIfMissing(pId, aName, sName);
	}

	virtual ItemFieldProviderService* getItemProviderService() override{
		return this;
	}

//--------------------device settings kind (page)-------------------
	#ifdef ESP8266
		String DeviceId = "SENS_"+String(ESP.getChipId());
	#endif
	#ifdef ESP32
		String DeviceId = "SENS_"+String(ESP.getChipRevision());
	#endif
	String DeviceDescription = DEVICE_DESCR;
	String DeviceLocation = DEVICE_LOCATION;

	String accessUser = "";
	String accessPass = "";

	String settingsUser = "balabas";
	String settingsPass = "balabas";

	uint16_t refreshInterval=60;

	boolean displayAlvaysOn=false;
	uint8_t displayAutochange=15;

	boolean isAccesPoint=false;
	#ifdef ESP8266
		String ssidAP="SENS_"+String(ESP.getChipId());;
	#endif
	#ifdef ESP32
		String ssidAP="SENS_"+String(ESP.getChipRevision());;
	#endif

	String ssid = "balabasKiev5";//balabasKiev5
	String password = "wuWylKegayg2wu22";//wuWylKegayg2wu22

	boolean staticIp=false;

	IPAddress localIp=IPAddress(192, 168, 0, 120);
	IPAddress apIp=IPAddress(192, 168, 4, 1);
	IPAddress gateIp=IPAddress(192, 168, 0, 1);
	IPAddress subnetIp=IPAddress(255, 255, 255, 0);
	IPAddress dnsIp=IPAddress(192, 168, 0, 1);
	IPAddress dnsIp2=IPAddress(192, 168, 0, 1);
	IPAddress serverIp=IPAddress(192, 168, 0, 2);

	//thingspeak settings
	boolean isThingSpeakEnabled=false;
	uint16_t postDataToTSInterval=241;
	String thSkUsrKey="YV1SA7H87PS8RFMU";

	int thSkChId=612324;
	String thSkWKey="V8V5G1W2CACCQOMV";
	String thSkRKey="SPH0UG4JJZC7GDDH";

	String thSkTKey="ZSFBN9V1K2QMB9M1";

	boolean isMqttEnabled=false;
	uint16_t postDataToMqttInterval=241;
	String mqtt_server = "mqtt.thingspeak.com";
	String mqtt_user = "user";
	String mqtt_pass = "HM4A48J6A926FL81";
	int mqtt_port = 1883;

	boolean isHttpPostEnabled=false;
	uint16_t postDataToHttpInterval=241;
	IPAddress httpPostIp=IPAddress(192, 168, 0, 2);

private:
	boolean saveRequired=false;
	boolean loaded=false;

	boolean spiffInitialized;
	String _fileName;

	void construct(boolean forceLoad,boolean initSpiff);
	void initSpiff();
	void listDir(fs::FS &fs, const char * dirname, uint8_t levels);

	size_t bufferLength=2048;
};




#endif /* LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_ */

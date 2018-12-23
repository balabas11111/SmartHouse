/*
 * EspSettingsBox.h
 *
 *  Created on: 2 февр. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_
#define LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_

#include "Arduino.h"
//#include "ESP8266WiFi.h"
#include "FS.h"
#include "ArduinoJson.h"
#include "Initializable.h"
#include "AbstractItem.h"
#include "ESP_Consts.h"
#include "IPAddress.h"
#include "ESPExtraSettingsBox.h"

class EspSettingsBox: public Initializable{

public:
	EspSettingsBox(String deviceKind);
	EspSettingsBox(String deviceKind,ESPExtraSettingsBox** boxes,uint8_t boxesCount);
	//EspSettingsBox(String deviceKind,boolean forceLoad);
	EspSettingsBox(String deviceKind,boolean forceLoad,boolean _initSpiff);

	virtual boolean initialize(boolean _init) override;

	boolean initExtraBoxes();

	void loadSettingsJson();
	void saveSettingsJson();

	boolean setSettingsValue(String fieldName,String fieldValue);

	void loadAbstractItemsFromFile(AbstractItem** items,uint8_t size){
		for(uint8_t i=0;i<size;i++){
			loadAbstractItemFromFile(items[i]);
		}
	}
	void saveAbstractItemsToFile(AbstractItem** items,uint8_t size){
		for(uint8_t i=0;i<size;i++){
			saveAbstractItemToFile(items[i]);
		}
	}

	void printHeap(){
		Serial.print(FPSTR("heap="));
		Serial.println(ESP.getFreeHeap());
	}

	void loadAbstractItemFromFile(AbstractItem* item);
	void saveAbstractItemToFile(AbstractItem* item);

	boolean stringToBoolean(String str);
	IPAddress stringToIp(String str);

	boolean validateIP(String str);
	String clearNlFromString(String str);

	String getThingSpeakChannelUrl();

	String getSimpleJson();
	String getJson(String page);

	String getFileName(AbstractItem* item);

	String getDefaultExtraBoxJson(){
		return "{\"name\":\"extraBoxesCount\",\"val\":\""+String(extraBoxesCount)+"\"}";
	}

	String getExtraBoxJson(){
		String result="";
		for(uint8_t i=0;i<extraBoxesCount;i++){
			result+=extraBoxes[i]->getJson();
			result+=",";
		}

		result+=getDefaultExtraBoxJson();
		return result;
	}

	String getExtraBoxJson(String boxname){
		ESPExtraSettingsBox* box=getExtraBox(boxname);
		if(box==nullptr){
			return getDefaultExtraBoxJson();
		}

		return box->getJson();
	}

	String getExtraBoxJson(uint8_t boxIndex){
		if(boxIndex>extraBoxesCount){
			return getDefaultExtraBoxJson();
		}

		return extraBoxes[boxIndex]->getJson();
	}

	String getBoxFileName(uint8_t boxIndex){
		String boxFileName=FPSTR(ESPSETTINGSBOX_SETTINGS_PATH);
					boxFileName+=extraBoxes[boxIndex]->getName();
					boxFileName+=FPSTR(ESPSETTINGSBOX_SETTINGS_TXT_EXT);

		return boxFileName;
	}

	boolean checkBoxFileExists(uint8_t boxIndex){
		if(boxIndex>extraBoxesCount){
			return false;
		}

		String boxFileName=getBoxFileName(boxIndex);

	   File file = SPIFFS.open(boxFileName, "r");

		if(!file || file.size()==0){
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_BOX_FILE_NOT_EXISTS));
			saveExtraBox(boxIndex);
		}

		return true;
	}

	void saveExtraBoxes(){
		for(uint8_t i=0;i<extraBoxesCount;i++){
			saveExtraBox(i);
		}
	}

	void loadExtraBoxes(){
		for(uint8_t i=0;i<extraBoxesCount;i++){
			loadExtraBox(i);
		}
	}

	boolean saveExtraBox(uint8_t boxIndex){
		if(boxIndex>extraBoxesCount){
			return false;
		}

		uint8_t boxKeySize=extraBoxes[boxIndex]->getKeySize();

		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SAVE_EXTRABOX_SETTINGS_TO_FILE));

		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_BEGIN_SAVE));

		DynamicJsonBuffer jsonBuffer;
		//StaticJsonBuffer<1024> jsonBuffer;

		JsonObject& root = jsonBuffer.createObject();

		for(uint8_t i=0;i<boxKeySize;i++){
			String key=extraBoxes[boxIndex]->getKey(i);
			String value=extraBoxes[boxIndex]->getValue(i);

			root[key] = value;
		}

		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SETTINGS_FROM_MEMORY));
		String vals="";
		root.printTo(vals);
		Serial.println(vals);
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SAVE_EXTRABOX_SETTINGS_TO_FILE));

		String boxFileName=getBoxFileName(boxIndex);
		File boxFile = SPIFFS.open(boxFileName, "w");

		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_BEGIN_SAVE));
		root.printTo(boxFile);
		boxFile.close();
		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_SAVED));

		delay(1);

		Serial.println(FPSTR(MESSAGE_DONE));
		boxFile.close();

		delay(1);

		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_SAVED));
		Serial.println(FPSTR(MESSAGE_DONE));

		return true;
	}

	boolean deleteExtraBoxFile(uint8_t boxIndex){
		String fileName=getBoxFileName(boxIndex);

		return SPIFFS.remove(fileName);
	}

	boolean loadExtraBox(uint8_t boxIndex){

	  checkBoxFileExists(boxIndex);

      StaticJsonBuffer<1024> jsonBuffer;
      delay(1);

      String boxFileName=getBoxFileName(boxIndex);
      File boxFile = SPIFFS.open(boxFileName, "r");
      size_t size = boxFile.size();

      std::unique_ptr<char[]> buf (new char[size]);
      boxFile.readBytes(buf.get(), size);
      JsonObject& root = jsonBuffer.parseObject(buf.get());
      if (!root.success()) {
    	  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
      } else {
    	  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_VALUE_PARSED));
    	  root.printTo(Serial);
    	  Serial.println();
    	  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_READ_FROM_FILE_COMPLETE));

    	  uint8_t boxKeySize=extraBoxes[boxIndex]->getKeySize();

    	  for(uint8_t i=0;i<boxKeySize;i++){
				String key=extraBoxes[boxIndex]->getKey(i);
				String value=root[key].as<char*>();

				uint8_t keyIndex=i;

				if(extraBoxes[boxIndex]->getKey(i)!=key){
					keyIndex=extraBoxes[boxIndex]->getKeyIndex(key);
				}

				extraBoxes[boxIndex]->setValue(keyIndex,value);
			}

    	  	Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SETTINGS_TO_MEMORY));
    	  	String vals="";
    	  	root.printTo(vals);
    	  	Serial.println(vals);
    	  	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

      	  }

      boxFile.close();
	  return true;
	}

	boolean loadDefaultValues(uint8_t boxIndex){
		if(boxIndex>extraBoxesCount){
			return false;
		}

		return extraBoxes[boxIndex]->init();
	}

	ESPExtraSettingsBox* getExtraBox(String boxName){
		if(extraBoxesCount!=0){
			for(uint8_t i=0;i<extraBoxesCount;i++){
				if(extraBoxes[i]->getName()==boxName){
					return extraBoxes[i];
				}
			}
		}

		return nullptr;
	}

	String getExtraValue(String boxName,String key){
		if(extraBoxesCount!=0){
			for(uint8_t i=0;i<extraBoxesCount;i++){
				if(extraBoxes[i]->getName()==boxName){
					return extraBoxes[i]->getValue(key);
				}
			}
		}

		return String("");
	}

	String getExtraValue(String boxName,int index){
		if(extraBoxesCount!=0){
			for(uint8_t i=0;i<extraBoxesCount;i++){
				if(extraBoxes[i]->getName()==boxName){
					return extraBoxes[i]->getValue(index);
				}
			}
		}
		return String("");
	}

	int getExtraValueInt(String boxName,String key){
		return getExtraValue(boxName,key).toInt();
	}

	int getExtraValueInt(String boxName,int index){
		return getExtraValue(boxName,index).toInt();
	}

	float getExtraValueFloat(String boxName,String key){
		return getExtraValue(boxName,key).toFloat();
	}

	float getExtraValueFloat(String boxName,int index){
		return getExtraValue(boxName,index).toFloat();
	}

	boolean setExtraValue(String boxName,String key,String value){
		ESPExtraSettingsBox* extraBox=getExtraBox(boxName);

		if(extraBox!=nullptr){
			return extraBox->setValue(key, value);
		}

		return false;
	}

	boolean setExtraValue(String boxName,int index,String value){
		ESPExtraSettingsBox* extraBox=getExtraBox(boxName);

		if(extraBox!=nullptr){
			return extraBox->setValue(index, value);
		}

		return false;
	}

	boolean setExtraValue(uint8_t boxIndex,String key,String value){
		if(boxIndex>extraBoxesCount){
			return false;
		}

		return extraBoxes[boxIndex]->setValue(key, value);
	}

	boolean setExtraValue(uint8_t boxIndex,int index,String value){
		if(boxIndex>extraBoxesCount){
			return false;
		}

		return extraBoxes[boxIndex]->setValue(index, value);
	}
/*
	String getSimpleJsonPublishUrl(){
		return FPSTR(ESPSETTINGSBOX_GET_SIMPLE_JSON_PUBLISH_URL);
	}
	String getJsonPublishUrl(){
		return FPSTR(ESPSETTINGSBOX_GET_JSON_PUBLISH_URL);
	}
	String getSetValueUrl(){
		return FPSTR(ESPSETTINGSBOX_SET_JSON_PUBLISH_URL);
	}
*/
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
				result+=DeviceKind;
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

	//void printSettingsFile();

	boolean isSpiffInitialized();

	void printSpiffsInfo();

	int deleteSettingsFiles();

	boolean saveThingSpeakChannelCreation(String response/*,boolean manageChannel*/);

	String deviceFirmWareVersion = "v.1.0";
	#ifdef ESP8266
		String DeviceId = "SENS_"+String(ESP.getChipId());
	#endif
	#ifdef ESP32
		String DeviceId = "SENS_"+String(ESP.getChipRevision());
	#endif
	String DeviceKind = "Климат и управление выключателями";
	String DeviceDescription = "_";
	String DeviceLocation = "Кухня";

	boolean displayAlvaysOn=false;
	uint8_t displayAutochange=15;

	uint16_t refreshInterval=60;

	String accessUser = "";
	String accessPass = "";

	String settingsUser = "balabas";
	String settingsPass = "balabas";

	boolean isAccesPoint=false;
	#ifdef ESP8266
		String ssidAP="SENS_"+String(ESP.getChipId());;
	#endif
	#ifdef ESP32
		String ssidAP="SENS_"+String(ESP.getChipRevision());;
	#endif
	//String passwordAP="";

	String ssid = "balabasKiev5";//balabasKiev5
	String password = "wuWylKegayg2wu22";//wuWylKegayg2wu22

	boolean staticIp=false;

	IPAddress localIp=IPAddress(192, 168, 0, 70);
	IPAddress apIp=IPAddress(192, 168, 4, 1);
	IPAddress gateIp=IPAddress(192, 168, 0, 1);
	IPAddress subnetIp=IPAddress(255, 255, 255, 0);
	IPAddress dnsIp=IPAddress(192, 168, 0, 1);
	IPAddress dnsIp2=IPAddress(192, 168, 0, 1);
	IPAddress serverIp=IPAddress(192, 168, 0, 2);

	boolean beepOnAlert=false;
	boolean alarmSendNotifAlertStart=true;
	boolean alarmSendNotifAlertStop=true;
	boolean alarmPlaySound=true;

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

	boolean ntpEnabled=false;
	String NTP_poolServerName="europe.pool.ntp.org";
	int NTP_timeOffset=7200;
	int NTP_updateInterval=120;

	ulong NTP_timeTriggerInterval=1000;

	//telegram settings
	//default bot  t.me/SmartHouse_balabas_bot   default chat   https://t.me/joinchat/JPhZDxJ7MBZV9HPkR0YE6A
	String telegramApiKey="737840576:AAH_9-PM8knquJ3x1GN-sOTX4NGPNdU50iE";
	String telegramReceivers="JPhZDxJ7MBZV9HPkR0YE6A";
	String telegramSenders;

private:
	boolean loaded=false;

	boolean spiffInitialized;
	String _fileName;
	//String _extFileName;

	ESPExtraSettingsBox** extraBoxes;
	uint8_t extraBoxesCount=0;

	void construct(String deviceKind,boolean forceLoad,boolean initSpiff);
	void initSpiff();
	void listDir(fs::FS &fs, const char * dirname, uint8_t levels);

	size_t bufferLength=2048;
};




#endif /* LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_ */

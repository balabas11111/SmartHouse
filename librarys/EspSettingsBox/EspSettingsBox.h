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

	boolean setSettingsValueIfExtra(String fieldName,String fieldValue);

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
	String booleanToString(boolean val);
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
		return getExtraBoxJson(getExtraBoxIndex(boxname));
	}

	String getExtraBoxJson(int boxIndex){
		if(boxIndex>extraBoxesCount || boxIndex<0){
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

	   boolean saveDefVals=!file || file.size()==0;
	   file.close();

		if(saveDefVals){
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_BOX_FILE_NOT_EXISTS));
			saveExtraBox(boxIndex);
		}else{
			Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_BOX_FILE_EXISTS));
			Serial.print(FPSTR(" file="));
			Serial.println(boxFileName);
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

		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SETTINGS_FROM_BOX_MEMORY));
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
		delay(1);

		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_SAVED));
		Serial.println(FPSTR(MESSAGE_DONE));

		return true;
	}

	void deleteExtraBoxesFiles(){
		for(uint8_t i=0;i<extraBoxesCount;i++){
			deleteExtraBoxFile(i);
		}

	}

	boolean deleteExtraBoxFile(uint8_t boxIndex){
		String fileName=getBoxFileName(boxIndex);
		boolean result= SPIFFS.remove(fileName);

		Serial.print(FPSTR("Del BOX file ="));
		Serial.print(fileName);
		Serial.print(FPSTR(" result ="));
		Serial.println(result);

		return result;
	}

	boolean validateExtraBoxFile(uint8_t boxIndex,boolean recreate){
	   String boxFileName=getBoxFileName(boxIndex);
	   File boxFile = SPIFFS.open(boxFileName, "r");
	   size_t size = boxFile.size();
	   boolean fileExists=boxFile && size!=0;
	   boxFile.close();

	   Serial.println(FPSTR("-------Validate BOX file------"));
	   Serial.print(boxFileName);
	   Serial.print(FPSTR(" exists="));
	   Serial.print(fileExists);
	   Serial.print(FPSTR(" size="));
	   Serial.println(size);

	   if(!fileExists && recreate){
		   Serial.println(FPSTR("file NOT exists and will be recreated"));
		   extraBoxes[boxIndex]->fillDefaultValues();
		   saveExtraBox(boxIndex);
	   }else{
		   Serial.println(FPSTR("file exists"));
	   }

	   File boxFile2 = SPIFFS.open(boxFileName, "r");
	   size_t size2 = boxFile2.size();

		StaticJsonBuffer<1024> jsonBuffer;
		std::unique_ptr<char[]> buf (new char[size2]);
		boxFile2.readBytes(buf.get(), size2);
		JsonObject& root = jsonBuffer.parseObject(buf.get());

		boxFile2.close();

		if (!root.success()) {
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
			return false;
		} else {
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_LINES));
			root.printTo(Serial);
			Serial.println(FPSTR("Validate Keys"));

			uint8_t boxKeySize=extraBoxes[boxIndex]->getKeySize();
			boolean keysValid=true;

			for(uint8_t i=0;i<boxKeySize;i++){
				String key=extraBoxes[boxIndex]->getKey(i);
				boolean inFile=root.containsKey(key);

				if(!inFile){
					keysValid=false;
					Serial.print(FPSTR("key="));
					Serial.print(key);
					Serial.println(FPSTR(" - NOT in file"));
				}
			}

			if(!keysValid){
				Serial.println(FPSTR("RECREATE file"));
				deleteExtraBoxFile(boxIndex);

				extraBoxes[boxIndex]->fillDefaultValues();

				saveExtraBox(boxIndex);
			}else{
				Serial.println(FPSTR("All keys are presented in file"));
			}
		}

	   return true;
	}

	boolean loadExtraBox(uint8_t boxIndex){

	  String boxFileName=getBoxFileName(boxIndex);
	  Serial.print(FPSTR("---Load Extra BOX from file "));
	  Serial.println(boxFileName);

	  validateExtraBoxFile(boxIndex,true);

      StaticJsonBuffer<1024> jsonBuffer;
      delay(1);

      File boxFile = SPIFFS.open(boxFileName, "r");
      size_t size = boxFile.size();

      std::unique_ptr<char[]> buf (new char[size]);
      boxFile.readBytes(buf.get(), size);
      JsonObject& root = jsonBuffer.parseObject(buf.get());
      if (!root.success()) {
    	  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
      } else {
    	  //Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_FILE_LINES));
    	  //root.printTo(Serial);
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

    	  	Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SETTINGS_TO_BOX_MEMORY));
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

	boolean hasExtraBoxes(){
		return extraBoxesCount!=0;
	}

	boolean hasExtraBoxes(int boxIndex){
		return extraBoxesCount!=0 && boxIndex>-1 && boxIndex<extraBoxesCount;
	}

	int getExtraBoxIndex(String boxName){
		if(hasExtraBoxes()){
			for(uint8_t i=0;i<extraBoxesCount;i++){
				if(extraBoxes[i]->getName()==boxName){
					return i;
				}
			}
		}

		return -1;
	}

	ESPExtraSettingsBox* getExtraBox(String boxName){
		int boxIndex=getExtraBoxIndex(boxName);

		if(boxIndex!=-1){
			return extraBoxes[boxIndex];
		}

		return nullptr;
	}

	int getExtraKeyIndex(int boxIndex,String key){
		if(!hasExtraBoxes(boxIndex)){return -1;}

		return extraBoxes[boxIndex]->getKeyIndex(key);
	}

	String getExtraKeyByIndex(int boxIndex,int keyIndex){
		if(!hasExtraBoxes(boxIndex)){return "";}

		return extraBoxes[boxIndex]->getKey(keyIndex);
	}

	boolean hasExtraKey(int boxIndex,String key){
		return getExtraKeyIndex(boxIndex, key)>-1;
	}
//--------------------Get Extra values-----------------------
	String getExtraValue(int boxIndex,int keyIndex){
		if(!hasExtraBoxes(boxIndex)){return "";};

		return extraBoxes[boxIndex]->getValue(keyIndex);
	}

	String getExtraValue(String boxName,int index){
		int boxIndex=getExtraBoxIndex(boxName);

		return getExtraValue(boxIndex,index);
	}

	String getExtraValue(String boxName,String key){
		int boxIndex=getExtraBoxIndex(boxName);
		int keyIndex=getExtraKeyIndex(boxIndex, key);

		return getExtraValue(boxIndex, keyIndex);
	}

	int getExtraValueInt(int boxIndex,int index){
		return getExtraValue(boxIndex,index).toInt();
	}

	int getExtraValueInt(String boxName,String key){
		return getExtraValue(boxName,key).toInt();
	}

	int getExtraValueInt(String boxName,int index){
		return getExtraValue(boxName,index).toInt();
	}

	float getExtraValueFloat(int boxIndex,int index){
		return getExtraValue(boxIndex,index).toFloat();
	}

	float getExtraValueFloat(String boxName,String key){
		return getExtraValue(boxName,key).toFloat();
	}

	float getExtraValueFloat(String boxName,int index){
		return getExtraValue(boxName,index).toFloat();
	}

	boolean getExtraValueBoolean(String boxName,String key){
		return stringToBoolean(getExtraValue(boxName, key));
	}

	boolean getExtraValueBoolean(String boxName,int index){
		return stringToBoolean(getExtraValue(boxName, index));
	}
//-------------------set extra values--------------
	boolean setExtraValue(int boxIndex,int keyIndex,String value){
		if(!hasExtraBoxes(boxIndex)){return false;}

		return extraBoxes[boxIndex]->setValue(keyIndex, value);
	}

	boolean setExtraValue(int boxIndex,String key,String value){
		int keyIndex=getExtraKeyIndex(boxIndex, key);

		return setExtraValue(boxIndex,keyIndex,value);
	}


	boolean setExtraValue(String boxName,String key,String value){
		int boxIndex=getExtraBoxIndex(boxName);

		return setExtraValue(boxIndex,key,value);
	}

	boolean setExtraValue(String boxName,String key,int value){
		return setExtraValue(boxName,key,String(value));
	}

	boolean setExtraValue(String boxName,String key,float value){
		return setExtraValue(boxName,key,String(value));
	}

	boolean setExtraValue(String boxName,String key,boolean value){
		return setExtraValue(boxName,key,String(value));
	}

	void testExtraBoxFunctionality(){
		if(extraBoxesCount==0){
			Serial.println(FPSTR("---Test FAILED---"));
			return;
		}

		Serial.println(FPSTR("-----------------Test extraBoxes started----------------"));

		int boxIndex=0;
		int keyIndex=0;

		ESPExtraSettingsBox* box=extraBoxes[boxIndex];

		String boxName=box->getName();

		Serial.println(FPSTR("--------------------------------------------------------"));

		box->printDetails();

		String key=box->getKey(keyIndex);
		String value=getExtraValue(boxName,keyIndex);
		String valueByStr=getExtraValue(boxName, key);

		String newValue="newValue";

		Serial.println("key="+key+" value="+value+" valueByStr="+valueByStr);

		Serial.println(FPSTR("-----------------SET NEW VALUE---------------------"));
		setExtraValue(boxName, key, newValue);

		box->printDetails();

		saveExtraBox(boxIndex);
		loadExtraBox(boxIndex);
		box->printDetails();

		Serial.println(FPSTR("-----------------SET OLD VALUE---------------------"));
		setExtraValue(boxName, key, value);

		saveExtraBox(boxIndex);
		loadExtraBox(boxIndex);
		box->printDetails();

		Serial.println(FPSTR("-----------------JSON---------------------"));
		Serial.println(getJson(FPSTR("device")));
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

	/*
	boolean beepOnAlert=false;
	boolean alarmSendNotifAlertStart=true;
	boolean alarmSendNotifAlertStop=true;
	boolean alarmPlaySound=true;

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
	*/

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

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

class EspSettingsBox: public Initializable{

public:
	EspSettingsBox(String deviceKind);
	//EspSettingsBox(String deviceKind,boolean forceLoad);
	EspSettingsBox(String deviceKind,boolean forceLoad,boolean _initSpiff);

	virtual boolean initialize(boolean _init) override;

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

	void construct(String deviceKind,boolean forceLoad,boolean initSpiff);
	void initSpiff();
	void listDir(fs::FS &fs, const char * dirname, uint8_t levels);

	size_t bufferLength=2048;
};




#endif /* LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_ */

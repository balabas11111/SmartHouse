/*
 * EspSettingsBox.h
 *
 *  Created on: 2 февр. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_
#define LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_

#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ArduinoJson.h"
#include "Initializable.h"
#include "AbstractItem.h"

class EspSettingsBox: public Initializable{

public:
	EspSettingsBox(String fileName);
	EspSettingsBox(String fileName,boolean forceLoad);
	EspSettingsBox(String fileName,String extValuesFileName,boolean forceLoad);
	EspSettingsBox(String fileName,String extValuesFileName,boolean forceLoad,boolean _initSpiff);

	virtual boolean initialize(boolean _init) override;

	String getParamVal(String key);

	void loadExternalFile();
	void saveExternalFile();
	String getExternalVal(String key);

	IPAddress stringToIp(String str);
	boolean validateIP(String str);
	void saveSettingsJson();
	String getSettingsAsJsonString();
	void loadSettingsJson();

	void getSettingsFromMemory(){
		getSettingsFromMemory(false);
	}
	JsonObject& getSettingsFromMemory(boolean doSave);
	boolean putSettingsToMemory(JsonObject& root);

	String clearNlFromString(String str);

	boolean isTrue(String str);

	String getJson();
	String getSimpleJson(){
		return getJson();
	}

	void saveAbstractItemsToFile(AbstractItem** items,uint8_t size){
		for(uint8_t i=0;i<size;i++){
			saveAbstractItemToFile(items[i]);
		}
	}
	void loadAbstractItemsFromFile(AbstractItem** items,uint8_t size){
		for(uint8_t i=0;i<size;i++){
			loadAbstractItemFromFile(items[i]);
		}
	}

	void saveAbstractItemToFile(AbstractItem* item);
	void loadAbstractItemFromFile(AbstractItem* item);

	String getFileName(AbstractItem* item);

	String getName(){
		return "espSettingsBox";
	}
	String getJsonPublishUrl(){
		return "/"+getName()+"/getJson";
	}

	void printSettingsFile();

	boolean isSpiffInitialized();

	void printSpiffsInfo();

	String DeviceId = "SENS_"+String(ESP.getChipId());
	String DeviceKind = "Климат и управление выключателями";
	String DeviceDescription = "_";
	String DeviceLocation = "Кухня";

	boolean displayAlvaysOn=false;
	uint8_t displayAutochange=15;
	uint8_t pageAutochange=60;

	String settingsUser = "balabas";
	String settingsPass = "balabas";

	String accessUser = "";
	String accessPass = "";

	boolean isAccesPoint=false;
	String ssidAP="SENS_"+String(ESP.getChipId());;
	//String passwordAP="";

	String ssid = "balabasKiev5";//balabasKiev5
	String password = "wuWylKegayg2wu22";//wuWylKegayg2wu22

	int serverPort=80;
	boolean staticIp=false;

	IPAddress localIp=IPAddress(192, 168, 0, 70);
	IPAddress apIp=IPAddress(192, 168, 4, 1);
	IPAddress gateIp=IPAddress(192, 168, 0, 1);
	IPAddress subnetIp=IPAddress(255, 255, 255, 0);
	IPAddress dnsIp=IPAddress(192, 168, 0, 1);
	IPAddress dnsIp2=IPAddress(192, 168, 0, 1);

	uint8_t refreshInterval=60;
	boolean beepOnAlert=false;

	uint8_t postDataToTSInterval=241;
	String thSkUsrKey="YV1SA7H87PS8RFMU";
	String thSkWKey="V8V5G1W2CACCQOMV";
	String thSkRKey="SPH0UG4JJZC7GDDH";
	int thSkChId=612324;
	String thSkTKey="ZSFBN9V1K2QMB9M1";

	boolean alarmSendNotifAlertStart=true;
	boolean alarmSendNotifAlertStop=true;
	boolean alarmPlaySound=true;
	uint16_t alamSoundInterval=30;
	//uint16_t alamNotificationInterval=60;

	boolean isMqttEnabled=false;
	String mqtt_server = "m23.cloudmqtt.com";
	int mqtt_port = 10186;
	String mqtt_user = "tpheglmk";
	String mqtt_pass = "QgYRfVzDhQ31";
	String mqtt_topic="topic/basePublish";

	String webDefPage="/web/index.htm";
	String webRoot="/web/";
	String webDepExt="html,htm,js,css,jpeg,png,jpg,gif,ttf,woff,woff2,tpl";

	String NTP_poolServerName="europe.pool.ntp.org";
	int NTP_timeOffset=7200;
	int NTP_updateInterval=120000;

	ulong NTP_timeTriggerInterval=1000;


private:
	boolean spiffInitialized;
	String _fileName;
	String _extFileName;
	JsonObject* extRoot;

	void construct(String fileName,String extValuesFileName,boolean forceLoad,boolean initSpiff);
	void initSpiff();

	size_t bufferLength=2048;
};




#endif /* LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_ */

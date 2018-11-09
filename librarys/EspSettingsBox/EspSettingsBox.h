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
	EspSettingsBox();
	EspSettingsBox(boolean forceLoad);
	EspSettingsBox(String extValuesFileName,boolean forceLoad);
	EspSettingsBox(String extValuesFileName,boolean forceLoad,boolean _initSpiff);

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

	String getName(){
		return "espSettingsBox";
	}
	String getSimpleJsonPublishUrl(){
			return "/"+getName()+"/getSimpleJson";
		}
	String getJsonPublishUrl(){
		return "/"+getName()+"/getJson";
	}
	String getSetValueUrl(){
		return "/"+getName()+"/setValue";
	}

	void printSettingsFile();

	boolean isSpiffInitialized();

	void printSpiffsInfo();

	String deviceFirmWareVersion = "v.1.0";
	String DeviceId = "SENS_"+String(ESP.getChipId());
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
	IPAddress serverIp=IPAddress(192, 168, 0, 2);

	boolean beepOnAlert=false;
	boolean alarmSendNotifAlertStart=true;
	boolean alarmSendNotifAlertStop=true;
	boolean alarmPlaySound=true;
	uint16_t alamSoundInterval=30;

	boolean isThingSpeakEnabled=false;
	uint16_t postDataToTSInterval=241;
	String thSkUsrKey="YV1SA7H87PS8RFMU";
	String thSkWKey="V8V5G1W2CACCQOMV";
	String thSkRKey="SPH0UG4JJZC7GDDH";
	int thSkChId=612324;
	String thSkTKey="ZSFBN9V1K2QMB9M1";

	//uint16_t alamNotificationInterval=60;

	boolean isMqttEnabled=false;
	boolean sendItemsToBaseQUeue=false;
	uint16_t postDataToMqttInterval=241;
	String mqtt_server = "m23.cloudmqtt.com";
	String mqtt_user = "tpheglmk";
	String mqtt_pass = "QgYRfVzDhQ31";
	String mqtt_topic="topic/basePublish";
	int mqtt_port = 10186;


	boolean isHttpPostEnabled=false;
	uint16_t postDataToHttpInterval=241;
	IPAddress httpPostIp=IPAddress(192, 168, 0, 2);

	boolean ntpEnabled=false;
	String NTP_poolServerName="europe.pool.ntp.org";
	int NTP_timeOffset=7200;
	int NTP_updateInterval=120;

	ulong NTP_timeTriggerInterval=1000;


private:
	boolean loaded=false;

	boolean spiffInitialized;
	String _fileName;
	String _extFileName;
	JsonObject* extRoot;

	void construct(String extValuesFileName,boolean forceLoad,boolean initSpiff);
	void initSpiff();

	size_t bufferLength=2048;
};




#endif /* LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_ */

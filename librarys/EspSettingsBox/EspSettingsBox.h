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

class EspSettingsBox{

public:
	EspSettingsBox(String fileName);
	EspSettingsBox(String fileName,boolean forceLoad);
	EspSettingsBox(String fileName,String extValuesFileName,boolean forceLoad);

	String getHtmlVal(String key);

	void loadExternalFile();
	void saveExternalFile();
	String getExternalVal(String key);

	IPAddress stringToIp(String str);
	boolean validateIP(String str);
	void printVariablesToSerial();
	void saveSettingsJson();
	void loadSettingsJson();

	String clearNlFromString(String str);

	void printFileToDIsc();
	boolean isTrue(String str);

	String DeviceId = "SENS_"+String(ESP.getChipId());
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

	IPAddress localIp=IPAddress(192, 168, 0, 70);
	IPAddress apIp=IPAddress(192, 168, 4, 1);
	IPAddress gateIp=IPAddress(192, 168, 0, 1);
	IPAddress subnetIp=IPAddress(255, 255, 255, 0);
	IPAddress dnsIp=IPAddress(192, 168, 0, 1);
	IPAddress dnsIp2=IPAddress(192, 168, 0, 1);

	uint8_t refreshInterval=60;
	boolean beepOnAlert=false;

	uint8_t postDataToTSInterval=241;
	String thSkUsrKey="";//="U6KUFVOVLGBA0FD2";
	String thSkWKey="";//="BN4T5Z1M0ZVALWYR";
	String thSkRKey="";//="SB58HIU5UR4F8WOW";
	int thSkChId=0;//=418229;
	String thSkTKey="";//="3L0KJ5J1YO247PFV";

	boolean alarmSendNotifAlertStart=true;
	boolean alarmSendNotifAlertStop=true;
	boolean alarmPlaySound=true;
	uint16_t alamSoundInterval=30;
	//uint16_t alamNotificationInterval=60;

	uint8_t maxDHtTemp=255;
	int8_t minDHtTemp=-120;
	uint8_t maxDHtHum=255;
	uint8_t minDHtHum=0;

	boolean isMqttEnabled=false;
	String mqtt_server = "m23.cloudmqtt.com";
	int mqtt_port = 10186;
	String mqtt_user = "tpheglmk";
	String mqtt_pass = "QgYRfVzDhQ31";
	String mqtt_topic="topic/basePublish";

private:
	String _fileName;
	String _extFileName;
	JsonObject* extRoot;

	void init(String fileName,String extValuesFileName,boolean forceLoad);
};




#endif /* LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_ */

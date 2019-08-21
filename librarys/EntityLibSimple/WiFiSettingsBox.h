/*
 * ServerSettingsBox.h
 *
 *  Created on: 24 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIB_WSERVERSETTINGSBOX_H_
#define LIBRARIES_ENTITYLIB_WSERVERSETTINGSBOX_H_
//device info
#define _DEVICE_ID "deviceId"
#define _DEVICE_FIRMWARE "deviceFirmware"
#define _DEVICE_DESCR "deviceDescr"
//network settings
#define _SSID "ssid"
#define _PASS "pass"

#define _SSID_AP "ssidAP"
#define _PASS_AP "passAP"

#define _IS_AP "isAp"
#define _IS_STAT_IP "isStatIp"
#define _DISCONNECT_ON_START_IF_CONN "disStIfCon"

#define _IP "ip"
#define _IP_AP "ipAP"
#define _IP_GATEWAY "gateway"
#define _IP_SUBNET "subnet"
#define _IP_DNS "dns"
#define _IP_DNS2 "dns2"
#define _SMART_HOUSE_SERVER_IP "smrtServIp"
//access settings
#define _USER_LOGIN "userLogin"
#define _USER_PASS "userPass"
#define _ADMIN_LOGIN "adminLogin"
#define _ADMIN_PASS "adminPass"
#define _REFRESH_INTERVAL "interval"
// AP default settings
#define _AP_CHANNEL  "apCH"
#define _AP_HIDDEN   "apH"
#define _AP_MAXCONN  "apMC"

#define _DEFAULT_AP_PASSWORD ""
#define _DEFAULT_AP_CHANNEL  1
#define _DEFAULT_AP_HIDDEN   0
#define _DEFAULT_AP_MAXCONN  4

#include <Entity.h>
#include <IPAddress.h>
#include <sstream>
#include "ObjectUtils.h"

class ServerSettingsBox: public Entity {
public:
	ServerSettingsBox(const char* firmware) :
			Entity(GROUP_SETTINGS, "WiFiSettings",
					"Wifi Server Device settings") {
		this->_firmware = firmware;
		this->_devId = "ESP_Dev_ID";

		this->_ssidAp = deviceId();
	}
	virtual ~ServerSettingsBox() {
	}

	virtual void init() override {
		//String devIdTmp="ESP_"+String(ESP.getChipId());
		//this->devId=strdup(devIdTmp.c_str());

	}

	void resetToAp() {
		this->_isAp = 1;
		this->_isStatIp = 1;
	}

	void printApConf() {
		Serial.print(FPSTR(" ssidAP="));
		Serial.print(ssidAP());
		Serial.print(FPSTR(" passwordAP="));
		Serial.print(passwordAP());
		Serial.print(FPSTR(" chann="));
		Serial.print(channelAP());
		Serial.print(FPSTR(" hidden="));
		Serial.print(hiddenAP());
		Serial.print(FPSTR(" maxCon="));
		Serial.print(maxConnAP());
		Serial.print(FPSTR(" ipAp="));
		Serial.print(IPaccessPoint());
		Serial.print(FPSTR(" ipStatic="));
		Serial.print(isStaticIp());
		Serial.print(FPSTR(" gateway="));
		Serial.print(gateway());
		Serial.print(FPSTR(" subnet="));
		Serial.print(subnet());
		Serial.print(FPSTR(" dns="));
		Serial.print(dns());
		Serial.print(FPSTR(" dns2="));
		Serial.print(dns2());
		Serial.print(FPSTR(" smrtServIp="));
		Serial.print(smartHouseServerIp());
		Serial.println();
	}

	void printStaConf() {
		Serial.print(FPSTR(" ssid="));
		Serial.print(ssid());
		Serial.print(FPSTR(" pass="));
		Serial.print(password());
		Serial.print(FPSTR(" ip="));
		Serial.print(IPstation());
		Serial.print(FPSTR(" ipStatic="));
		Serial.print(isStaticIp());
		Serial.print(FPSTR(" gateway="));
		Serial.print(gateway());
		Serial.print(FPSTR(" subnet="));
		Serial.print(subnet());
		Serial.print(FPSTR(" dns="));
		Serial.print(dns());
		Serial.print(FPSTR(" dns2="));
		Serial.print(dns2());
		Serial.println();
	}

	const char* ssid() {
		return this->_ssid;
	}
	const char* password() {
		return this->_pass;
	}

	const char* ssidAP() {
		return this->_ssidAp;
	}
	const char* passwordAP() {
		return this->_passAp;
	}
	int channelAP() {
		return this->_apCH;
	}
	int hiddenAP() {
		return this->_apH;
	}
	int maxConnAP() {
		return this->_apMC;
	}

	int refreshInterval() {
		return this->_interval;
	}

	bool isAccessPoint() {
		return this->_isAp;
	}
	bool isStaticIp() {
		return this->_isStatIp;
	}
	bool isDisconnectOnStartIfConn() {
		return this->_disStIfCon;
	}

	IPAddress IPstation() {
		return this->_ip;
	}
	IPAddress IPaccessPoint() {
		return this->_ipAP;
	}
	IPAddress gateway() {
		return this->_gateway;
	}
	IPAddress subnet() {
		return this->_subnet;
	}
	IPAddress dns() {
		return this->_dns;
	}
	IPAddress dns2() {
		return this->_dns2;
	}
	IPAddress smartHouseServerIp() {
		return this->_smrtServIp;
	}

	const char* userLogin() {
		return this->_userLogin;
	}
	const char* userPassword() {
		return this->_userPassword;
	}

	const char* adminLogin() {
		return this->_adminLogin;
	}
	const char* adminPassword() {
		return this->_adminPassword;
	}

	const char* deviceId() {
		return this->_devId;
	}
	const char* deviceFirmWare() {
		return this->_firmware;
	}
	char* deviceDescr() {
		return this->_deviceDescr;
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		setJsonField(response, _DEVICE_ID, this->_devId);
		setJsonField(response, _DEVICE_FIRMWARE, this->_firmware);
		setJsonField(response, _DEVICE_DESCR, this->_firmware);

		setJsonField(response, _SSID, this->_ssid);
		setJsonField(response, _PASS, this->_pass);

		setJsonField(response, _SSID_AP, this->_ssidAP);
		setJsonField(response, _PASS_AP, this->_firmware);


//network settings

#define  ""
#define  "passAP"

#define _IS_AP "isAp"
#define _IS_STAT_IP "isStatIp"
#define _DISCONNECT_ON_START_IF_CONN "disStIfCon"

#define _IP "ip"
#define _IP_AP "ipAP"
#define _IP_GATEWAY "gateway"
#define _IP_SUBNET "subnet"
#define _IP_DNS "dns"
#define _IP_DNS2 "dns2"
#define _SMART_HOUSE_SERVER_IP "smrtServIp"
//access settings
#define _USER_LOGIN "userLogin"
#define _USER_PASS "userPass"
#define _ADMIN_LOGIN "adminLogin"
#define _ADMIN_PASS "adminPass"
#define _REFRESH_INTERVAL "interval"
// AP default settings
#define _AP_CHANNEL  "apCH"
#define _AP_HIDDEN   "apH"
#define _AP_MAXCONN  "apMC"
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
	}

	virtual void doLoad(JsonObject& jsonFromFile) override {
	}

	virtual void doSave(JsonObject& jsonToFile) override {
	}

protected:
	const char* _devId;
	const char* _firmware;
	char* _ssid = "balabasKiev5";
	char* _pass = "wuWylKegayg2wu22";
	char* _ssidAp = "";
	char* _passAp = "";bool _isAp = 0;bool _isStatIp = 0;bool _disStIfCon = 0;

	uint8_t _apCH = 1;
	uint8_t _apH = 0;
	uint8_t _apMC = 4;

	IPAddress _ip = "192.168.0.120";
	IPAddress _ipAP = "192.168.0.4";
	IPAddress _gateway = "192.168.0.1";
	IPAddress _subnet = "255.255.255.0";
	IPAddress _dns = "192.168.0.1";
	IPAddress _dns2 = "192.168.0.1";
	IPAddress _smrtServIp = "192.168.0.5";
	char* _userLogin = "";
	char* _userPassword = "";
	char* _adminLogin = "admin";
	char* _adminPassword = "admin";
	uint16_t _interval = 60;
	char* _deviceDescr = "Default Device description";
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_ */

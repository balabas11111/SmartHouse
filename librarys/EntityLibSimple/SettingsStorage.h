/*
 * ServerSettingsBox.h
 *
 *  Created on: 24 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIB_WIFI_WSERVERSETTINGSBOX_H_
#define LIBRARIES_ENTITYLIB_WIFI_WSERVERSETTINGSBOX_H_

#define _DEVICE "device"
#define _INFO "info"
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
#define _SMART_HOUSE_SERVER_ADDR "smrtServAddr"
#define _SMART_HOUSE_SERVER_URL "smrtServUrl"
#define _SMART_HOUSE_SERVER_KEY "smrtServKey"
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

#define _MASKED_VALUE "******"

#define _WIFI_SETTINGS_DEF_NAME "DeviceSettings"
#define _WIFI_SETTINGS_DEF_DESCR "Wifi Server Device settings"

//device statuses
#define DEVICE_STATUS_INITIALIZING "Initializing"
#define DEVICE_STATUS_LOAD_ENTITIES "Loading Entities"
#define DEVICE_STATUS_CONNECTING "Connecting WiFi"
#define DEVICE_STATUS_DISCONNECTED "DisConnected"
#define DEVICE_STATUS_CONNECTED "Connected"
#define DEVICE_STATUS_DEPLOYING "Deploy urls"
#define DEVICE_STATUS_REGISTERING "Registering"
#define DEVICE_STATUS_UPDATE_SENSORS "Update sensors"
#define DEVICE_STATUS_READY "Ready"
#define DEVICE_STATUS_RESTARTING "Restart"

#include "Arduino.h"
#include "ArduinoJson.h"
#include <Entity.h>
#include <IPAddress.h>
#include <sstream>
#include <utils/DeviceUtils.h>
#include "utils/HashUtils.h"
#include <DeviceConfig.h>
#include <DeviceConstants.h>
#include <EntityDescriptor.h>
#include "Emoji.h"
#include "functional"

class SettingsStorage: public Entity {
public:
	SettingsStorage(const char* firmware, char* description, const char* emoji = EMOJI_PAGER) :
			Entity(GROUP_SETTINGS, _WIFI_SETTINGS_DEF_NAME,
					(char*)_WIFI_SETTINGS_DEF_DESCR) {
		this->_firmware = firmware;
		this->_deviceDescr = description;
		this->_emoji = emoji;

		this->noGroupGet = true;
	}
	virtual ~SettingsStorage() {
	}

	void initDeviceId(){
#ifdef ESP8266
		String devIdTmp="ESP_"+String(ESP.getChipId());
#endif
#ifdef ESP32
		String devIdTmp="ESP_"+String(ESP.getChipRevision());
#endif
		this->_devId=strdup(devIdTmp.c_str());

		this->_ssidAp = strdup(deviceId());

		Serial.println(FPSTR("---------Init device---------"));
		Serial.print(FPSTR("DeviceId = "));
		Serial.print(this->deviceId());
		Serial.print(FPSTR("; firmware = "));
		Serial.print(this->deviceFirmWare());
		Serial.print(FPSTR("; descr = "));
		Serial.println(this->deviceDescr());
	}

	void setCurrentIp(char* ip) {
		this->currentIp = ip;
	}

	char* getCurrentIp() {
		return this->currentIp;
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
		Serial.print(this->_gateway);
		Serial.print(FPSTR(" subnet="));
		Serial.print(this->_subnet);
		Serial.print(FPSTR(" dns="));
		Serial.print(this->_dns);
		Serial.print(FPSTR(" dns2="));
		Serial.print(this->_dns2);
#ifndef SETTINGS_SERVER_CONNECTION_DISABLED
		Serial.print(FPSTR(" smrtServUrl="));
		Serial.print(this->_smrtServAddr);
#endif
		Serial.println();
	}

	void printStaConf() {
		Serial.print(FPSTR(" ssid="));
		Serial.print(ssid());
		Serial.print(FPSTR(" pass="));
		Serial.print(password());
		Serial.print(FPSTR(" ip="));
		Serial.print(this->_ip);
		Serial.print(FPSTR(" ipStatic="));
		Serial.print(isStaticIp());
		Serial.print(FPSTR(" gateway="));
		Serial.print(this->_gateway);
		Serial.print(FPSTR(" subnet="));
		Serial.print(this->_subnet);
		Serial.print(FPSTR(" dns="));
		Serial.print(this->_dns);
		Serial.print(FPSTR(" dns2="));
		Serial.print(this->_dns2);
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
	bool isResetToAp(){
		return false;
	}
	bool isDisconnectOnStartIfConn() {
		return this->_disStIfCon;
	}

	IPAddress IPstation() {
		return (new IPAddress())->fromString(this->_ip);
	}
	IPAddress IPaccessPoint() {
		return  (new IPAddress())->fromString(this->_ipAP);
	}
	IPAddress gateway() {
		return  (new IPAddress())->fromString(this->_gateway);
	}
	IPAddress subnet() {
		return  (new IPAddress())->fromString(this->_subnet);
	}
	IPAddress dns() {
		return  (new IPAddress())->fromString(this->_dns);
	}
	IPAddress dns2() {
		return  (new IPAddress())->fromString(this->_dns2);
	}

	char* userLogin() {
		return this->_userLogin;
	}
	char* userPassword() {
		return this->_userPassword;
	}

	char* adminLogin() {
		return this->_adminLogin;
	}
	char* adminPassword() {
		return this->_adminPassword;
	}

	const char* deviceId() {
		return this->_devId;
	}
	const char* deviceEmoji() {
		return this->_emoji;
	}
	const char* deviceFirmWare() {
		return this->_firmware;
	}
	char* deviceDescr() {
		return this->_deviceDescr;
	}

	bool hasAdminPassword(){
		return strcmp(_adminPassword,"")!=0;
	}

	bool hasUserPassword(){
		return strcmp(_userPassword,"")!=0;
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		UNUSED(params);
		itemsToJson(response, true);
	}

	void doAppendFieldsSwg(JsonObject& swgJson){
		EntityDescriptor::appendSwgFieldString(swgJson, ENTITY_FIELD_DESCRIPTION, EDC_DESCR_SETTINGS);
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
		UNUSED(response);
		markEntityAsChangedIfTrue(jsonToItems(params));
	}

	virtual void doLoad(JsonObject& jsonFromFile) override {
		jsonToItems(jsonFromFile);
	}

	virtual void doSave(JsonObject& jsonToFile) override {
		itemsToJson(jsonToFile, false);
	}

	void addDeviceInfoToJson(JsonObject& json){
		JsonObject& info = JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(json,
												_DEVICE, _INFO);
		JsonObjectUtil::setField(info, _DEVICE_ID, deviceId());
		JsonObjectUtil::setField(info, _DEVICE_FIRMWARE, deviceFirmWare());
		JsonObjectUtil::setField(info, _DEVICE_DESCR, deviceDescr());
		JsonObjectUtil::setField(info, EDC_FIELD_EMOJI, deviceEmoji());
	}

	const String& getServerAuthorization() const {
		return serverAuthorization;
	}

	void setServerAuthorization(String serverAuthorization) {
		this->serverAuthorization = serverAuthorization;
	}

	const String& getDeviceAuthorization() const {
		return deviceAuthorization;
	}

	void setDeviceAuthorization(String deviceAuthorization) {
		this->deviceAuthorization = deviceAuthorization;
	}

	void generateAuthorization(String& tempServerKey, char* tempDevKey) {
		Serial.println(FPSTR("generate Authorization-------------------------"));
		String serverKeyHash = encode_sha1(smartServerKey());
		String tempDeviceKey = tempDevKey;

		Serial.print(FPSTR("tempDeviceKey="));
		Serial.println(tempDeviceKey);
		Serial.print(FPSTR("tempServerKey="));
		Serial.println(tempServerKey);
		String tmp = serverKeyHash + tempServerKey + tempDeviceKey;

		Serial.print(FPSTR("unhashedDeviceToken="));
		Serial.println(tmp);

		setDeviceAuthorization(encode_sha1(tmp));

		String tmp2 = tempServerKey + smartServerKey() + deviceId();
		setServerAuthorization(encode_sha1(tmp2));

		Serial.print(FPSTR("unhashedServerToken="));
		Serial.println(tmp2);

		Serial.print(FPSTR("deviceHash="));
		Serial.println(getDeviceAuthorization());
		Serial.print(FPSTR("serverHash="));
		Serial.println(getServerAuthorization());
		Serial.println(FPSTR("...done"));
	}

	char* smartServerAddr(){
		return this->_smrtServAddr;
	}

	char* smartServerKey(){
		return this->_smrtServKey;
	}

	char* getDeviceStatus() const {
		return deviceStatus;
	}

	void setDeviceStatus(char* deviceStatus) {
		this->deviceStatus = deviceStatus;
		if (this->onDeviceStatusChanged != nullptr){
			onDeviceStatusChanged();
		}
	}

	std::function<void(void)> getOnDeviceStatusChanged() {
		return onDeviceStatusChanged;
	}

	void setOnDeviceStatusChanged(
			std::function<void(void)> onDeviceStatusChanged = nullptr) {
		this->onDeviceStatusChanged = onDeviceStatusChanged;
	}

protected:
	char* _devId = (char*)" ";
	const char* _firmware;
	char* _deviceDescr = (char*)"Default Device description";
	const char* _emoji = EMOJI_PAGER;

	char* _ssid = (char*)SETTINGS_WIFI_SSSID;
	char* _pass = (char*)SETTINGS_WIFI_PASSWORD;
	char* _ssidAp = (char*)"";
	char* _passAp = (char*)"";
	bool _isAp = 0;bool _isStatIp = 0;bool _disStIfCon = 0;

	uint8_t _apCH = 1;
	uint8_t _apH = 0;
	uint8_t _apMC = 4;

	char* _ip = (char*)"192,168,0,120";
	char* _ipAP = (char*)"192,168,0,4";
	char* _gateway = (char*)"192,168,0,1";
	char* _subnet = (char*)"255,255,255,0";
	char* _dns = (char*)"192,168,0,1";
	char* _dns2 = (char*)"192,168,0,1";

	char* _userLogin = (char*)"";
	char* _userPassword = (char*)"";
	char* _adminLogin = (char*)"admin";
	char* _adminPassword = (char*)"admin";
	uint16_t _interval = 20;

	char* currentIp = (char*)EMPTY_LINE;

	//Device current security information
	String serverAuthorization;
	String deviceAuthorization;

	char* _smrtServAddr = (char*)SETTINGS_SMART_SERVER_ADDRESS;
	char* _smrtServKey = (char*)SETTINGS_SMART_SERVER_KEY;


	std::function<void(void)> onDeviceStatusChanged = nullptr;
	char* deviceStatus = (char*)DEVICE_STATUS_INITIALIZING;

	bool jsonToItems(JsonObject& json){
		bool chg = false;

		chg = getKeyValueIfExistsAndNotEquals(json, _DEVICE_DESCR, &this->_deviceDescr)?true:chg;

		chg = getKeyValueIfExistsAndNotEquals(json, _SSID, &this->_ssid)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _PASS, &this->_pass)?true:chg;

		chg = getKeyValueIfExistsAndNotEquals(json, _SSID_AP, &this->_ssidAp)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _PASS_AP, &this->_passAp)?true:chg;

		chg = getKeyValueIfExistsAndNotEquals(json, _IS_AP, &this->_isAp)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _IS_STAT_IP, &this->_isStatIp)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _DISCONNECT_ON_START_IF_CONN, &this->_disStIfCon)?true:chg;

		chg = getKeyValueIfExistsAndNotEquals(json, _IP, &this->_ip)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _IP_AP, &this->_ipAP)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _IP_GATEWAY, &this->_gateway)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _IP_SUBNET, &this->_subnet)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _IP_DNS, &this->_dns)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _IP_DNS2, &this->_dns2)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _SMART_HOUSE_SERVER_ADDR, &this->_smrtServAddr)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _SMART_HOUSE_SERVER_KEY, &this->_smrtServKey)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _USER_LOGIN, &this->_userLogin)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _USER_PASS, &this->_userPassword)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _ADMIN_LOGIN, &this->_adminLogin)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _ADMIN_PASS, &this->_adminPassword)?true:chg;

		chg = getKeyValueIfExistsAndNotEquals(json, _REFRESH_INTERVAL, &this->_interval)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _AP_CHANNEL, &this->_apCH)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _AP_HIDDEN, &this->_apH)?true:chg;
		chg = getKeyValueIfExistsAndNotEquals(json, _AP_MAXCONN, &this->_apMC)?true:chg;

		return chg;
	}

	void itemsToJson(JsonObject& json, bool mask) {
		setJsonField(json, _DEVICE_ID, this->_devId);
		setJsonField(json, _DEVICE_FIRMWARE, this->_firmware);
		setJsonField(json, _DEVICE_DESCR, this->_deviceDescr);

		setJsonField(json, _SSID, this->_ssid);
		setJsonField(json, _PASS, mask?_MASKED_VALUE:this->_pass);

		setJsonField(json, _SSID_AP, this->_ssidAp);
		setJsonField(json, _PASS_AP, mask?_MASKED_VALUE:this->_passAp);

		setJsonField(json, _IS_AP, this->_isAp);
		setJsonField(json, _IS_STAT_IP, this->_isStatIp);
		setJsonField(json, _DISCONNECT_ON_START_IF_CONN, this->_disStIfCon);

		setJsonField(json, _IP, this->_ip);
		setJsonField(json, _IP_AP, this->_ipAP);
		setJsonField(json, _IP_GATEWAY, this->_gateway);
		setJsonField(json, _IP_SUBNET, this->_subnet);
		setJsonField(json, _IP_DNS, this->_dns);
		setJsonField(json, _IP_DNS2, this->_dns2);
		setJsonField(json, _SMART_HOUSE_SERVER_ADDR, this->_smrtServAddr);
		setJsonField(json, _SMART_HOUSE_SERVER_KEY, mask?_MASKED_VALUE:this->_smrtServKey);
		setJsonField(json, _USER_LOGIN, this->_userLogin);
		setJsonField(json, _USER_PASS, mask?_MASKED_VALUE:this->_userPassword);
		setJsonField(json, _ADMIN_LOGIN, this->_adminLogin);
		setJsonField(json, _ADMIN_PASS, mask?_MASKED_VALUE:this->_adminPassword);
		setJsonField(json, _REFRESH_INTERVAL, this->_interval);
		setJsonField(json, _AP_CHANNEL, this->_apCH);
		setJsonField(json, _AP_HIDDEN, this->_apH);
		setJsonField(json, _AP_MAXCONN, this->_apMC);
	}

};

#endif /* LIBRARIES_DEVICELIB_ENTITY_SETTINGS_WIFI_SERVERSETTINGSBOX_H_ */

/*
 * SmartHouseServerHelper.h
 *
 *  Created on: 21 сент. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_WIFI_SMARTHOUSESERVERHELPER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_WIFI_SMARTHOUSESERVERHELPER_H_

#define KEY_IP "ip"
#define KEY_DATA_URL "dataUrl"
#define KEY_ROOT_URL "rootUrl"
#define KEY_SERVER_KEY "serverKey"
#define KEY_DEVICE_KEY "deviceKey"

#define KEY_SERVER_TOKEN "sToken"
#define KEY_DEVICE_TOKEN "dToken"

#define SMART_HOUSE_SERVER_URL_BASE "/api/v1"
#define SMART_HOUSE_SERVER_URL_REGISTER        "/devices/register/"
#define SMART_HOUSE_SERVER_URL_ON_DATA_CHANGED "/devices/data/"
#define SMART_HOUSE_SERVER_URL_ON_DATA_CHANGED_GET "/devices/data?deviceId="
#define SMART_HOUSE_SERVER_URL_SENDDATA        "/data/"

#define SMART_HOUSE_SERVER_URL_REGISTERSTR    "/devices/registerStr/"
#define SMART_HOUSE_SERVER_URL_REGISTERSIMPLE    "/devices/registerSimple/"

#define MAX_CONNECTION_ERRORS 3
#define MAX_REGISTER_ERRORS 1

#define MAX_CONNECTION_ERRORS_NO_SERVER_FOUND_TIMEOUT 3600000
#define MAX_CONNECTION_ERRORS_REACHED_TIMEOUT 30000
#define MAX_REGISTER_ERRORS_REACHED_TIMEOUT 30000

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ObjectUtils.h>
#include <JsonObjectUtil.h>
#include <Hash.h>
#include <HttpUtils.h>

#include <WiFi/NetConstants.h>
#include <EntityManager.h>

#include <EntityJsonRequestResponse.h>

class SmartHouseServerHelper {
public:
	SmartHouseServerHelper(EntityManager* conf);
	virtual ~SmartHouseServerHelper(){};

	void loop();

	void triggerOnServerRegister();
	void triggerOnServerDeviceChanged(bool addData = false);

	void createDeviceRegistrationRequest(JsonObject& request, bool addUrl = false);
	void createDeviceDataChangedRequest(JsonObject& request, bool addUrl = false);

	bool validateServersKeyHash(char* serverKey);

	char* getDeviceKeyHashed();
	bool validateDeviceKeyHash(String& devicekey);

	bool connectedAndRegistered();

private:

	void executeOnServerRegisterIfTriggered();
	void executeOnServerDataChangedIfTriggered();

	int sendDataUpdatedRequestPost();
	int sendDataUpdatedRequestGet();

	char* getServerKeyHashed();

	char* getDeviceBaseUrl();
	char* getDeviceRootUrl();
	char* getDeviceDataUrl();

	char* getServerRegisterUrl();
	char* getServerDataChangedUrl();

	String& getServerDataChangedUrlStr();
	String& getServerDataChangedUrlStrGet();

	void generateDeviceToken();

	bool triggeredOnServerRegister = false;
	bool triggeredOnServerDeviceChanged = false;
	bool addDataToChangedRequest = false;

	bool serverRegisterRuns = false;
	bool serverDataChangedRuns = false;

	int registerErrors = 0;
	int connectionErrors = 0;

	bool timeoutExecuteRegister = false;
	bool timeoutExecuteUpdate = false;

	unsigned long nextRegisterExecution = 0;
	unsigned long nextUpdateExecution = 0;

	bool registered = false;

	EntityManager* manager;

	char* serverToken = nullptr;
	char* deviceToken = nullptr;

	char* deviceBaseUrl = nullptr;
	char* deviceRootUrl = nullptr;
	char* deviceDataUrl = nullptr;

	char* serverRegisterUrl = nullptr;
	char* serverDataChangedUrl = nullptr;

	bool usePostAsDC = false;

	String serverDatachangeUrlStr;
	String serverDatachangeUrlGet;
	bool sdcg = false;
	bool sdcgg = false;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_WIFI_SMARTHOUSESERVERHELPER_H_ */

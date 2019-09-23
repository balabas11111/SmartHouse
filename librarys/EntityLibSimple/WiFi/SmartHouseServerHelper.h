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
#define SMART_HOUSE_SERVER_URL_SENDDATA        "/data/"

#define SMART_HOUSE_SERVER_URL_REGISTERSTR    "/devices/registerStr/"
#define SMART_HOUSE_SERVER_URL_REGISTERSIMPLE    "/devices/registerSimple/"

#define MAX_CONNECTION_ERRORS 3
#define MAX_REGISTER_ERRORS 3

#define MAX_CONNECTION_ERRORS_REACHED_TIMEOUT 30000
#define MAX_REGISTER_ERRORS_REACHED_TIMEOUT 30000

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ObjectUtils.h>
#include <JsonObjectUtil.h>
#include <Hash.h>
#include <HttpUtils.h>

#include <WiFi/NetConstants.h>
#include <WiFi/WiFiSettingsBox.h>

#include <EntityJsonRequestResponse.h>

class SmartHouseServerHelper {
public:
	SmartHouseServerHelper(WiFiSettingsBox* conf);
	virtual ~SmartHouseServerHelper(){};

	void loop();

	int sendRegisterRequest(EntityJsonRequestResponse* req);
	int sendDataUpdateRequest(EntityJsonRequestResponse* req);
	int sendGetDataRequest(EntityJsonRequestResponse* req);

	void triggerOnServerRegister();
	void triggerOnServerDeviceDataChanged();

	void prepareRegisterRequestTest(JsonObject& json, const char* urlPath);
	void prepareRegisterRequest(JsonObject& json);
	void prepareSendDataUpdateRequest(JsonObject& json);

	void addDeviceInfoToJson(JsonObject& json);

	void createDeviceRegistrationRequest(JsonObject& request);
	void createDeviceDataChangedRequest(JsonObject& request);

	char* getServersToken();
	void setServersToken(String& serverToken);
	bool validateServersToken(String& serverToken);

	char* getDeviceKeyHashed();
	void generateDeviceToken();
	bool validateDeviceToken(String& deviceToken);

	bool connectedAndRegistered();

protected:

	void executeOnServerRegisterIfTriggered();
	void executeOnServerDataChangedIfTriggered();

	void getServerBaseUrl(String& baseUrl);
	void getServerKeyHashed(String& serverKeyHash);
	char* getServerKeyHashed();

	void addDeviceTokenToJson(JsonObject& json);

	void addServerTokenToJson(JsonObject& json);
	void addServerKeyHashToJson(JsonObject& json);
	void addServerUrlToJson(JsonObject& json, const char* serviceUrl, const char* jsonKey);
	void addDeviceUrlsToJson(JsonObject& json);

	char* getDeviceBaseUrl();
	char* getDeviceRootUrl();
	char* getDeviceDataUrl();

private:
	bool triggeredOnServerRegister = false;
	bool triggeredOnServerDataChanged = false;

	bool serverRegisterRuns = false;
	bool serverDataChangedRuns = false;

	int registerErrors = 0;
	int connectionErrors = 0;

	bool timeoutExecuteRegister = false;
	bool timeoutExecuteUpdate = false;

	unsigned long nextRegisterExecution = 0;
	unsigned long nextUpdateExecution = 0;

	bool registered = false;

	WiFiSettingsBox* conf;

	char* serverToken = nullptr;
	char* deviceToken = nullptr;

	char* deviceBaseUrl = nullptr;
	char* deviceRootUrl = nullptr;
	char* deviceDataUrl = nullptr;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_WIFI_SMARTHOUSESERVERHELPER_H_ */

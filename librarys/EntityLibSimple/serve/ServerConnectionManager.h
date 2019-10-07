/*
 * SecurityManager.h
 *
 *  Created on: 28 сент. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_SERVE_SERVERCONNECTIONMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_SERVE_SERVERCONNECTIONMANAGER_H_

#define SMART_HOUSE_SERVER_URL_BASE "/api/v1"
#define SMART_HOUSE_SERVER_URL_PING        "/security/online/"
#define SMART_HOUSE_SERVER_URL_REGISTER        "/security/register/"
#define SMART_HOUSE_SERVER_URL_ON_DATA_CHANGED "/devices/data/"
#define SMART_HOUSE_SERVER_URL_ON_DATA_CHANGED_GET "/devices/data?deviceId="

#include "Arduino.h"
#include "ArduinoJson.h"
#include "Hash.h"
#include "ESP8266HTTPClient.h"
#include "WiFi/WiFiSettingsBox.h"
#include "WiFi/NetConstants.h"
#include "EntityJsonRequestResponse.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#ifndef SERVER_CONNECTION_PING_FAILED_TIMEOUT
	#define SERVER_CONNECTION_PING_FAILED_TIMEOUT 10000
#endif

#ifndef SERVER_CONNECTION_REGISTRATION_FAILED_FIRST_TIMEOUT
	#define SERVER_CONNECTION_REGISTRATION_FAILED_FIRST_TIMEOUT 60000
#endif

#ifndef SERVER_CONNECTION_REGISTRATION_FAILED_NEXT_COUNT
	#define SERVER_CONNECTION_NEXT_REGISTRATION_NEXT_COUNT 1
#endif

#ifndef SERVER_CONNECTION_REGISTRATION_FAILED_NEXT_TIMEOUT
	#define SERVER_CONNECTION_REGISTRATION_FAILED_NEXT_TIMEOUT 600000
#endif

#ifndef SERVER_CONNECTION_REGISTRATION_FAILED_MIN_HEAP_TO_RESTART
	#define SERVER_CONNECTION_REGISTRATION_FAILED_MIN_HEAP_TO_RESTART 18000
#endif

#ifndef SERVER_CONNECTION_DATA_UPDATE_REQUEST_FAILED_TIMEOUT
	#define SERVER_CONNECTION_DATA_UPDATE_REQUEST_FAILED_TIMEOUT 10000
#endif



class ServerConnectionManager {
public:
	ServerConnectionManager(WiFiSettingsBox* conf);
	virtual ~ServerConnectionManager(){};

	void init();

	void triggerServerPing(bool trigger = true);
	void triggerRegisterOnServer(bool trigger = true);

	void triggerDataChanged(bool trigger = true);

	void sendDataChangedPostMethod(JsonObject& data);

	void loop();
protected:
	void generateTempDeviceKey();
	void generateServerUrls();

	void generateAuthorization(String& tempServerKey);

	void sendPingRequest();
	void sendRegisterRequest();
	void sendDataChangedRequest();

	bool isConnected();
	bool isRegistered();

	bool isRequestEnabled();

	void setRequestPostPoned(unsigned long delay);
	void checkRequestPostPonedTime();

private:
	bool initComplete = false;
	bool requestPostPoned = false;

	unsigned long nextReqTime = 0;

	bool triggeredServerPing = false;
	bool triggeredServerRegister = false;
	bool triggeredDataChange = false;

	bool serverPinged = false;
	bool deviceRegistered = false;

	bool runsPingRequest = false;
	bool runsRegisterRequest = false;
	bool runsDataChangeRequest = false;

	String serverKeyHash;

	char* tempDeviceKey = (char*)"";
	char* deviceAuthorization = (char*)"";

	String urlPing;
	String urlRegister;
	String urlData;

	WiFiSettingsBox* conf;

	unsigned char registrationFailures = 0;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_SERVE_SERVERCONNECTIONMANAGER_H_ */

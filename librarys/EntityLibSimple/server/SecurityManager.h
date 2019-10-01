/*
 * SecurityManager.h
 *
 *  Created on: 28 сент. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_SERVER_SECURITYMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_SERVER_SECURITYMANAGER_H_

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

class SecurityManager {
public:
	SecurityManager(WiFiSettingsBox* conf);
	virtual ~SecurityManager(){};

	void init();

	void triggerServerPing(bool trigger = true);
	void triggerRegisterOnServer(bool trigger = true);

	void triggerDataChanged(bool trigger = true);

	void sendDataChangedPostMethod(JsonObject& data);

	void loop();
protected:
	void generateTempDeviceKey();
	void generateServerUrls();

	void generateDeviceAuthorization(String& tempServerKey);
	void generateServerAuthorization(String& tempServerKey);

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
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_SERVER_SECURITYMANAGER_H_ */

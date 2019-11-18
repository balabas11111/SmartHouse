/*
 * ServerConnector.h
 *
 *  Created on: 3 ����. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_SERVE_SERVERCONNECTOR_H_
#define LIBRARIES_ENTITYLIBSIMPLE_SERVE_SERVERCONNECTOR_H_

#include "Arduino.h"
#include "ArduinoJson.h"
#include "utils/HashUtils.h"

#include "SettingsStorage.h"
#include "DeviceConfig.h"
#include "DeviceConstants.h"
#include <functional>
#include "WiFi/HttpConstants.h"
#include "EntityJsonRequestResponse.h"
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include "ESP8266HTTPClient.h"
#endif

#ifdef ESP32
#include <WiFi.h>
#include "HTTPClient.h"
#endif

#define SMART_HOUSE_SERVER_URL_REGISTER            "/api/v1/devices/register?deviceId="
#define SMART_HOUSE_SERVER_URL_DATA_CHANGED        "/api/v1/devices/dataChanged?deviceId="

#define LAST_FROM_SERVER_REQUEST_TIMED_OUT         90000

class ServerConnector {
public:
	ServerConnector(SettingsStorage* conf, std::function<void(void)> onServerRegistered);
	virtual ~ServerConnector(){};

	void triggerCheckRegistrationOnServer();

	void dispatchDataChanged();

	void loop();

	bool isLastRequestFromServerTimedOut(bool shrReceived, unsigned long lastFromServerRequestTime);

	bool isTriggeredCheckRegistrationOnServer() const {
		return triggeredCheckRegistrationOnServer;
	}

private:
	bool initComplete = false;
	SettingsStorage* conf;

	String urlRegister;
	String urlDataChanged;

	int registrationFailures = 0;

	bool deviceRegistered = false;
	bool requestPostPoned = false;
	bool requestRuns = false;

	bool triggeredCheckRegistrationOnServer = false;
	unsigned long nextReqTime =0;

	void checkRegistrationOnServer();
	void setRequestPostPoned(unsigned long delay);
	void checkCancelRequestPostPoned();

	bool isConnected();
	bool isRequestEnabled();

	std::function<void(void)> onServerRegistered;

	uint8_t errorCount = 0;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_SERVE_SERVERCONNECTOR_H_ */

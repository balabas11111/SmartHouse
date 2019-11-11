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
#include "Hash.h"
#include "ESP8266HTTPClient.h"
#include "SettingsStorage.h"
#include "DeviceConfig.h"
#include "DeviceConstants.h"
#include <functional>
#include "WiFi/HttpConstants.h"
#include "EntityJsonRequestResponse.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SMART_HOUSE_SERVER_URL_REGISTER            "/api/v1/devices/register?deviceId="
#define SMART_HOUSE_SERVER_URL_DATA_CHANGED        "/api/v1/devices/dataChanged?deviceId="

class ServerConnector {
public:
	ServerConnector(SettingsStorage* conf, std::function<void(void)> onServerRegistered);
	virtual ~ServerConnector(){};

	void triggerCheckConnection();

	void checkConnection();

	void dispatchDataChanged();

	void loop();

	bool isConnected();

	bool isRequestEnabled();

private:
	bool initComplete = false;
	SettingsStorage* conf;

	String urlRegister;
	String urlDataChanged;

	int registrationFailures = 0;

	bool deviceRegistered = false;
	bool requestPostPoned = false;
	bool requestRuns = false;

	bool triggeredCheckConnectionRequest = false;
	unsigned long nextReqTime =0;

	void setRequestPostPoned(unsigned long delay);
	void checkCancelRequestPostPoned();

	std::function<void(void)> onServerRegistered;

	uint8_t errorCount = 0;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_SERVE_SERVERCONNECTOR_H_ */

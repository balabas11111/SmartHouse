/*
 * WiFiServerManager.h
 *
 *  Created on: Aug 22, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_WIFI_WIFISERVERMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_WIFI_WIFISERVERMANAGER_H_

#include <Arduino.h>
#include <FS.h>
#include <ArduinoJson.h>

#include <utils/FileUtils.h>
#include <SettingsStorage.h>
#include <DeviceConstants.h>
#ifdef ESP8266
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32HTTPUpdateServer.h>
#endif
#include <EntityManager.h>
#include <EntityJsonRequestResponse.h>
#include <WiFi/HttpConstants.h>
#include <functional>


class WiFiServerManager {
public:
	WiFiServerManager(EntityManager* manager, SettingsStorage* conf,
			int port = 80);
	virtual ~WiFiServerManager() {
	}

	void begin();
	void loop();

	unsigned long getLastSrequestTime();
	bool isShrReceived();

	void setShrReceived(bool shrReceived = false) {
		this->shrReceived = shrReceived;
	}

protected:
	void deployDefaultUrls();
	void deployStaticFiles();

	void onRoot();
	void onDir();
	void onInfo();

	void onFileDelete();
	void onFileUpload();

	const char* getServerName();

	const char* getContentType(String& path);

	void onFileRead(const char* fileName);

	void onEntityRequest(const char* method);

	bool authenticateRequest(const char* method);

	bool isAuthenticatedRequest(char* login, char* password);

private:
#ifdef ESP8266
	ESP8266WebServer* server;
	ESP8266HTTPUpdateServer* updateServer;
#endif
#ifdef ESP32
	WebServer* server;
	ESP32HTTPUpdateServer* updateServer;
#endif
	EntityManager* manager;
	SettingsStorage* conf;

	File fsUploadFile;

	bool shrReceived = false;
	unsigned long lastSrequestTime = 0;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_WIFI_WIFISERVERMANAGER_H_ */

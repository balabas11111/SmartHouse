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

#include <FileUtils.h>
#include <WiFi/WiFiSettingsBox.h>
#include <WiFi/NetConstants.h>
#include <ESP8266WebServer.h>
#include <EntityManager.h>
#include <EntityJsonRequestResponse.h>

#include <functional>

class WiFiServerManager {
public:
	WiFiServerManager(EntityManager* manager, WiFiSettingsBox* conf,
			int port = 80);
	virtual ~WiFiServerManager() {
	}

	void begin();
	void loop();

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

private:
	ESP8266WebServer* server;
	EntityManager* manager;
	WiFiSettingsBox* conf;

	File fsUploadFile;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_WIFI_WIFISERVERMANAGER_H_ */

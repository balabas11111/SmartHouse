/*
 * WiFiManagerAsync.h
 *
 *  Created on: Feb 25, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIB_WIFIMANAGERASYNC_H_
#define LIBRARIES_ENTITYLIB_WIFIMANAGERASYNC_H_

#define ASYNC 1

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <functional>
#include <Hash.h>
#ifdef ASYNC
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif
#ifndef ASYNC
#include <ESP8266WebServer.h>
#endif
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <WiFiUtils.h>
#include <FileUtils.h>
#include <ServerSettingsBox.h>
#include <ESP_Consts.h>
#include <EntityConsts.h>
#include <EventSender.h>

class WiFiManagerAsync: public EventSender {
public:
	WiFiManagerAsync(ServerSettingsBox* conf, EntityModelDataProvider* dao, int port =80){
		this->conf=conf;
		this->oldWiFistatus = WiFi.status();
		this->dao=dao;
		this->server = new AsyncWebServer(port);
		this->events = new AsyncEventSource("/events");
#ifdef ESP8266
		//this->updater = (otaEnabled)?new ESP8266HTTPUpdateServer(true):nullptr;
#endif
	}
	virtual ~WiFiManagerAsync(){};

	bool init(bool resetToAp=false,
			std::function<void(void)> externalEvents = nullptr,
			std::function<void(void)> onServerStarted = nullptr,
			std::function<void(void)> onWiFiConnected = nullptr,
			std::function<void(void)> onWiFiDisConnected = nullptr);
	bool isConnected();

	bool isOtaEnabled();

	bool checkAuth(bool userLogin=false);

protected:
	bool disconnect();
	void waitForConnect();

	bool startAsAP();
	bool startAsSta();

	void registerApEvents();
	void registerStaEvents();

	void onStationModeDisconnected(const WiFiEventStationModeDisconnected& evt);
	void onStationModeGotIP(const WiFiEventStationModeGotIP& evt);

	void startServer();
	void startUpdater();

	void deployDefaultUrls();
	void deployStaticFiles();
	void deployTemplates();
	void deployServices();
	void deployEventSource();

	void onRoot(AsyncWebServerRequest *request);
	void onInfo(AsyncWebServerRequest *request);
	void onDir(AsyncWebServerRequest *request);
	void onCat(AsyncWebServerRequest *request);
	void onDelete(AsyncWebServerRequest *request);
	void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
	void onFileRead(AsyncWebServerRequest *request);
	void onEntityPost(AsyncWebServerRequest *request);

	virtual void sendAsEventSource(JsonObject& obj) override;

	void notFound(AsyncWebServerRequest *request);
	bool handleFileRead(String path,AsyncWebServerRequest* request);
	bool handleFileGzRead(String path,AsyncWebServerRequest* request);
	String getContentType(String filename,AsyncWebServerRequest* request);
#ifdef DEPLOY_TEMPLATES
	String processor(const String& var)	{ return dao->getByTemplateKey(var);}
#endif

	ServerSettingsBox* conf;

	EntityModelDataProvider* dao;

#ifdef ASYNC
	AsyncWebServer* server;
	AsyncEventSource* events;
	//AsyncCallbackJsonWebHandler* jsonPostHandler;
#endif

#ifdef ESP8266
	//ESP8266HTTPUpdateServer* updater;
#endif

	wl_status_t oldWiFistatus;
	bool reconnected=false;

	File fsUploadFile;

	WiFiEventHandler wiFiEventSoftAPModeProbeRequestReceivedHandler;
	WiFiEventHandler onSoftAPModeStationConnectedHandler;
	WiFiEventHandler onSoftAPModeStationDisconnectedHandler;

	WiFiEventHandler stationModeConnectedHandler;
	WiFiEventHandler onStationModeDHCPTimeoutHandler;
	WiFiEventHandler onStationModeDisconnectedHandler;
	WiFiEventHandler onStationModeGotIPHandler;

	std::function<void(void)> externalEvents;
	std::function<void(void)> onServerStarted;
	std::function<void(void)> onWiFiConnected;
	std::function<void(void)> onWiFiDisConnected;
};

#endif /* LIBRARIES_DEVICELIB_MANAGERS_WIFIMANAGERASYNC_H_ */

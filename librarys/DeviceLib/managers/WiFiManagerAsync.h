/*
 * WiFiManagerAsync.h
 *
 *  Created on: Feb 25, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_MANAGERS_WIFIMANAGERASYNC_H_
#define LIBRARIES_DEVICELIB_MANAGERS_WIFIMANAGERASYNC_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <functional>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266HTTPUpdateServer.h>

#include <utils/WiFiUtils.h>
#include <entity/settings/ServerSettingsBox.h>
#include <ESP_Consts.h>

class WiFiManagerAsync {
public:
	WiFiManagerAsync(ServerSettingsBox* conf, int port =80, bool otaEnabled = true){
		this->conf=conf;
		this->oldWiFistatus = WiFi.status();

		this->server = new AsyncWebServer(port);
#ifdef ESP8266
		this->updater = (otaEnabled)?new ESP8266HTTPUpdateServer(true):nullptr;
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

	ServerSettingsBox* conf;
	AsyncWebServer* server;

#ifdef ESP8266
	ESP8266HTTPUpdateServer* updater;
#endif

	wl_status_t oldWiFistatus;
	bool reconnected=false;

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

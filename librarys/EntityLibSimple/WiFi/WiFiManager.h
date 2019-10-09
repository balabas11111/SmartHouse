/*
 * WiFiManager.h
 *
 *  Created on: Aug 22, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_WIFI_WIFIMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_WIFI_WIFIMANAGER_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <functional>
#include <Hash.h>

#include <WiFiUtils.h>
#include <SettingsStorage.h>

#include <EntityJsonRequestResponse.h>
#include <HttpUtils.h>

class WiFiManager {
public:
	WiFiManager(SettingsStorage* conf,
			std::function<void(void)> onWiFiConnected = nullptr,
			std::function<void(void)> onWiFiDisConnected = nullptr);
	virtual ~WiFiManager() {
	}

	bool begin(bool resetToAp = false);

	bool isConnected();

	bool disconnect();

protected:
	bool startAsAP();
	bool startAsSta();

	void waitForConnect();

	void registerApEvents();
	void registerStaEvents();

	void onStationModeDisconnected(const WiFiEventStationModeDisconnected& evt);
	void onStationModeGotIP(const WiFiEventStationModeGotIP& evt);

private:
	SettingsStorage* conf;

	wl_status_t oldWiFistatus;
	bool reconnected=false;

	WiFiEventHandler wiFiEventSoftAPModeProbeRequestReceivedHandler;
	WiFiEventHandler onSoftAPModeStationConnectedHandler;
	WiFiEventHandler onSoftAPModeStationDisconnectedHandler;

	WiFiEventHandler stationModeConnectedHandler;
	WiFiEventHandler onStationModeDHCPTimeoutHandler;
	WiFiEventHandler onStationModeDisconnectedHandler;
	WiFiEventHandler onStationModeGotIPHandler;

	std::function<void(void)> onWiFiConnected;
	std::function<void(void)> onWiFiDisConnected;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_WIFI_WIFIMANAGER_H_ */

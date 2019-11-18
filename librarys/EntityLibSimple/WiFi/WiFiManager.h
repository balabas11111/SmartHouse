/*
 * WiFiManager.h
 *
 *  Created on: Aug 22, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_WIFI_WIFIMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_WIFI_WIFIMANAGER_H_

#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif
#include <functional>
#include <utils/HashUtils.h>

#include <utils/WiFiUtils.h>
#include <SettingsStorage.h>

#include <EntityJsonRequestResponse.h>
#include <utils/HttpUtils.h>

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

	void onStationModeDisconnected();
	void onStationModeGotIP(String ip);

#ifdef ESP8266
	void onStationModeDisconnected(const WiFiEventStationModeDisconnected& evt);
	void onStationModeGotIP(const WiFiEventStationModeGotIP& evt);
#endif

private:
	SettingsStorage* conf;

	wl_status_t oldWiFistatus;
	bool reconnected=false;
#ifdef ESP8266
	WiFiEventHandler wiFiEventSoftAPModeProbeRequestReceivedHandler;
	WiFiEventHandler onSoftAPModeStationConnectedHandler;
	WiFiEventHandler onSoftAPModeStationDisconnectedHandler;

	WiFiEventHandler stationModeConnectedHandler;
	WiFiEventHandler onStationModeDHCPTimeoutHandler;
	WiFiEventHandler onStationModeDisconnectedHandler;
	WiFiEventHandler onStationModeGotIPHandler;
#endif
	std::function<void(void)> onWiFiConnected;
	std::function<void(void)> onWiFiDisConnected;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_WIFI_WIFIMANAGER_H_ */

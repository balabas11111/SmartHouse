/*
 * WiFiManager.cpp
 *
 *  Created on: Aug 22, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <WiFiManager.h>

WiFiManager::WiFiManager(WiFiSettingsBox* conf,
		std::function<void(void)> onWiFiConnected,
		std::function<void(void)> onWiFiDisConnected) {
	this->conf = conf;

	this->oldWiFistatus = WiFi.status();

	this->onWiFiConnected = onWiFiConnected;
	this->onWiFiDisConnected = onWiFiDisConnected;
}

bool WiFiManager::begin(bool resetToAp) {
	bool res = false;
	Serial.println(FPSTR("WiFi begin"));

	if (resetToAp) {
		Serial.println(FPSTR("WiFi resetTo access point"));
		conf->resetToAp();
		disconnect();
	}

	if (conf->isDisconnectOnStartIfConn() && isConnected()) {
		disconnect();
	}

	if (conf->isAccessPoint()) {
		res = startAsAP();
	} else {
		res = startAsSta();
	}

	waitForConnect();
	WiFiUtils::printWiFiDetails();

	return res;
}

bool WiFiManager::isConnected() {
	return WiFi.status() == WL_CONNECTED;
}

bool WiFiManager::disconnect() {
	Serial.println(FPSTR("WiFi disconnect"));
#ifdef ESP8266
	return WiFi.disconnect(1);
#endif
#ifdef ESP32
	return WiFi.disconnect();
#endif
}

bool WiFiManager::startAsAP() {
	Serial.println(FPSTR("Start as AP"));

	WiFi.mode(WIFI_AP);

	registerApEvents();

	bool confOk = WiFi.softAPConfig(conf->IPaccessPoint(), conf->gateway(),
			conf->subnet());
	bool startOk = WiFi.softAP(conf->ssidAP(), conf->passwordAP(),
			conf->channelAP(), conf->hiddenAP(), conf->maxConnAP());

	WiFiUtils::printInitResults(confOk, startOk);

	return confOk && startOk;
}

bool WiFiManager::startAsSta() {
	WiFi.disconnect(0);
	WiFi.persistent(false);
	WiFi.mode(WIFI_STA);

	registerStaEvents();

	bool confOk = false;
	WiFi.setAutoConnect(true);
	WiFi.setAutoReconnect(true);

	/*
	 if(conf->isStaticIp()){
	 confOk = WiFi.config(conf->IPstation(), conf->gateway(), conf->subnet(),conf->dns(), conf->dns2());
	 }else{
	 confOk = WiFi.config(0U,conf->gateway(), conf->subnet(),conf->dns(), conf->dns2());
	 WiFi.enableSTA(true);
	 }
	 */
	int startOk = WiFi.begin(conf->ssid(), conf->password());

	Serial.print(FPSTR(" ssid="));
	Serial.print(conf->ssid());
	Serial.print(FPSTR(" pass="));
	Serial.print(conf->password());

	Serial.print(FPSTR(" status="));
	Serial.println(WiFi.status());

	WiFiUtils::printInitResults(confOk, startOk);

	return confOk && startOk;
}

void WiFiManager::waitForConnect() {
	if (WiFiUtils::isAp()) {
		return;
	}
	int count = 0;

	while (!isConnected()) {
		if (count == 0 || count >= 20 || oldWiFistatus != WiFi.status()) {
			WiFiUtils::printWiFiDetails();
			count = 1;
		} else {
			//Serial.print(FPSTR("."));
			delay(500);
			count++;
		}

		oldWiFistatus = WiFi.status();
	}

	Serial.println(FPSTR("WiFi is CONNECTED"));
	reconnected = false;
}

void WiFiManager::registerApEvents() {
	reconnected = false;

	wiFiEventSoftAPModeProbeRequestReceivedHandler =
			WiFi.onSoftAPModeProbeRequestReceived(
					[this](const WiFiEventSoftAPModeProbeRequestReceived& evt) {WiFiUtils::printSoftAPModeProbeRequestReceived(evt);});
	onSoftAPModeStationConnectedHandler =
			WiFi.onSoftAPModeStationConnected(
					[this](const WiFiEventSoftAPModeStationConnected& evt) {WiFiUtils::printSoftAPModeStationConnected(evt);});
	onSoftAPModeStationDisconnectedHandler =
			WiFi.onSoftAPModeStationDisconnected(
					[this](const WiFiEventSoftAPModeStationDisconnected& evt) {WiFiUtils::printSoftAPModeStationDisconnected(evt);});

	Serial.println(FPSTR("WiFi registered Ap events"));
}

void WiFiManager::registerStaEvents() {
	reconnected = false;

	stationModeConnectedHandler =
			WiFi.onStationModeConnected(
					[this](const WiFiEventStationModeConnected& evt) {WiFiUtils::printStationModeConnected(evt);});
	onStationModeDHCPTimeoutHandler = WiFi.onStationModeDHCPTimeout(
			[this]() {WiFiUtils::printStationModeDHCPTimeout();});

	onStationModeDisconnectedHandler =
			WiFi.onStationModeDisconnected(
					[this](const WiFiEventStationModeDisconnected& evt) {onStationModeDisconnected(evt);});
	onStationModeGotIPHandler =
			WiFi.onStationModeGotIP(
					[this](const WiFiEventStationModeGotIP& evt) {onStationModeGotIP(evt);});

	Serial.println(FPSTR("WiFi register Sta events"));
}

void WiFiManager::onStationModeDisconnected(
		const WiFiEventStationModeDisconnected& evt) {
	WiFiUtils::printStationModeDisconnected(evt);
	if (onWiFiDisConnected != nullptr) {
		onWiFiDisConnected();
	}
}

void WiFiManager::onStationModeGotIP(const WiFiEventStationModeGotIP& evt) {
	reconnected = true;
	WiFiUtils::printStationModeGotIP(evt);
	if (onWiFiConnected != nullptr) {
		onWiFiConnected();
	}
}

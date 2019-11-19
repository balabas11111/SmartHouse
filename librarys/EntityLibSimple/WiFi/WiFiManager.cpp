/*
 * WiFiManager.cpp
 *
 *  Created on: Aug 22, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <WiFi/WiFiManager.h>

#include <SettingsStorage.h>

WiFiManager::WiFiManager(SettingsStorage* conf,
		std::function<void(void)> onWiFiConnected,
		std::function<void(void)> onWiFiDisConnected) {
	this->conf = conf;

	this->oldWiFistatus = WiFi.status();

	this->onWiFiConnected = onWiFiConnected;
	this->onWiFiDisConnected = onWiFiDisConnected;
}

bool WiFiManager::begin(bool resetToAp) {
	conf->setDeviceStatus((char*)DEVICE_STATUS_CONNECTING);

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
#ifdef ESP8266
	wiFiEventSoftAPModeProbeRequestReceivedHandler =
			WiFi.onSoftAPModeProbeRequestReceived(
					[this](const WiFiEventSoftAPModeProbeRequestReceived& evt) {WiFiUtils::printSoftAPModeProbeRequestReceived(evt);});
	onSoftAPModeStationConnectedHandler =
			WiFi.onSoftAPModeStationConnected(
					[this](const WiFiEventSoftAPModeStationConnected& evt) {WiFiUtils::printSoftAPModeStationConnected(evt);});
	onSoftAPModeStationDisconnectedHandler =
			WiFi.onSoftAPModeStationDisconnected(
					[this](const WiFiEventSoftAPModeStationDisconnected& evt) {WiFiUtils::printSoftAPModeStationDisconnected(evt);});
#endif
	Serial.println(FPSTR("WiFi registered Ap events"));
}

void WiFiManager::registerStaEvents() {
	reconnected = false;

#ifdef ESP8266
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
#endif
#ifdef ESP32
	WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
			Serial.print("WiFi connected. sssid: ");

			system_event_sta_connected_t eventinfo = info.connected;

			for(int i=0; i < eventinfo.ssid_len; i++){
				Serial.print(eventinfo.ssid[i]);
			}

			Serial.println();
		}, WiFiEvent_t::SYSTEM_EVENT_STA_CONNECTED);

	WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
			Serial.print("WiFi lost ip");

		}, WiFiEvent_t::SYSTEM_EVENT_STA_LOST_IP);

	WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info){
	        Serial.print("WiFi lost connection. Reason: ");
	        Serial.println(info.disconnected.reason);

	        onStationModeDisconnected();
	    }, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);

	WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info){
		String ip = String(info.got_ip.ip_info.ip.addr);
		Serial.print("WiFi got ip ");
		Serial.println(ip);

		onStationModeGotIP(ip);
	}, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
#endif
	Serial.println(FPSTR("WiFi register Sta events"));
}

#ifdef ESP8266
void WiFiManager::onStationModeDisconnected(
		const WiFiEventStationModeDisconnected& evt) {
	WiFiUtils::printStationModeDisconnected(evt);
	onStationModeDisconnected();
}

void WiFiManager::onStationModeGotIP(const WiFiEventStationModeGotIP& evt) {
	WiFiUtils::printStationModeGotIP(evt);
	onStationModeGotIP(evt.ip.toString());
}
#endif

void WiFiManager::onStationModeDisconnected() {
	if (onWiFiDisConnected != nullptr) {
		onWiFiDisConnected();
	}
	//conf->setCurrentIp(EMPTY_STR);
	conf->setDeviceStatus((char*)DEVICE_STATUS_DISCONNECTED);
}
void WiFiManager::onStationModeGotIP(String ip) {
	reconnected = true;

	conf->setCurrentIp(strdup(ip.c_str()));

	if (onWiFiConnected != nullptr) {
		onWiFiConnected();
	}

	conf->setDeviceStatus((char*)DEVICE_STATUS_CONNECTED);
}

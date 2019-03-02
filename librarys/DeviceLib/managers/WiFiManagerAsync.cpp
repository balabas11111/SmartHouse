/*
 * WiFiManagerAsync.cpp
 *
 *  Created on: Feb 25, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <managers/WiFiManagerAsync.h>

bool WiFiManagerAsync::init(bool resetToAp,
		std::function<void(void)> externalEvents,
		std::function<void(void)> onServerStarted,
		std::function<void(void)> onWiFiConnected,
		std::function<void(void)> onWiFiDisConnected) {

	this->externalEvents = externalEvents;
	this->onServerStarted = onServerStarted;
	this->onWiFiConnected = onWiFiConnected;
	this->onWiFiDisConnected = onWiFiDisConnected;

	bool res = false;
	Serial.println(FPSTR("WiFi begin"));

	if(resetToAp){
		Serial.println(FPSTR("WiFi resetTo access point"));
		conf->resetToAp();
		disconnect();
	}

	if(conf->isDisconnectOnStartIfConn() && isConnected()){
		disconnect();
	}

	if(conf->isAccessPoint()){
		res = startAsAP();
	}else{
		res = startAsSta();
	}

	waitForConnect();
	WiFiUtils::printWiFiDetails();

	startServer();
	startUpdater();

	if(onServerStarted!=nullptr){onServerStarted();}

	return res;
}

bool WiFiManagerAsync::isConnected() {
	return WiFi.status() == WL_CONNECTED;
}

bool WiFiManagerAsync::disconnect() {
	Serial.println(FPSTR("WiFi disconnect"));
	#ifdef ESP8266
		return WiFi.disconnect(1);
	#endif
	#ifdef ESP32
		return WiFi.disconnect();
	#endif
}

bool WiFiManagerAsync::startAsAP() {
	Serial.println(FPSTR("Start as AP"));

	WiFi.mode(WIFI_AP);

	registerApEvents();

	bool confOk= WiFi.softAPConfig(conf->IPaccessPoint(), conf->gateway(), conf->subnet());
	bool startOk = WiFi.softAP(conf->ssidAP(), conf->passwordAP(), conf->channelAP(), conf->hiddenAP(), conf->maxConnAP());

	WiFiUtils::printInitResults(confOk, startOk);

	return confOk && startOk;
}

bool WiFiManagerAsync::isOtaEnabled() {
#ifndef ESP8266
	return false;
#endif
#ifdef ESP8266
	return updater!=nullptr;
#endif
}

bool WiFiManagerAsync::startAsSta() {
	WiFi.disconnect(0);
	WiFi.persistent(false);
	WiFi.mode(WIFI_STA);

	registerStaEvents();

	bool confOk=false;

	if(conf->isStaticIp()){
		confOk = WiFi.config(conf->IPstation(), conf->gateway(), conf->subnet(),conf->dns(), conf->dns2());
	}else{
		confOk = WiFi.config(0U,0U,0U,conf->dns(), conf->dns2());
	}

	bool startOk = WiFi.begin (conf->ssid(),conf->password());

	WiFiUtils::printInitResults(confOk, startOk);

	return confOk && startOk;
}

void WiFiManagerAsync::registerApEvents() {
	reconnected=false;

	wiFiEventSoftAPModeProbeRequestReceivedHandler=WiFi.onSoftAPModeProbeRequestReceived([this](const WiFiEventSoftAPModeProbeRequestReceived& evt){WiFiUtils::printSoftAPModeProbeRequestReceived(evt);});
	onSoftAPModeStationConnectedHandler=WiFi.onSoftAPModeStationConnected([this](const WiFiEventSoftAPModeStationConnected& evt){WiFiUtils::printSoftAPModeStationConnected(evt);});
	onSoftAPModeStationDisconnectedHandler=WiFi.onSoftAPModeStationDisconnected([this](const WiFiEventSoftAPModeStationDisconnected& evt){WiFiUtils::printSoftAPModeStationDisconnected(evt);});

	if(externalEvents!=nullptr){
		externalEvents();
	}
	Serial.println(FPSTR("WiFi registered Ap events"));
}

void WiFiManagerAsync::registerStaEvents() {
	reconnected=false;

	stationModeConnectedHandler=WiFi.onStationModeConnected([this](const WiFiEventStationModeConnected& evt){WiFiUtils::printStationModeConnected(evt);});
	onStationModeDHCPTimeoutHandler=WiFi.onStationModeDHCPTimeout([this](){WiFiUtils::printStationModeDHCPTimeout();});

	onStationModeDisconnectedHandler=WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected& evt){onStationModeDisconnected(evt);});
	onStationModeGotIPHandler=WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP& evt){onStationModeGotIP(evt);});

	if(externalEvents!=nullptr){
		externalEvents();
	}
	Serial.println(FPSTR("WiFi register Sta events"));
}

void WiFiManagerAsync::onStationModeDisconnected(const WiFiEventStationModeDisconnected& evt) {
	WiFiUtils::printStationModeDisconnected(evt);
	if(onWiFiDisConnected!=nullptr){onWiFiDisConnected();}
}

void WiFiManagerAsync::waitForConnect() {
	if(WiFiUtils::isAp()){
		return;
	}
	uint8_t count=0;

	while(!isConnected() ){
		if(count==0 || count>=1000 || oldWiFistatus!=WiFi.status()){
			WiFiUtils::printWiFiDetails();
			count=1;
		}else{
			//Serial.print(FPSTR("."));
			//delay(RE_CONNECT_DELAY);
			count++;
		}

		oldWiFistatus=WiFi.status();
	}

	Serial.println(FPSTR("WiFi is CONNECTED"));
	reconnected=false;
}

void WiFiManagerAsync::onStationModeGotIP(const WiFiEventStationModeGotIP& evt) {
	reconnected=true;
	WiFiUtils::printStationModeGotIP(evt);
	if(onWiFiConnected!=nullptr){onWiFiConnected();}
}

void WiFiManagerAsync::startUpdater() {
#ifdef ESP8266
	Serial.println(FPSTR("Init OTA update"));
	if(updater!=nullptr){
		updater->setup(this->server,conf->adminLogin(),conf->adminPassword());
	}else{
		Serial.println(FPSTR("OTA Update is disabled on this device"));
	}
#endif
}

void WiFiManagerAsync::startServer() {
	deployDefaultUrls();
	deployStaticFiles();
	deployTemplates();
	deployServices();
}

/*
 * ButtonB.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_WIFIHELPER_H_
#define LIBRARIES_TIMETRIGGER_WIFIHELPER_H_

#include "Arduino.h"
#include "EspSettingsBox.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

class WiFiHelper{

public:
	WiFiHelper(EspSettingsBox *_settingsBox,std::function<void(void)> _clearDisplayFunction,std::function<void(String,int,int)> _displayLineFunction,std::function<void(void)> _ledOnFunction,std::function<void(void)> _ledOffFunction,std::function<void(ESP8266WebServer)> _setupsServerUrlsyFunction);
	void setup();
	void connect();
	boolean isConnected();
	void connectIfNotConnected();

	void loop();

	WiFiClient getClient();
	Client& getClientAs();

	ESP8266WebServer getServer();
	ESP8266WiFiClass getWiFi();

private:
	EspSettingsBox *espSettingsBox;
	ESP8266WebServer server;
	WiFiClient client;


	void setupAsAccessPoint();
	void setupAsClient();

	std::function<void(void)> clearDisplayFunction;
	std::function<void(String,int,int)> displayLineFunction;

	std::function<void(ESP8266WebServer)> setupsServerUrlsFunction;

	std::function<void(void)> ledOnFunction;
	std::function<void(void)> ledOffFunction;

	void clearDisplay();
	void displayLine(String message,uint8_t row,uint8_t column);
	void ledOn();
	void ledOff();
	void setupServersUrls();
};


#endif /* LIBRARIES_TIMETRIGGER_WIFIHELPER_H_ */

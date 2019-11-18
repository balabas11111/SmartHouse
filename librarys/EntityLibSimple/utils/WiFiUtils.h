/*
 * WiFiUtils.h
 *
 *  Created on: Feb 27, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_UTILS_WIFIUTILS_H_
#define LIBRARIES_ENTITYLIBSIMPLE_UTILS_WIFIUTILS_H_

#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

class WiFiUtils {
public:
	WiFiUtils(){};
	virtual ~WiFiUtils(){};

	static const char* macToString(const unsigned char* mac);

	static void printWiFiDetails();

	static IPAddress getIp();

	static bool isAp();

	static bool isConnected();

	static void printInitResults(int confOk,int startOk);
#ifdef ESP8266
	static void printSoftAPModeProbeRequestReceived(const WiFiEventSoftAPModeProbeRequestReceived& evt);
	static void printSoftAPModeStationConnected(const WiFiEventSoftAPModeStationConnected& evt);
	static void printSoftAPModeStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& evt);

	static void printStationModeConnected(const WiFiEventStationModeConnected& evt);
	static void printStationModeDisconnected(const WiFiEventStationModeDisconnected& evt);
	static void printStationModeDHCPTimeout();
	static void printStationModeGotIP(const WiFiEventStationModeGotIP& evt);
#endif

};

#endif /* LIBRARIES_DEVICELIB_UTILS_WIFIUTILS_H_ */

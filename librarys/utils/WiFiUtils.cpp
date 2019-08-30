/*
 * WiFiUtils.cpp
 *
 *  Created on: Feb 27, 2019
 *      Author: Vitaliy_Vlasov
 */

#include "WiFiUtils.h"

const char* WiFiUtils::macToString(const unsigned char* mac) {
	  char buf[20];
	  snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
			   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	  return buf;
}

void WiFiUtils::printWiFiDetails() {
	Serial.println();
	Serial.println(FPSTR("WiFi details--------------------------"));
	Serial.print(FPSTR(" SSSID=")); 	  Serial.print (WiFi.SSID());
	Serial.print(FPSTR(" GATEWAY=")); 	  Serial.print (WiFi.gatewayIP());
	Serial.print(FPSTR(" SUBNET=")); 	  Serial.print (WiFi.subnetMask());
	Serial.print(FPSTR(" MAC=")); 	  Serial.print (WiFi.macAddress());
	Serial.print(FPSTR(" IP=")); 	  Serial.print (getIp());
	Serial.print(FPSTR(" STATUS="));  Serial.println(WiFi.status());
	Serial.println(FPSTR("----------------------------------"));
}

IPAddress WiFiUtils::getIp() {
	if(isAp()){
		return WiFi.softAPIP();
	}
	return WiFi.localIP();
}

bool WiFiUtils::isAp() {
	return WiFi.getMode() == WIFI_AP;
}

void WiFiUtils::printInitResults(int confOk, int startOk) {
	if(isAp()){
		Serial.print(FPSTR("init AP ("));
	}else{
		Serial.print(FPSTR("init STA ("));
	}
		Serial.print(FPSTR(" confOk="));
		Serial.print(confOk);
		Serial.print(FPSTR(" startOk="));
		Serial.print(startOk);
		Serial.println(FPSTR(")"));
}

void WiFiUtils::printSoftAPModeProbeRequestReceived(const WiFiEventSoftAPModeProbeRequestReceived& evt) {
  Serial.print(FPSTR("AP WiFiEventSoftAPModeProbeRequestReceived: MAC="));
  Serial.print(WiFiUtils::macToString(evt.mac));
  Serial.print(FPSTR(" rssi="));
  Serial.println(evt.rssi);
}

void WiFiUtils::printSoftAPModeStationConnected(const WiFiEventSoftAPModeStationConnected& evt) {
  Serial.print(FPSTR("AP connected: MAC="));
  Serial.println(WiFiUtils::macToString(evt.mac));

}

void WiFiUtils::printSoftAPModeStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& evt) {
  Serial.print(FPSTR("AP disconnected: "));
  Serial.println(WiFiUtils::macToString(evt.mac));
}

void WiFiUtils::printStationModeConnected(const WiFiEventStationModeConnected& evt){
	Serial.print(FPSTR("STA connected: "));
	Serial.print(FPSTR(" ssid="));
	Serial.print(evt.ssid);
	Serial.print(FPSTR(" bssid="));
	Serial.print(WiFiUtils::macToString(evt.bssid));
	Serial.print(FPSTR(" channel="));
	Serial.println(evt.channel);
}

void WiFiUtils::printStationModeDisconnected(const WiFiEventStationModeDisconnected& evt){
	Serial.print(FPSTR("STA DISconnected: "));
	Serial.print(FPSTR(" ssid="));
	Serial.print(evt.ssid);
	Serial.print(FPSTR(" bssid="));
	Serial.print(WiFiUtils::macToString(evt.bssid));
	Serial.print(FPSTR(" reason="));
	Serial.println(evt.reason);
}

void WiFiUtils::printStationModeDHCPTimeout(){
	Serial.print(FPSTR("STA DHCP timed out: "));
}

void WiFiUtils::printStationModeGotIP(const WiFiEventStationModeGotIP& evt){
	Serial.print(FPSTR("onStationModeGotIP IP="));
	Serial.println(evt.ip);
}

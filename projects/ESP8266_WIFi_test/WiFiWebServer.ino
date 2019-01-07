/*
    This sketch shows how to use WiFi event handlers.

    In this example, ESP8266 works in AP mode.
    Three event handlers are demonstrated:
    - station connects to the ESP8266 AP
    - station disconnects from the ESP8266 AP
    - ESP8266 AP receives a probe request from a station

    Written by Markus Sattler, 2015-12-29.
    Updated for new event handlers by Ivan Grokhotkov, 2017-02-02.
    This example is released into public domain,
    or, at your option, CC0 licensed.
*/

#define DEBUG_ESP_CORE 1

#include <ESP8266WiFi.h>
#include "ESP8266WebServer.h"
#include <stdio.h>

const char* ssid     = "balabasKiev5";
const char* password = "wuWylKegayg2wu22";

WiFiEventHandler stationConnectedHandler;
WiFiEventHandler stationDisconnectedHandler;
WiFiEventHandler probeRequestPrintHandler;
WiFiEventHandler probeRequestBlinkHandler;

WiFiEventHandler stationModeConnectedHandler;
WiFiEventHandler onStationModeDHCPTimeoutHandler;
WiFiEventHandler onStationModeDisconnectedHandler;
WiFiEventHandler onStationModeGotIPHandler;

wl_status_t oldSTatus;
ulong lastConnected;

ESP8266WebServer server ( 80 );

bool blinkFlag;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Don't save WiFi configuration in flash - optional

  WiFi.persistent(false);
  WiFi.disconnect(0);
  WiFi.mode(WIFI_STA);


  WiFi.begin(ssid, password);
  // Set up an access point
  /*
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);


  stationConnectedHandler = WiFi.onSoftAPModeStationConnected(onStationConnected);
  stationDisconnectedHandler = WiFi.onSoftAPModeStationDisconnected(onStationDisconnected);
  probeRequestPrintHandler = WiFi.onSoftAPModeProbeRequestReceived(onProbeRequestPrint);
  probeRequestBlinkHandler = WiFi.onSoftAPModeProbeRequestReceived(onProbeRequestBlink);
  */
  oldSTatus=WiFi.status();
  lastConnected=millis();

  stationModeConnectedHandler=WiFi.onStationModeConnected(onStationModeConnected);
  onStationModeDHCPTimeoutHandler=WiFi.onStationModeDHCPTimeout(onStationModeDHCPTimeout);
  onStationModeDisconnectedHandler=WiFi.onStationModeDisconnected(onStationModeDisconnected);
  onStationModeGotIPHandler=WiFi.onStationModeGotIP(onStationModeGotIP);

  server.on("/test", onTestHandler);
  server.begin();

  Serial.println("Events attached");
}

void onStationModeConnected(const WiFiEventStationModeConnected& evt){
	 Serial.print("onStationModeConnected ssid");
	 Serial.print(evt.ssid);
	 Serial.print(" channel=");
	 Serial.println(evt.channel);
	 Serial.print(" bssid=");
	 for(uint8_t i=0;i<6;i++){
		 Serial.print(evt.bssid[i]);
	 }
	 Serial.println();
}

void onStationModeDHCPTimeout(){
	Serial.println("onStationModeDHCPTimeout");
}

void onStationModeDisconnected(const WiFiEventStationModeDisconnected& evt){
	Serial.println("onStationModeDisconnected");
	Serial.println(evt.ssid);
	Serial.println(evt.reason);
}

void onStationModeGotIP(const WiFiEventStationModeGotIP& evt){
	Serial.print("onStationModeGotIP =");
	Serial.println(evt.ip);
}
//-----------------------------------------------------------------------------------
void onTestHandler(){
	Serial.println("On Test handler");
	server.send(200, "Text/html", "Test passed");
}

//----------------------------------------------------------------------
void onStationConnected(const WiFiEventSoftAPModeStationConnected& evt) {
  Serial.print("Station connected: ");
  Serial.println(macToString(evt.mac));
}

void onStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& evt) {
  Serial.print("Station disconnected: ");
  Serial.println(macToString(evt.mac));
}

void onProbeRequestPrint(const WiFiEventSoftAPModeProbeRequestReceived& evt) {
  Serial.print("Probe request from: ");
  Serial.print(macToString(evt.mac));
  Serial.print(" RSSI: ");
  Serial.println(evt.rssi);
}

void onProbeRequestBlink(const WiFiEventSoftAPModeProbeRequestReceived&) {
  // We can't use "delay" or other blocking functions in the event handler.
  // Therefore we set a flag here and then check it inside "loop" function.
  blinkFlag = true;
}

void loop() {
	/*
  if (millis() > 10000 && probeRequestPrintHandler) {
    Serial.println("Not printing probe requests any more (LED should still blink)");
    probeRequestPrintHandler = WiFiEventHandler();
  }
  */


	wl_status_t newStatus=WiFi.status();

	while (newStatus!=WL_CONNECTED){
		newStatus=WiFi.status();

		if(newStatus!=oldSTatus){
			Serial.print("oldSTatus=");
			Serial.print(oldSTatus);
			Serial.print("newStatus=");
			Serial.print(newStatus);
		}

		if(newStatus!=WL_CONNECTED){
			Serial.print(".");
			delay(1000);
		}

		oldSTatus=WiFi.status();
	}

	if(newStatus==WL_CONNECTED){
		if(lastConnected+10000<=millis()){
			Serial.print("connected ");
			Serial.println(millis());

			lastConnected=millis();
		}else{
		}
	}else{
		Serial.println("Not connected");
	}

	server.handleClient();

  if (blinkFlag) {
    blinkFlag = false;
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  delay(10);
}

String macToString(const unsigned char* mac) {
  char buf[20];
  snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

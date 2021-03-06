#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <Wire.h>
#include "OLED.h"

#include <OneWire.h>
#include "DallasTemperature.h"
#include "BeeperB.h"
#include "ButtonB.h"
#include "LedB.h"
#include "TimeTrigger.h"
#include <RtcDS3231.h>
#include "SensorsDS18_helper.h"

const char *ssid = "balabasKiev5";
const char *password = "wuWylKegayg2wu22";

const char *DeviceId = "ESP 8266 temp collector1";

const uint8_t okButtonPin=5;
boolean okButtonPressed=false;

LedB greenLed(10);
LedB redLed(9);
//BeeperB beeper(5);

TimeTrigger timeTrigger(1000,4000);

ESP8266WebServer server ( 80 );
OLED display(4, 0);

RtcDS3231<TwoWire> rtc(Wire);

const int sensorBusCount=2;

String sensorPinDescr[sensorBusCount]={"Pin1","Pin2"};
OneWire oneWires[sensorBusCount]={OneWire(14),OneWire(12)};
DallasTemperature dallasTemperatures[sensorBusCount];
SensorsDS18_helper sensorHelper;

void setup ( void ) {
	Serial.begin ( 115200 );

	setupButton();
	greenLed.init();
	redLed.init();
	//beeper.init();

	rtc.Begin();

	setupWiFi();
	initSensors();
	setupDisplay();

	//beeper.shortBeep();

	greenLed.turnOn();
}

void loop ( void ) {

	if ( okButtonPressed ) {
	  Serial.println("Ok button pressed");

	  //okButtonPressed=false;
	}

	server.handleClient();
}

void setupButton(){
	pinMode(okButtonPin, INPUT);
	attachInterrupt(okButtonPin, onOkButtonPressed, RISING);
	attachInterrupt(okButtonPin, onOkButtonReleased, FALLING);
}

void onOkButtonReleased(){
	Serial.println("telegramButtonReleased------");
	  int button = digitalRead(okButtonPin);
	  if(button == LOW)
	  {
		  okButtonPressed = false;
	  }
	  return;
}

void onOkButtonPressed(){
	Serial.println("telegramButtonPressed++++++");
	  int button = digitalRead(okButtonPin);
	  if(button == HIGH)
	  {
		  okButtonPressed = true;
	  }
	  return;
}

String sensorsToHtml(){
	String html="<table>";

	for(unsigned int i=0;i<sensorBusCount;i++){
		html+=sensorHelper.sensorToHtml(&dallasTemperatures[i]);
	}
	html+="</table>";

	return html;
}

void refreshSensors(){

	for (int i=0;i<sensorBusCount;i++){
		dallasTemperatures[i].requestTemperatures();
	}

	Serial.println("Sensors requested and refreshed");
}

int initSensors(){
	int totalSensors=0;

	for(int i=0;i<sensorBusCount;i++){
		dallasTemperatures[i]=DallasTemperature(&oneWires[i]);
		sensorHelper.initSensor(&dallasTemperatures[i], sensorPinDescr[i]);
		totalSensors+=dallasTemperatures[i].getDeviceCount();
	}

	Serial.println("Sensors initialized");

	return totalSensors;
}

int getSensorscount(){
	int totalSensors=0;

	for(int i=0;i<sensorBusCount;i++){
		totalSensors+=dallasTemperatures[i].getDeviceCount();
	}

	return totalSensors;
}


void setupDisplay(){

	Serial.println("OLED test!");

	  display.begin();

	  display.print("Hello World");
	  delay(3*1000);

	  display.print("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
	  delay(3*1000);

	  display.clear();
	  delay(3*1000);

	  display.print("TOP-LEFT");
	  display.print("4th row", 4);
	  display.print("RIGHT-BOTTOM", 7, 4);
	  delay(3*1000);

	  display.off();
	  display.print("3rd row", 3, 8);
	  delay(3*1000);

	  display.on();
	  delay(3*1000);
}

void setupWiFi(){
	WiFi.begin ( ssid, password );
		Serial.println ( "" );

		// Wait for connection
		while ( WiFi.status() != WL_CONNECTED ) {
			delay ( 500 );
			Serial.print ( "." );
		}

		Serial.println ( "" );
		Serial.print ( "Connected to " );
		Serial.println ( ssid );
		Serial.print ( "IP address: " );
		Serial.println ( WiFi.localIP() );

		if ( MDNS.begin ( "esp8266" ) ) {
			Serial.println ( "MDNS responder started" );
		}

		server.on ( "/", handleRoot );
		server.on ( "/getJson", handleJson );
		server.on ( "/test.svg", drawGraph );
		server.on ( "/inline", []() {
			server.send ( 200, "text/plain", "this works as well" );
		} );
		server.onNotFound ( handleNotFound );
		server.begin();
		Serial.println ( "HTTP server started" );
}

String getDateTimeStr(){

	return getDateStr()+" "+getTimeStr();
}

String getDateStr(){
	char str[15];

	RtcDateTime dt= rtc.GetDateTime();
	sprintf(str, "%d/%d/%d",     //%d allows to print an integer to the string
		 dt.Year(),   //get year method
		 dt.Month(),  //get month method
		 dt.Day()  //get second method
		 );

	Serial.println(str);

	return String(str);
}

String getTimeStr(){
	char str[15];

	RtcDateTime dt= rtc.GetDateTime();
	sprintf(str, "%02d:%02d:%02d",     //%d allows to print an integer to the string
		 dt.Hour(),   //get hour method
		 dt.Minute(), //get minute method
		 dt.Second()  //get second method
		 );

	Serial.println(str);

	return String(str);
}



void handleRoot() {
	redLed.changeState();

	refreshSensors();

	char temp[400];

	String html="<html>\
			  <head>\
			    <meta http-equiv='refresh' content='5'/>\
			    <title>ESP8266 Demo</title>\
			    <style>\
			      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
			    </style>\
			  </head>\
			  <body>\
			    <h1>Hello from ESP8266!</h1>\
			    <p>Uptime: %02d:%02d:%02d</p>\
			    <img src=\"/test.svg\" />";

	html+="<p>";
	html+=sensorsToHtml();
	html+="</p>";
	html+="<p>";
	html+="Last request time:    ";
	html+=getTimeStr();
	html+="</p>";
	html+="</body>\
</html>";

/*	snprintf ( temp, 400,

"<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

		hr, min % 60, sec % 60
	);
	*/


	server.send ( 200, "text/html", html );
	redLed.changeState();
}

void handleJson(){
	refreshSensors();

	String result="{\"device\":\"";
	result+=String(DeviceId);
	result+="\",\"date\":\"";
	result+=getDateStr();
	result+="\",\"time\": \"";
	result+=getTimeStr();
	result+="\",\"count\":";
	result+=String(getSensorscount());
	result+=",\"sensors\": [";

	for(unsigned int i=0;i<sensorBusCount;i++){
		//result+=sensorHelper.sensorToJson(&dallasTemperatures[i],getDateStr(),getTimeStr(),DeviceId);
		int length=dallasTemperatures[i].getDeviceCount();

		for(int j=0;j<length;j++){
			result+="{\"id\":";
			result+=String(j);
			result+=",\"value\":";
			result+=dallasTemperatures[i].getTempCByIndex(j);
			result+="}";
			if(j!=length-1){
				result+=",";
			}
		}

		if(i!=sensorBusCount-1){
			result+=",";
		}
	}

	result+="]}";

	server.send ( 200, "text/html", result );
}

void handleNotFound() {
	redLed.changeState();
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}

	server.send ( 404, "text/plain", message );
	redLed.changeState();
}

void drawGraph() {
	String out = "";
	char temp[100];
	out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
 	out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
 	out += "<g stroke=\"black\">\n";
 	int y = rand() % 130;
 	for (int x = 10; x < 390; x+= 10) {
 		int y2 = rand() % 130;
 		sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
 		out += temp;
 		y = y2;
 	}
	out += "</g>\n</svg>\n";

	server.send ( 200, "image/svg+xml", out);

}

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

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
#include "DHT.h"
#include "FS.h"
#include "KeyValue.h"
#include "EspSettingsBox.h"

#define FIRMVARE_VERSION "b@l@bas-soft SENSORZ v0.0.5"
//Robot Dyn//vem  // 12E
#define D0_PIN 16 //GPIO016 ////beeper
#define D1_PIN 5  //GPIO05  //DallasTemp
#define D2_PIN 4  //GPIO04  //OLED //SDA //blue
#define D3_PIN 0  //GPIO00  //OLED //SCL //green
#define D4_PIN 2  //GPIO02  //RedLed
#define D5_PIN 14 //GPIO14  //DHT
#define D6_PIN 12 //GPIO12  //DallasTemp red led2
#define D7_PIN 13 //GPIO13  //GreenLed
#define D8_PIN 15 //GPIO15  //button
#define RX_PIN 3
#define TX_PIN 1

#define SD3_PIN 10
#define SD2_PIN 9

#define BUZER_PLAY_FLAG 1
#define BUZER_NO_PLAY_FLAG 0

#define DHTTYPE DHT22   // DHT 21 (AM2301)
DHT dht(D5_PIN, DHTTYPE);
float humidity;
float temperature;

boolean dhtActive=false;
String dhtName="pin_D5";
String dhtHumidityAddress="dht_hum_addr";
String dhtTemperatureAddress="dht_temp_addr";

const uint8_t BUTTON_PIN=D8_PIN;

LedB greenLed(D7_PIN);
//LedB redLed(D6_PIN);
//BeeperB beeper(DO_PIN,BUZER_NO_PLAY_FLAG,BUZER_PLAY_FLAG);

ESP8266WebServer server ( 80 );

boolean displayOn=false;
OLED display(D2_PIN, D3_PIN);

uint8_t curPage=0;

RtcDS3231<TwoWire> rtc(Wire);

const int sensorBusCount=1;

//String sensorPinDescr[sensorBusCount]={"Pin_D6","Pin_D1"};
//OneWire oneWires[sensorBusCount]={OneWire(D6_PIN),OneWire(D1_PIN)};
String sensorPinDescr[sensorBusCount]={"Pin_D1"};
OneWire oneWires[sensorBusCount]={OneWire(D1_PIN)};
DallasTemperature dallasTemperatures[sensorBusCount];
SensorsDS18_helper sensorHelper;

uint8_t totalDetectedSensors=0;
uint8_t totalActivePages=0;

//boolean activePages[sensorBusCount+2]={true,false,false,false};
boolean activePages[sensorBusCount+2]={true,false,false};
boolean haveActivePages=false;

boolean pageButtonHandleInLoop=false;
boolean sendToThingSpeakTriggered=false;

boolean alertMode=false;
uint8_t alertBeepCount=0;

TimeTrigger sensorTrigger(-5000,10000,true);
TimeTrigger displayTrigger(0,20000,true);
TimeTrigger alertTrigger(0,1000,false);
TimeTrigger alertTriggerDelay(0,30000,false);

TimeTrigger displayPageChangeTrigger(0,0,false);
TimeTrigger thingSpeakTrigger(0,0,false);
TimeTrigger restartTrigger(1000,4000,false);

KeyValueContainer sensorsKVContainer("/sensors.txt");

EspSettingsBox espSettingsBox("/values.txt");

void setup ( void ) {
	Serial.begin ( 115200 );

	//beeper.init();

	initDisplay();
	initFS();

	initSettings();
	refreshTimeTriggers();

	greenLed.init();
	//redLed.init();
	rtc.Begin();
	scanI2C();

	//setupButtonInterrupts();

	setupWiFi();
	initSensors(false);

	refreshSensors(true);
	refreshDisplay();

	displayTrigger.saveTime();

	greenLed.turnOn();

	//beeper.play(2500,250);
}

void loop ( void ) {
	connectToWiFiIfNotConnected();
	handleAlertNotifSound();
	refreshSensors(false);
	handlePageButton();
	handleDisplayPover();

	sendDataToThingSpeak();
	checkRestartHandled();
	checkDisplayPageAutochange();

	server.handleClient();
}

void checkRestartHandled(){
	if(restartTrigger.checkTrigger()){
		Serial.print("Restart started at ");
		Serial.println(millis());
		ESP.reset();
	}
}

void initSettings(){
	delay(500);
	if(onPageButtonChanged())
	{
		Serial.println("---STart access point -----2");
		espSettingsBox.isAccesPoint=true;
		espSettingsBox.saveSettingsJson();
		clearDisplay();
		displayLine("Default Load",3,0);
		delay(500);
		Serial.println("Station vill vork as station");

		Serial.println("access point conf done");
	}else{
		//espSettingsBox.loadSettingsFromFile();
		clearDisplay();
		displayLine("Loading device",3,0);
	}

	Serial.println("-----Stat load variables from file");

	espSettingsBox.loadSettingsJson();

	Serial.println("------Variables load complete");

	delay(500);
}

void refreshTimeTriggers(){
	sensorTrigger=TimeTrigger(millis(),espSettingsBox.refreshInterval*1000,true);
	thingSpeakTrigger=TimeTrigger(millis(),espSettingsBox.postDataToTSInterval*1000,(espSettingsBox.postDataToTSInterval>0));
	displayPageChangeTrigger=TimeTrigger(millis(),espSettingsBox.displayAutochange*1000,(espSettingsBox.displayAlvaysOn && espSettingsBox.displayAutochange>0));
	if(espSettingsBox.displayAlvaysOn && !displayOn){
		turnOnDisplay();
		displayTrigger.saveTime();
		displayTrigger.setActive(false);
	}else{
		if(!espSettingsBox.displayAlvaysOn && displayOn){
			displayTrigger.saveTime();
			displayTrigger.setActive(true);
		}
	}
}


void setupButtons(){
	pinMode(BUTTON_PIN, INPUT);
}

void setupButtonInterrupts(){
	//attachInterrupt(okButtonPin, onOkButtonChanged, CHANGE);
	//attachInterrupt(BUTTON_PIN, onPageButtonChanged,RISING );
	//attachInterrupt(okButtonPin, onOkButtonReleased, FALLING);
}

void initFS(){
	bool result = SPIFFS.begin();
	Serial.println("SPIFFS opened: " + result);
}

void clearDisplay(){
	//lcd.clear();
	display.clear();
}

void displayHoldOn(){
	clearDisplay();
	displayLine("Read sensors...", 1,0);
	displayLine("Please wait...", 3,0);
}

void initDisplay(){

	display.begin();
	clearDisplay();

	displayLine("Balabas-soft",1,0);
	displayLine("Loading....",3,0);
	displayLine("Hold Button to",5,0);
	displayLine("reset device",6,0);

	displayTrigger.setActive(true);
	displayOn=true;

	delay(1000);
}

void turnOnDisplay(){

	  display.on();
	  clearDisplay();
	  displayHoldOn();
	  //display.print("Request sensors...HoldOn");

	//lcd.backlight();
	  displayTrigger.saveTime();
	  displayTrigger.setActive(true);

	displayOn=true;
}

void turnOffDisplay(){

	if(!espSettingsBox.displayAlvaysOn){
		displayOn=false;
		displayTrigger.setActive(false);

		display.off();
	}
}

void refreshDisplay(){

	if(displayOn){
		String str;

		uint8_t sb=curPage-1;
		uint8_t dc=dallasTemperatures[sb].getDeviceCount();

		if(curPage==0){

			clearDisplay();


			if(!espSettingsBox.isAccesPoint){
				displayLine("http://",0,0);
				displayLine(WiFi.localIP().toString(),1,0);

				uint8_t sc=totalDetectedSensors;
				str=String(sc)+" sensors";
				displayLine(str,3,0);

				str=" on ("+String(totalActivePages)+") pages";

				displayLine(str,5,0);
			}else{
				String ipLine="IP="+WiFi.softAPIP().toString();

				Serial.println(WiFi.softAPIP());
				Serial.println(ipLine);

				displayLine("SETUP MODE",0,0);
				displayLine("CONNECT AND SET",1,0);
				displayLine(espSettingsBox.ssidAP,2,0);
				displayLine(ipLine,4,0);
				displayLine("PORT=80",6,0);
			}

		}else{

			if(sensorTrigger.checkTrigger()){
				displayHoldOn();
				refreshSensors(true);
			}


			clearDisplay();

			if(curPage==(sensorBusCount+1)){
				//DHT sensor

				str=String(DHTTYPE);
				displayLine(str,0,0);

				str="----------------";
				displayLine(str,1,0);

				str="Hum: "+String(dht.readHumidity(false));
				displayLine(str,3,0);

				str="Temp:"+String(dht.readTemperature(false, false));
				displayLine(str,5,0);
			}else{
				//OneWire sensors
			str=sensorPinDescr[sb];
			if((dc)!=0){
				str+=" (";
				str+=String(dc);
				str+=" sensors) ";
			}else{
				str+=" (NO SENSORS)";
			}
			displayLine(str,0,0);

			str="----------------";
			displayLine(str,1,0);

			if(dc!=0){
				int rov=3;

				for(int j=0;j<dc;j++){
					float val=dallasTemperatures[sb].getTempCByIndex(j);

					char iStr[4];
					char fStr[5];

					sprintf(iStr, "%d", j);
					sprintf(fStr, "%.2f", val);

					str=String(iStr);
					displayLine(str,rov,0);


					str=String(fStr);
					displayLine(str,rov,5);

					rov++;
				}
			}
		}
		}

		Serial.println("Screen refresh complete");
	}

}

void displayLine(String line,uint8_t r,uint8_t c){
	char l[16];
	strcpy(l, line.c_str());
	display.print(l, r,c);
}

boolean onPageButtonChanged(){
	int button = digitalRead(BUTTON_PIN);

	  if(button == HIGH)
	  {
		  pageButtonHandleInLoop=true;
		  return true;

	  }

  return false;
}

void selectNextDisplayPage(){
	 if(displayOn){
		  curPage=getNextActivePage();
	  }

	  if(curPage>getMaxPossPage()){
		  //hm hov?
		  curPage=0;
	  }
}

void handlePageButton(){
	onPageButtonChanged();

	if(pageButtonHandleInLoop){
		if(espSettingsBox.displayAlvaysOn){
			displayOn=true;
			turnOnDisplay();
		}

		selectNextDisplayPage();

		  if(!displayOn){
			  turnOnDisplay();
		  }

		  Serial.print("CurPage=");
		  Serial.println(curPage);

		  refreshSensors(true);
		  refreshDisplay();

		  displayTrigger.saveTime();

		  pageButtonHandleInLoop=false;
	}
}

void startAlertMode(){
	alertBeepCount=0;

	alertTriggerDelay.saveTime();
	alertTriggerDelay.setInterval(1000);
	alertTriggerDelay.setActive(true);

	alertTrigger.saveTime();
	alertTrigger.setInterval(3000);
	alertTrigger.setActive(true);

	if(espSettingsBox.alarmSendNotifAlertStart){
		sendALarmNotificationTS("Устройство "+espSettingsBox.DeviceId+" перешло в режим тревоги!!!");
	}

	alertMode=true;
	//redLed.turnOn();
	Serial.println("----red led to be started-----");
	Serial.println("Alert mode started");
}

void stopAlertMode(){
	alertTriggerDelay.setActive(false);
	alertMode=false;
	if(espSettingsBox.alarmSendNotifAlertStop){
		sendALarmNotificationTS("Устройство "+espSettingsBox.DeviceId+" Значения датчиков в пределах нормы. Режим тревоги выключен");
	}
	if(espSettingsBox.alarmPlaySound){
		//beeper.play(2500,500);
	}
	//redLed.turnOff();
	Serial.println("Alert mode finished");
}

void handleAlertNotifSound(){
	if(alertMode){
		if(alertTriggerDelay.checkTrigger()){
			if(alertTrigger.checkTrigger()){

				Serial.println("ALertGenerated Beep Beep");
				if(espSettingsBox.alarmPlaySound){
					//beeper.play(2500,2500);
				}
				alertBeepCount++;

				alertTrigger.saveTime();
				if(alertBeepCount>2){
					uint16_t inter=espSettingsBox.alamSoundInterval*1000;
					if(inter<30000){
						inter=31000;
					}
					alertTriggerDelay.setInterval(inter);
					alertTriggerDelay.setActive(true);
					alertTriggerDelay.checkTriggerAndSaveTime();
					alertBeepCount=0;
					Serial.print("-----Beep series  completed---Time Interval=");
					Serial.print(espSettingsBox.alamSoundInterval*1000);
				}
			}
		}
	}
}

void sendALarmNotificationTS(String message){
	if(message.length()!=0 && espSettingsBox.thSkTKey.length()!=0){
		message="http://api.thingspeak.com/apps/thingtweet/1/statuses/update?api_key="+espSettingsBox.thSkTKey+"&status="+message;

		int8_t sendCount=0;

		HTTPClient http;  //Declare an object of class HTTPClient

		while(sendCount>-1 && sendCount<4){
			http.begin(message);
			Serial.println("send message to thingSpeak");
			delay(1);
			int httpCode = http.GET();                                                                  //Send the request
			delay(1);
			if (httpCode > 0) { //Check the returning code

			  String payload = http.getString();
			  Serial.println("-----response from thingSpeak----");
			  Serial.println(payload);
			  Serial.println("---------------------------------");
			  sendToThingSpeakTriggered=false;
			  thingSpeakTrigger.saveTime();
			  sendCount=-1;

			}else{
				Serial.print("Send failed status");
				Serial.println(httpCode);
				Serial.println(http.errorToString(httpCode));
				sendCount++;
			}
		}
	}
}

void checkDisplayPageAutochange(){

	if(displayPageChangeTrigger.checkTrigger()){
		selectNextDisplayPage();
		refreshDisplay();
		displayPageChangeTrigger.saveTime();
	}
}

void handleDisplayPover(){
	if(!espSettingsBox.displayAlvaysOn){
		if(displayTrigger.checkTrigger()){
			Serial.println("Display sleep triggered");
			turnOffDisplay();
			delay(1);
		}
	}
}

uint8_t getMaxPossPage(){
	uint8_t result=sensorBusCount+1;
	return result;
}

uint8_t getNextActivePage(){
	uint8_t maxPossiblePage=getMaxPossPage();

	if(totalDetectedSensors==0 || curPage==maxPossiblePage){
		Serial.println("totalDetectedSensors==0 || curPageIn==maxPossiblePage");
		return 0;
	}

	for(uint8_t i=curPage+1;i<=maxPossiblePage;i++){
		if(activePages[i]){
			return i;
		}
	}

	return 0;
}

void refreshSensors(boolean displayDataActual){
	if(sensorTrigger.checkTrigger()){

	for (int i=0;i<sensorBusCount;i++){
		dallasTemperatures[i].requestTemperatures();
	}

	unsigned long startTime=millis();
	boolean timeOut=false;

	while(!timeOut){
		humidity=dht.readHumidity(true);

		timeOut=!isnan(humidity);

		if(!timeOut && (startTime+5000>millis())){
			timeOut=true;
		}

		if(!timeOut){
			delay(1000);
		}
	}

	startTime=millis();
	timeOut=false;

	while(!timeOut){

		temperature=dht.readTemperature(false, true);

		timeOut=!isnan(temperature);

		if(!timeOut && (startTime+5000>millis())){
			timeOut=true;
		}

		if(!timeOut){
			delay(1000);
		}

	}


		Serial.println("Sensors requested and refreshed");

		sensorTrigger.saveTime();

		checkHighLowValues();
	}else{
		if(displayDataActual){
			Serial.println("Sensors requested. Still actual data");
		}
	}
}

int initSensors(boolean clearSettings){
	sensorsKVContainer=KeyValueContainer("/sensors.txt");

	sensorHelper.setKeyValueContainer(&sensorsKVContainer);

	uint8_t totalOneVireSensors=0;
	uint8_t totalDHTSensors=0;

	//OneVire section
	for(int i=0;i<sensorBusCount;i++){
		dallasTemperatures[i]=DallasTemperature(&oneWires[i]);
		sensorHelper.initSensor(&dallasTemperatures[i], sensorPinDescr[i]);
		uint8_t sOnCDev=dallasTemperatures[i].getDeviceCount();
		totalOneVireSensors+=sOnCDev;

		if(sOnCDev!=0){
			sensorHelper.putKeyValue(sensorPinDescr[i], sensorPinDescr[i]+"_value");

			activePages[i+1]=true;
			totalActivePages++;

			for(uint8_t j=0;j<sOnCDev;j++){
				String deviceAddressD=sensorHelper.
						getSensorAddressByIndexStr(&dallasTemperatures[i], j);

				sensorHelper.putKeyValue(deviceAddressD, deviceAddressD+"_value");
			}
		}
	}

	//DHT section

	dht.begin();
	dhtActive=dht.read(true);

	if(dhtActive){

		sensorHelper.putKeyValue(dhtName, dhtName+"_value");
		sensorHelper.putKeyValue(dhtHumidityAddress, dhtHumidityAddress+"_value");
		sensorHelper.putKeyValue(dhtTemperatureAddress, dhtTemperatureAddress+"_value");

		activePages[sensorBusCount+1]=true;
		totalDHTSensors++;
		totalActivePages++;
	}

	totalDetectedSensors=totalOneVireSensors+totalDHTSensors;

	Serial.println("Sensors initialized");
	Serial.print("OneVire count=");
	Serial.println(totalOneVireSensors);
	Serial.print("DHT count=");
	Serial.println(totalDHTSensors);
	Serial.print("totalDetectedSensors=");
	Serial.println(totalDetectedSensors);
	Serial.print("totalActivePages=");
	Serial.println(totalActivePages);
	Serial.print("activePages={");
	for(uint8_t i=0;i<sizeof(activePages);i++){
		Serial.print(activePages[i]);
		if(i!=sizeof(activePages)-1){
			Serial.print(",");
		}
	}
	Serial.println("}");

	Serial.println("------------------------------");

	sensorsKVContainer.saveToFile(clearSettings);
	sensorsKVContainer.loadFromFile();

	return totalDetectedSensors;
}

void checkHighLowValues(){
	boolean result=false;

	if(dhtActive){
		float hum=dht.readHumidity(false);
		float temp=dht.readTemperature(false, false);

		if(!isnan(hum)){
			if(hum>=espSettingsBox.maxDHtHum){
				result=true;
			}
			if(hum<=espSettingsBox.minDHtHum){
				result=true;
			}
		}
		if(!isnan(temp)){
			if(temp>=espSettingsBox.maxDHtTemp){
				result=true;
			}
			if(temp<=espSettingsBox.minDHtTemp){
				result=true;
			}
		}
	}

	for(uint8_t busI=0;busI<sensorBusCount;busI++){
		uint8_t c=dallasTemperatures[busI].getDeviceCount();

		if(c>0){
			if(dallasTemperatures[busI].hasAlarm()){
				Serial.print("Alarm found on bus=");
				Serial.println(busI);
				result=true;
			}
		}
	}

	if(result){
		Serial.println("startAlarmModeHere();");
		if(!alertMode){
			startAlertMode();
		}
	}else{
		Serial.println("---no need to start alarmMode");
		if(alertMode){
			stopAlertMode();
		}
	}
}

void setupWiFi(){
	Serial.println("CHecking is access point");
	clearDisplay();

	if(espSettingsBox.isAccesPoint){
		WiFi.mode(WIFI_AP);
		displayLine("Start",3,0);
		displayLine("       AccessPoint",5,0);
		Serial.println("STarting as access point");
		/*
		 WiFi.softAPConfig(espSettingsBox.apIp,espSettingsBox.gateIp,
				espSettingsBox.subnetIp);
				*/
		//WiFi.softAP(const_cast<char*>(espSettingsBox.ssidAP.c_str()),const_cast<char*>(espSettingsBox.passwordAP.c_str()));
		WiFi.softAP(const_cast<char*>(espSettingsBox.ssidAP.c_str()));
		//String apName="ESP "+ESP.getChipId();
		//WiFi.softAP(espSettingsBox.ssidAP);

		Serial.println ( "-----------" );
		Serial.print ( "SoftAP " );
		Serial.println ( espSettingsBox.ssidAP);
		Serial.print ( "IP address: " );
		Serial.println ( WiFi.softAPIP() );
		Serial.println ( "-----------" );

		Serial.print("ViFi status");
		Serial.println(WiFi.status());
		Serial.println ( "-----------" );

	}else{
		displayLine("connect to",2,0);
		displayLine(espSettingsBox.ssid,5,0);
		Serial.print("espSettingsBox.ssid=");
		Serial.println(espSettingsBox.ssid);
		Serial.print("espSettingsBox.password=");
		Serial.println(espSettingsBox.password);

		WiFi.mode(WIFI_STA);

		WiFi.config(espSettingsBox.localIp,espSettingsBox.gateIp,
				espSettingsBox.subnetIp,espSettingsBox.dnsIp,espSettingsBox.dnsIp2);
		WiFi.begin ( const_cast<char*>(espSettingsBox.ssid.c_str()),
				const_cast<char*>(espSettingsBox.password.c_str()) );

		// Wait for connection

		greenLed.turnOn();

		connectToWiFiIfNotConnected();

		greenLed.turnOff();

		Serial.println ( "" );
		Serial.print ( "Connected to " );
		Serial.println ( WiFi.SSID() );
		Serial.print ( "IP address: " );
		Serial.println ( WiFi.localIP() );
	}

		Serial.println ( "" );

		server.on ( "/", handleRoot );
		server.on ( "/getJson", handleJson );
		server.on ( "/setup", HTTP_GET, getPageHtmlSetup );
		server.on ( "/setup", HTTP_POST, getPageHtmlSettingsForm );
		server.onNotFound ( handleNotFound );
		server.serveStatic("/css/w3.css", SPIFFS, "/css/w3.css");
		server.serveStatic("/js/main.js", SPIFFS, "/js/main.js");

		server.begin();
		 //MDNS.addService("http", "tcp", 80);
		 //Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", DNS_NAME);
		Serial.println ( "HTTP server started" );

		Serial.print("Status=");
		Serial.println(WiFi.status());
		Serial.print(" Mac=");
		Serial.println(WiFi.macAddress());

}

void connectToWiFiIfNotConnected(){
	if(WiFi.status() != WL_CONNECTED){
	unsigned long start=millis();

	greenLed.turnOff();

	while ( WiFi.status() != WL_CONNECTED ) {
			greenLed.turnOff();
			delay ( 500 );
			Serial.print ( "status=" );
			Serial.print ( WiFi.status() );
			greenLed.turnOn();
			if(start+60000<millis()){
				Serial.println("can not connect to vifi. Restart");
				//beeper.play(2500,4000);
				//ESP.reset();
				displayLine("ERROR WiFi",0,0);
				delay(600000);
			}
		}
	}
}

char* getDateTimeStr(){
	char str[15];

	RtcDateTime dt= rtc.GetDateTime();
	sprintf(str, "%d-%d-%d %d:%d:%d",     //%d allows to print an integer to the string
		 dt.Year(),   //get year method
		 dt.Month(),  //get month method
		 dt.Day(),
		 dt.Hour(),   //get hour method
		 dt.Minute(), //get minute method
		 dt.Second()//get second method
		 );

	//Serial.println(str);

	return str;
}

String getDateStrMinus(){
	char str[11];

	RtcDateTime dt= rtc.GetDateTime();
	sprintf(str, "%d-%02d-%02d",     //%d allows to print an integer to the string
		 dt.Year(),   //get year method
		 dt.Month(),  //get month method
		 dt.Day()  //get second method
		 );

	//Serial.println(str);

	return String(str);
}

String getDateStr(){
	char str[11];

	RtcDateTime dt= rtc.GetDateTime();
	sprintf(str, "%d/%d/%d",     //%d allows to print an integer to the string
		 dt.Year(),   //get year method
		 dt.Month(),  //get month method
		 dt.Day()  //get second method
		 );

	//Serial.println(str);

	return String(str);
}

String getTimeStr(){
	char str[9];

	RtcDateTime dt= rtc.GetDateTime();
	sprintf(str, "%02d:%02d:%02d",     //%d allows to print an integer to the string
		 dt.Hour(),   //get hour method
		 dt.Minute(), //get minute method
		 dt.Second()  //get second method
		 );

	//Serial.println(str);

	return String(str);
}



void handleRoot() {

	Serial.println("Handle root");

	refreshSensors(true);
	//refreshDisplay();

	getPageHtmlRoot();

}

void checkAuthentication(){
	if(espSettingsBox.settingsUser.length()!=0 && espSettingsBox.settingsPass.length()!=0){
		Serial.println("Authentication is REQUIRED for setupPage");
		if(!server.authenticate(const_cast<char*>(espSettingsBox.settingsUser.c_str()),
			const_cast<char*>(espSettingsBox.settingsPass.c_str()))){
			return server.requestAuthentication();
		}else{
			Serial.println("User entered correct User/pass");
		}
	}else{
		Serial.println("Authentication is not required for setupPage");
	}
}

String errorString="";
String warnString="";
String successString="";
String redirectUrl="";
String redirectDelay="";

void getPageHtmlSettingsForm(){
	checkAuthentication();

	if(server.args()!=0){
		String vievName=server.arg("vievName");
		String form=server.arg("form");
		errorString="";
		warnString="";
		successString="";
		redirectUrl="";
		redirectDelay="";
		String html="";

		if(form.length()!=0){
			//process submit form
			Serial.println("Processing form submit");
			processSettingsRequestParams(&vievName,&redirectUrl,&redirectDelay,
					&errorString, &warnString,&successString);
			processPageRedirect(&errorString, &warnString,&successString,&redirectUrl,&redirectDelay);

			html=processErrorsViev(&errorString,&warnString,&successString);
			//formsubmit processed
		}else{

			Serial.print("generating viev=");
			Serial.println(vievName);

			if(vievName=="info"){
				html+=getFormDeviceInfo();
			}else{
			if(vievName=="device"){
				html+=getFormDeviceSettings();
			}else{
				if(vievName=="net"){
					html+=getFormNetSettins();
				}else{
					if(vievName=="thingspeak"){
						html+=getFormThingSpeakSetings();
					}else{
						if(vievName=="sensors"){
							html+=getFormSensorsSettins();
						}else{
							if(vievName=="restart"){
								html+=getFormRestart();
							}else{
								if(vievName=="alarm"){
									html+=getFormAlarmSettins();
								}
							}
						}
					}
					}
				}
			}
		}
		Serial.println("Send html result");
		Serial.println(html);
		Serial.println("------------");
		delay(1);
		server.send ( 200, "text/html", html );
	}

	server.send(200,"text/html","Что-то не так на стороне сервера");

}

String processErrorsViev(String *errorString,String *warnString,String *successString){
	String result="";

	if(errorString->length()>0){
		result+="<div class=\"w3-panel w3-red w3-card-4\">\
			  <h3>Ошибка!</h3>\
				<p>";
		result.concat(errorString->c_str());
		result+="</p>\
			</div>";
	}
	if(warnString->length()>0){
		result+="<div class=\"w3-panel w3-yellow w3-card-4\" >\
			  <h3>Предупреждение!</h3>\
				<p>";
		result.concat(warnString->c_str());
		result+="</p>\
			</div>";
	}
	if(successString->length()>0){
		result+="<div class=\"w3-panel w3-green w3-card-4\">\
				<p>";
		result.concat(successString->c_str());
		result+="</p>\
			</div>";
	}

	return result;
}

void processSettingsRequestParams(String *vievName,String *redirectUrl,String *redirectDelay,
									String *errorString,String *warnString,String *successString)
{
	boolean error=false;

	if(server.args()!=0){

		for(int i=0;i<server.args();i++){
			Serial.print("---REQUEST--- argName=");
			Serial.print(server.argName(i));
			Serial.print(" ; argValue=");
			Serial.println(server.arg(i));
		}

		Serial.print("Process vievName=");
		Serial.println(vievName->c_str());

		if(vievName){
			if(vievName->equals("device")){
				if(server.args()!=1){
					String s_accessUser=server.arg("s_accessUser");
					String s_accessPass=server.arg("s_accessPass");

					String time=server.arg("s_time");
					String date=server.arg("s_date");

					String s_refreshInterval=server.arg("s_refreshInterval");
					/*if(server.arg("s_displayAutochange").toInt()!=0 && server.arg("s_displayAutochange").toInt()<1){
						error=true;
						errorString->concat("Интервал переключения дисплея должен быть>15 и <120. 0 если неактивно; ");
					}
*/
					if(!s_refreshInterval || s_refreshInterval.length()==0
							|| s_refreshInterval.toInt()<60){
						errorString->concat("Интервал отсылки должен быть больше 60; ");
						error=true;
					}

					if(((!s_accessUser || s_accessUser.length()==0)  && (s_accessPass && s_accessPass.length()!=0))||((!s_accessPass || s_accessPass.length()==0) &&  (s_accessUser && s_accessUser.length()!=0))){
						errorString->concat("Пользователь/пароль пользователя должны быть пустыми или быть заданы; ");
						error=true;
					}

					if(!error){
						if(date.length()!=0 && time.length()!=0){
							String year="";
							String month="";
							String day="";

							String hour="";
							String minute="";
							String second="";
							//main settings form submitted
							hour=time.substring(0, 2);
							minute=time.substring(3, 5);
							second=time.substring(6, 8);

							year=date.substring(0, 4);
							month=date.substring(5, 7);
							day=date.substring(8, 10);

							RtcDateTime dateTime=RtcDateTime(year.toInt(),month.toInt(),day.toInt(),hour.toInt(),minute.toInt(),second.toInt());
							rtc.SetDateTime(dateTime);
						}

						espSettingsBox.refreshInterval=s_refreshInterval.toInt();
						espSettingsBox.DeviceId=server.arg("s_DeviceId");
						espSettingsBox.settingsUser=server.arg("s_settingsUser");
						espSettingsBox.settingsPass=server.arg("s_settingsPass");
						espSettingsBox.accessUser=s_accessUser;
						espSettingsBox.accessPass=s_accessPass;
						espSettingsBox.displayAutochange=server.arg("s_displayAutochange").toInt();
						espSettingsBox.pageAutochange=server.arg("s_pageAutochange").toInt();
						espSettingsBox.displayAlvaysOn=(server.arg("s_displayAlvaysOn")=="on")?true:false;

						espSettingsBox.saveSettingsJson();
						refreshTimeTriggers();

						successString->concat("Настройки устройства сохранены");
					}
			}
			}else{
				if(vievName->equals("net")){

					if(server.args()!=1){
						String s_isAccesPoint=server.arg("s_isAccesPoint");

						boolean isAP=(s_isAccesPoint=="on"?true:false);

						if(isAP){
								warnString->concat("Желательно подключить устройство к роутеру. В режиме точка доступа невозможно отправить данные на сервис https://thingspeak.com");
						}

						bool rstReq=false;

						if(!error){
							if((espSettingsBox.isAccesPoint!=isAP)){
								rstReq=true;
							}

							espSettingsBox.isAccesPoint=isAP;
							espSettingsBox.ssidAP=server.arg("s_ssidAP");
							//espSettingsBox.passwordAP=server.arg("s_passwordAP");
							espSettingsBox.ssid=server.arg("s_ssid");
							espSettingsBox.password=server.arg("s_password");
							espSettingsBox.localIp=espSettingsBox.stringToIp(server.arg("s_localIp"));
							espSettingsBox.apIp=espSettingsBox.stringToIp(server.arg("s_apIp"));
							espSettingsBox.gateIp=espSettingsBox.stringToIp(server.arg("s_gateIp"));
							espSettingsBox.subnetIp=espSettingsBox.stringToIp(server.arg("s_subnetIp"));
							espSettingsBox.dnsIp=espSettingsBox.stringToIp(server.arg("s_dnsIp"));
							espSettingsBox.dnsIp2=espSettingsBox.stringToIp(server.arg("s_dnsIp2"));

							espSettingsBox.saveSettingsJson();
							successString->concat("Настройки сети сохранены.");
							if(rstReq){
								restartTrigger=TimeTrigger(millis(),3000,true);
								warnString->concat("Устройство перегружается");

							}
						}
					}

				}else{
					if(vievName->equals("thingspeak")){
						Serial.println("process thingSpeak details save");
						delay(1);

						if(server.args()!=1){
							delay(1);

							espSettingsBox.postDataToTSInterval=server.arg("s_postDataToTSInterval").toInt();
							espSettingsBox.thSkUsrKey=server.arg("s_thSkUsrKey");
							espSettingsBox.thSkWKey=server.arg("s_thSkWKey");
							espSettingsBox.thSkRKey=server.arg("s_thSkRKey");
							espSettingsBox.thSkTKey=server.arg("s_thSkTKey");
							espSettingsBox.thSkChId=server.arg("s_thSkChId").toInt();

							espSettingsBox.saveSettingsJson();
							refreshTimeTriggers();

							successString->concat("Настройки ThingSpeak сохранены");
						}
					}else{
					if(vievName->equals("sensors")){

						boolean noerror=true;
						delay(1);

						String preffix="";
						String keyValue="";
						String argValue="";

						//validate TS id
						int tsId[8]={-1,-1,-1,-1,-1,-1,-1,-1};

						for(int i=0;i<server.args();i++){

							 preffix=server.argName(i).substring(0,3);
								if(preffix.equals("SR_")){
									keyValue=server.argName(i).substring(3);
									argValue=server.arg(i);

									delay(1);
								}
								if(preffix.equals("TS_")){
									keyValue=server.argName(i).substring(3);
									argValue=server.arg(i);

									delay(1);

									if(argValue.toInt()>8 || argValue.toInt()<0){
										noerror=false;
										errorString->concat("Неверное значение ключа TS");
									}

									uint8_t ind=argValue.toInt()-1;

									if(argValue.toInt()>0){
										if(tsId[ind]!=-1){
											errorString->concat("Ключ TS не уникален "+argValue+"; ");
											noerror=false;
										}else{
											tsId[ind]=argValue.toInt();
										}
									}
								}
							}

						delay(1);

						if(noerror){
							for(int i=0;i<server.args();i++){

							 preffix=server.argName(i).substring(0,3);
								if(preffix.equals("SR_")){
									keyValue=server.argName(i).substring(3);
									argValue=server.arg(i);
									Serial.print("sensors descriptor found key=");
									Serial.print(keyValue);
									Serial.print("; value=");
									Serial.print(argValue);
									Serial.println(";");

									delay(1);
									KeyValue kv=sensorsKVContainer.getKeyValue(keyValue);
									kv.setValue(argValue);
									sensorsKVContainer.putKeyValue(kv);

								}
								if(preffix.equals("TS_")){
									keyValue=server.argName(i).substring(3);
									argValue=server.arg(i);

									delay(1);
									if(argValue.length()>0){
										KeyValue kv=sensorsKVContainer.getKeyValue(keyValue);
										kv.updateKind(argValue.toInt());
										sensorsKVContainer.putKeyValue(kv);
									}
								}
							}

							sensorsKVContainer.saveToFile(true);
							successString->concat("Настройки датчиков сохранены");
						}

						//redirectUrl->concat("http://"+WiFi.localIP().toString()+"/setup?page=sensors");
						//redirectDelay->concat("3");
					}else{
					if(vievName->equals("alarm")){

						String preffix="";
						String busValue="";
						String keyValue="";
						String argValue="";
						DeviceAddress termAddr;

						uint8_t ind=-1;

						for(int i=0;i<server.args();i++){

						 preffix=server.argName(i).substring(0,3);
							if(preffix.equals("ht_")){
								keyValue=server.argName(i).substring(3);
								argValue=server.arg(i);
								ind=keyValue.indexOf("_");
								Serial.print("keyValue=");
								Serial.println(keyValue);
								Serial.print("; ind=");
								Serial.print(ind);
								busValue=keyValue.substring(0,ind);
								keyValue=keyValue.substring(ind+1);
								Serial.print("; sensors ht found index=");
								Serial.print(keyValue);
								Serial.print("; bus=");
								Serial.print(busValue);
								Serial.print("; value=");
								Serial.print(argValue);
								Serial.print(";");

								delay(1);
								dallasTemperatures[busValue.toInt()].getAddress(termAddr, keyValue.toInt());

								for(uint8_t k=0;k<sizeof(termAddr);k++){
									Serial.print(termAddr[k]);
									}
								Serial.println();

								dallasTemperatures[busValue.toInt()].setHighAlarmTemp(termAddr, argValue.toInt());
							}
							if(preffix.equals("lt_")){
								keyValue=server.argName(i).substring(3);
								argValue=server.arg(i);
								ind=keyValue.indexOf("_");
								Serial.print("keyValue=");
								Serial.println(keyValue);
								Serial.print("; ind=");
								Serial.print(ind);
								busValue=keyValue.substring(0,ind);
								keyValue=keyValue.substring(ind+1);
								Serial.print("sensors lt found index=");
								Serial.print(keyValue);
								Serial.print("; bus=");
								Serial.print(busValue);
								Serial.print("; value=");
								Serial.print(argValue);
								Serial.println(";");

								delay(1);

								dallasTemperatures[busValue.toInt()].getAddress(termAddr, keyValue.toInt());

								for(uint8_t k=0;k<sizeof(termAddr);k++){
									Serial.print(termAddr[k]);
									}
								Serial.println();
								dallasTemperatures[busValue.toInt()].setLowAlarmTemp(termAddr, argValue.toInt());
							}
						}

						if(dhtActive){
							espSettingsBox.maxDHtTemp=server.arg("s_maxDHtTemp").toInt();
							espSettingsBox.minDHtTemp=server.arg("s_minDHtTemp").toInt();
							espSettingsBox.maxDHtHum=server.arg("s_maxDHtHum").toInt();
							espSettingsBox.minDHtHum=server.arg("s_minDHtHum").toInt();
						}

						espSettingsBox.alarmSendNotifAlertStop=(server.arg("s_alarmSendNotifAlertStop")=="on"?true:false);
						espSettingsBox.alarmSendNotifAlertStart=(server.arg("s_alarmSendNotifAlertStart")=="on"?true:false);
						espSettingsBox.alarmPlaySound=(server.arg("s_alarmPlaySound")=="on"?true:false);
						espSettingsBox.alamSoundInterval=server.arg("s_alamSoundInterval").toInt();

						successString->concat("Настройки режима тревоги сохранены");
					}else{
						if(vievName->equals("restart")){

							if(server.args()!=1){
								if(server.arg("doReset")=="doReset"){
									restartTrigger=TimeTrigger(millis(),3000,true);
									warnString->concat("Устройство перегружается");

									redirectUrl->concat("http://"+WiFi.localIP().toString()+"/setup?vievName=restart");
									redirectDelay->concat("30");
									Serial.print("Restart triggered at ");
									Serial.println(millis());
								}
								if(server.arg("doReset")=="sensors"){
									initSensors(true);
									warnString->concat("Датчики были перезагружены");
								}
								if(server.arg("doReset")=="sendToTS"){
									sendToThingSpeakTriggered=true;
									warnString->concat("Даные будут отправлены в ThingSpeak");
								}
								if(server.arg("doReset")=="beep"){

									if(alertMode){
										stopAlertMode();
										successString->concat("Режим аварии выключен");
									}else{
										//alertMessage="Тестовое сообщение Тревоги";
										startAlertMode();
										warnString->concat("Режим Аварии активирован");
									}
								}

							}
						}
					}
					}
					}
				}
				}
			}
		}

}

void processPageRedirect(String *errorString,String *warnString,String *successString,
		String *redirect,String *redirectDelay){
	if(redirect->length()!=0){
		String result="<!DOCTYPE html>\
				<html>\
				<title>Настройки устройства</title>\
				<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
				<meta charset=\"utf-8\">\
				<meta http-equiv=\"refresh\" content=\"";
				result.concat(redirectDelay->c_str());
				result+="; URL='";
				result.concat(redirect->c_str());
				result+="'\" />\
				<link rel=\"stylesheet\" href=\"/css/w3.css\">\
				<body>\
					<script>\
						window.postUpdateForm=function(){\
						alert('111');\
						var countdownElement = document.getElementById('cdt');\
						var	interval,\
						var	seconds = ";
				result.concat(redirectDelay->c_str());
				result+="-5;\
						interval = setInterval(function() {\
							document.getElementById('cdt').innerHTML = seconds+' сек';\
							if (seconds<=0) {\
								alert('restart page');\
								loadForm('restart','mainContent');\
							}else{\
							seconds--;};\
						}, 1000);}\
					</script>\
				<div class=\"w3-container w3-animate-opacity\">\
					<div class=\"w3-container w3-card-4\" style=\"margin-top: 30px;\">\
					<div style=\"float: left;\"><b >Обрабатываем данные. Страница обновится через   </b></div>\
					<div id=\"cdt\" style=\"float: right;\"></div></div><p>";
					if(errorString->length()>0){
						result+="<div class=\"w3-panel w3-red w3-card-4\">\
							  <h3>Ошибка!</h3>\
								<p>";
						result.concat(errorString->c_str());
						result+="</p>\
							</div>";
					}
					if(warnString->length()>0){
						result+="<div class=\"w3-panel w3-yellow w3-card-4\" style=\"width: 50%\">\
							  <h3>Предупреждение!</h3>\
								<p>";
						result.concat(warnString->c_str());
						result+="</p>\
							</div>";
					}
					if(successString->length()>0){
						result+="<div class=\"w3-panel w3-green w3-card-4\" style=\"width: 50%\">\
							  <h3>Отлично :-)!</h3>\
								<p>";
						result.concat(successString->c_str());
						result+="</p>\
							</div>";
					}
			result+="</p></div></body>\
				</html>";

				server.send ( 200, "text/html", result );
				return;
	}else{
		Serial.println("Redirect is not requested");
	}
}


void getPageHtmlRoot(){
	String result="<!DOCTYPE html>\
			<html>\
			<title>Датчики</title>\
			<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
			<meta http-equiv=\"refresh\" content=\"";
			result+=espSettingsBox.pageAutochange;
			result+="\">\
			<meta charset=\"utf-8\">\
			<link rel=\"stylesheet\" href=\"/css/w3.css\">\
			<body>\
				<script>\
					function w3_open() {\
					  document.getElementById(\"leftSidebar\").style.display = \"block\";\
					}\
					function w3_close() {\
					  document.getElementById(\"leftSidebar\").style.display = \"none\";\
					}\
				</script>\
			<div class=\"w3-sidebar w3-bar-block w3-light-grey w3-border-right w3-animate-left\" style=\"display:none\" id=\"leftSidebar\">\
			  <button class=\"w3-bar-item w3-button w3-large\" onclick=\"w3_close()\">Свернуть &times;</button>\
			  <a href=\"setup\" class=\"w3-bar-item w3-button w3-hover-green\">Настройки</a>\
			  <a href=\"https://thingspeak.com/channels/";
				result+=espSettingsBox.thSkChId;
				result+="/private_show\" class=\"w3-bar-item w3-button w3-hover-green\">ThingSpeak.com</a>\
			</div>\
	\
			<div id=\"main\">\
	\
			<div class=\"w3-teal\">\
			  <button id=\"openNavBtn\" class=\"w3-button w3-teal w3-xlarge\" onclick=\"w3_open()\">&#9776;</button>\
			  <div class=\"w3-container w3-animate-opacity\">\
				<h1>Устройство ";
					result+=espSettingsBox.DeviceId;
		 result+="</h1>\
			  </div>\
			</div>\
	\
				<div class=\"w3-container\">\
			<!--Main content starts here-->";
					if(dhtActive){
						result+="\
						<!--DHT info starts-->\
						<div class=\"w3-card-4\" style=\"width:25%;  float:left; min-width: 250px; min-height:270px; margin-left: 10px; margin-top: 10px; margin-right: 10px; margin-bottom: 10px;\">\
							<div class=\"w3-container w3-teal\">\
							  <h2>";
								  result+=sensorHelper.getValueByKey(dhtName);
					   result+="</h2>\
							   </div>\
							   <form class=\"w3-container\">\
								<label style=\"max-width: 240px;\"><b>";
							result+=sensorHelper.getValueByKey(dhtHumidityAddress);
						  result+="</b></label>\
								<input class=\"w3-input w3-border\" style=\"width:95%\" name=\"s_humValue\" type=\"text\" value=\"";
								result+=dht.readHumidity(false);
								result+="\" disabled>\
								<label style=\"max-width: 240px;\"><b>";
								result+=sensorHelper.getValueByKey(dhtTemperatureAddress);
								result+="</b></label>\
								<input class=\"w3-input w3-border\" style=\"width:95%\" name=\"s_tempValue\" type=\"text\" value=\"";
								result+=dht.readTemperature(false,false);
								result+="\" disabled>\
							<p></p>\
							</form>\
						</div>\
						<!--DHT info ends-->";
					}
				for(unsigned int i=0;i<sensorBusCount;i++){
					uint8_t countz=dallasTemperatures[i].getDeviceCount();
					if(countz>0){

					result+="\
						<!--OneVIre info starts-->\
						<div class=\"w3-card-4\" style=\"width:25%;  float:left; min-width: 250px; min-height:270px; margin-left: 10px; margin-top: 10px; margin-right: 10px; margin-bottom: 10px;\">\
							<div class=\"w3-container w3-teal\">\
							  <h2>";
									result+=sensorHelper.getValueByKey(dallasTemperatures[i].getDescription());
					   result+="</h2>\
							   </div>\
							   <form class=\"w3-container\">";
					   result+=sensorHelper.sensorToHtmlV2(&dallasTemperatures[i]);

					   result+="<p></p>\
							</form>\
						</div>\
						<!--OneVire info ends-->";
					}
				}
				result+="<!--Main content ends here-->\
				</div>\
	\
			</div>\
	\
			</body>\
			</html>";


		server.send ( 200, "text/html", result );
}


void getPageHtmlSetup(){
delay(1);

String result="<!DOCTYPE html>\
<html>\
<title>Настройки устройства</title>\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
<meta charset=\"utf-8\">\
<link rel=\"stylesheet\" href=\"/css/w3.css\">\
<body>\
<script src=\"/js/main.js\"></script>\
<div class=\"w3-sidebar w3-light-grey w3-bar-block\" style=\"width:20%\">\
<h3 class=\"w3-bar-item\">Навигация</h3>\
<a href=\"/\" class=\"w3-bar-item w3-button w3-hover-green\">Датчики (главная)</a>\
<a onclick=\"loadForm('info','mainContent')\" class=\"w3-bar-item w3-button w3-hover-green\">Информация</a>\
<h3 class=\"w3-bar-item\">Настройки</h3>\
<a onclick=\"loadForm('device','mainContent')\" class=\"w3-bar-item w3-button w3-hover-green\">Устройство</a>\
<a onclick=\"loadForm('net','mainContent')\" class=\"w3-bar-item w3-button w3-hover-green\">Сеть</a>\
<a onclick=\"loadForm('thingspeak','mainContent')\" class=\"w3-bar-item w3-button w3-hover-green\">ThingSpeak</a>\
<a onclick=\"loadForm('sensors','mainContent')\" class=\"w3-bar-item w3-button w3-hover-green\">Датчики</a>\
<a onclick=\"loadForm('alarm','mainContent')\" class=\"w3-bar-item w3-button w3-hover-green\">Режим тревоги</a>\
<a onclick=\"loadForm('restart','mainContent')\" class=\"w3-bar-item w3-button w3-hover-green\">Управление</a>\
</div>\
<!-- Page Content -->\
<div style=\"margin-left:20%\">\
<div class=\"w3-container w3-teal w3-animate-opacity\">\
<h1>Настройки устройства</h1>\
</div>\
<div id=\"mainContent\" class=\"w3-card-4\" style=\"width:60%; min-width: 600px; margin-top:30px; margin-left:30px;  margin-bottom:30px;\">\
Выберите тип настроек\
</div>\
</div>\
</body>\
</html>";

delay(1);
Serial.println("Html setupfinished and sent");
Serial.println(result);
delay(1);
server.send ( 200, "text/html", result );
}

String getFormThingSpeakSetings(){
delay(1);
String html="<div class=\"w3-container w3-teal\">\
<h2>ThingSpeak</h2>\
</div><form id=\"settForm\" class=\"w3-container\"  action=\"\" method=\"POST\">\
<label><b>Интервал отсылки данных</b></label>\
<input class=\"w3-input w3-border\" name=\"s_postDataToTSInterval\" type=\"number\" min=\"121\" step=\"10\" value=\"";
html+=espSettingsBox.postDataToTSInterval;
html+="\" style=\"width:50%\"/>\
<label><b>Ключ User Api</b></label>\
<input class=\"w3-input w3-border\" name=\"s_thSkUsrKey\" type=\"text\" value=\"";
html+=espSettingsBox.thSkUsrKey;
html+="\" required/>\
<label><b>Ключ на запись</b></label>\
<input class=\"w3-input w3-border\" name=\"s_thSkWKey\" type=\"text\" value=\"";
html+=espSettingsBox.thSkWKey;
html+="\" required/>\
<label><b>Ключ на чтение</b></label>\
<input class=\"w3-input w3-border\" name=\"s_thSkRKey\" type=\"text\" value=\"";
html+=espSettingsBox.thSkRKey;
html+="\" required/>\
<label><b>Идентификатор канала</b></label>\
<input class=\"w3-input w3-border\" name=\"s_thSkChId\" type=\"text\" value=\"";
html+=String(espSettingsBox.thSkChId);
html+="\" required/>\
<label><b>Идентификатор Оповещения ThingSpeak</b></label>\
<input class=\"w3-input w3-border\" name=\"s_thSkTKey\" type=\"text\" value=\"";
html+=String(espSettingsBox.thSkTKey);
html+="\" required/>\
<div id=\"msgContent\" class=\"w3-card-4\" style=\"margin-left:10px; margin-right:10px; margin-top:10px; margin-bottom:10px;\"></div>\
<p>\
<button class=\"w3-btn w3-teal\" onclick=\"submitForm('thingspeak','settForm','msgContent');\">Сохранить</button>\
</p>\
</form></div>";

return html;
}

String getFormSensorsSettins(){
delay(1);
String html="<div class=\"w3-container w3-teal\">\
<h2>Датчики</h2>\
</div><form id=\"settForm\" class=\"w3-container\" action=\"\" method=\"POST\">\
<div id=\"sensorsTable\" class=\"w3-container\">\
<p>Адресс - уникальный идентификатор, Псевдоним - имя датчика или шины отображаемое на главной странице, Id thingSpeak - цифровой индекс датчика на сервисе (целое число >0 и<8) если 0 то данные не отправляются, Шина данных - физический адресс куда подключены датчики. </p>\
<p>Всего подключено датчиков <b>";
html+=String(totalDetectedSensors);
html+="</b></p>\
<table class=\"w3-table-all\" style=\"width: 95%;\">\
<thead>\
<tr class=\"w3-teal\">\
<th>Адресс</th>\
<th>Псевдоним</th>\
<th>Id thingSpeak</th>\
</tr>\
</thead>\
<tr>";
html+=sensorHelper.getSettingsHtmlTableBodyV2("SR_","TS_");
html+="</tr>\
</table>\
</div>\
<div id=\"msgContent\" class=\"w3-card-4\" style=\"margin-left:10px; margin-right:10px; margin-top:10px; margin-bottom:10px;\"></div>\
<p>\
<button class=\"w3-btn w3-teal\" onclick=\"submitForm('sensors','settForm','msgContent');\">Сохранить</button>\
</p>\
</p>\
</form>";

return html;
}

String getFormRestart(){
	delay(1);
	String html="<div class=\"w3-container w3-teal\">\
					  <h2>Управление</h2>\
					</div>\
	<form class=\"w3-container\" id=\"settForm1\" action=\"\" method=\"POST\">\
		<p><label><b>Перезагрузить устройство.</b></label></p>\
		<input type=\"hidden\" name=\"doReset\" value=\"doReset\">\
			<p>\
			<button class=\"w3-btn w3-teal\" onclick=\"submitForm('restart','settForm1','msgContent');\">Перезагрузить</button>\
			</p>\
		</form>\
	<form class=\"w3-container\" id=\"settForm2\" action=\"\" method=\"POST\">\
			<p><label><b>Перезагрузить датчики.</b></label></p>\
			<input type=\"hidden\" name=\"doReset\" value=\"sensors\">\
				<p>\
				<button class=\"w3-btn w3-teal\" onclick=\"submitForm('restart','settForm2','msgContent');\">Скинуть датчики</button>\
				</p>\
			</form>\
	<form class=\"w3-container\"  id=\"settForm3\" action=\"\" method=\"POST\">\
			<p><label><b>Отправить данные в ThingSpeak.</b></label></p>\
			<input type=\"hidden\" name=\"doReset\" value=\"sendToTS\">\
				<p>\
				<button class=\"w3-btn w3-teal\" onclick=\"submitForm('restart','settForm3','msgContent');\">Отправить данные</button>\
				</p>\
			</form>\
	<form class=\"w3-container\"  id=\"settForm4\" action=\"\" method=\"POST\">\
				<p><label><b>Найти устройство (Сделать бип-бип)</b></label></p>\
				<input type=\"hidden\" name=\"doReset\" value=\"beep\">\
					<p>\
					<button class=\"w3-btn w3-teal\" onclick=\"submitForm('restart','settForm4','msgContent');\">Бибикнуть</button>\
					</p>\
				</form><div id=\"msgContent\" class=\"w3-card-4\" style=\"margin-left:10px; margin-right:10px; margin-top:10px; margin-bottom:10px;\"></div>";

		return html;
}

String getFormNetSettins(){
delay(1);
String html="<div class=\"w3-container w3-teal\">\
<h2>Сеть</h2>\
</div>\
<form class=\"w3-container\" id=\"settForm\" action=\"\" method=\"POST\">\
<p>\
<input class=\"w3-check\" name=\"s_isAccesPoint\" type=\"checkbox\"";
html+=espSettingsBox.isAccesPoint?" checked=\"checked\"":"";
html+=">\
<label><b>Устройство работает как точка доступа</b></label></p>\
<p>Параметры точки доступа</p>\
<label><b>Имя</b></label>\
<input class=\"w3-input w3-border\" name=\"s_ssidAP\" type=\"text\" value=\"";
html+=espSettingsBox.ssidAP;
html+="\" disabled/>\
<label><b>IP устройства</b></label>\
<input class=\"w3-input w3-border\" name=\"s_apIp\" type=\"text\" value=\"";
html+=espSettingsBox.apIp.toString();
html+="\" pattern=\"((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$\" disabled/>\
<p>Параметры подключения к роутеру</p>\
<label><b>Имя (SSID)</b></label>\
<input class=\"w3-input w3-border\" name=\"s_ssid\" type=\"text\" value=\"";
html+=espSettingsBox.ssid;
html+="\" required/>\
<label><b>Пароль (SSID PASS)</b></label>\
<input class=\"w3-input w3-border\" name=\"s_password\" type=\"password\" value=\"";
html+=espSettingsBox.password;
html+="\"/>\
<label><b>IP устройства</b></label>\
<input class=\"w3-input w3-border\" name=\"s_localIp\" type=\"text\" value=\"";
html+=espSettingsBox.localIp.toString();
html+="\" required/>\
<label><b>Шлюз</b></label>\
<input class=\"w3-input w3-border\" name=\"s_gateIp\" type=\"text\" value=\"";
html+=espSettingsBox.gateIp.toString();
html+="\" required/>\
<label><b>Подсеть</b></label>\
<input class=\"w3-input w3-border\" name=\"s_subnetIp\" type=\"text\" value=\"";
html+=espSettingsBox.subnetIp.toString();
html+="\" required/>\
<label><b>DNS1</b></label>\
<input class=\"w3-input w3-border\" name=\"s_dnsIp\" type=\"text\" value=\"";
html+=espSettingsBox.dnsIp.toString();
html+="\" required/>\
<label><b>DNS2</b></label>\
<input class=\"w3-input w3-border\" name=\"s_dnsIp2\" type=\"text\" value=\"";
html+=espSettingsBox.dnsIp2.toString();
html+="\" required/>\
<div id=\"msgContent\" class=\"w3-card-4\" style=\"margin-left:10px; margin-right:10px; margin-top:10px; margin-bottom:10px;\"></div>\
<p>\
<button class=\"w3-btn w3-teal\" onclick=\"submitForm('net','settForm','msgContent');\">Сохранить</button>\
</p>\
</p>\
</form>";

return html;
}

String getFormDeviceInfo(){
delay(1);
String result="<div class=\"w3-container w3-teal\"><h2>Информация</h2></div>\
<form class=\"w3-container\">\
<label><b>MAC адресс </b></label>\
<input class=\"w3-input w3-border\" type=\"text\" value=\"";
result+=WiFi.macAddress();;
result+="\" disabled/>\
<label><b>Версия прошивки </b></label>\
<input class=\"w3-input w3-border\" type=\"text\" value=\"";
result+=FIRMVARE_VERSION;
result+="\" disabled/>\
<label><b>Идентификатор чипа </b></label>\
<input class=\"w3-input w3-border\" type=\"text\" value=\"";
result+=ESP.getChipId();
result+="\" disabled/>\
<label><b>Частота Mhz </b></label>\
<input class=\"w3-input w3-border\" type=\"text\" value=\"";
result+=ESP.getCpuFreqMHz();
result+="\" disabled/>\
<label><b>Реальный обьем чипа байт </b></label>\
<input class=\"w3-input w3-border\" type=\"text\" value=\"";
result+=ESP.getFlashChipRealSize();
result+="\" disabled/>\
<label><b>Размер программы байт </b></label>\
<input class=\"w3-input w3-border\" type=\"text\" value=\"";
result+=ESP.getSketchSize();
result+="\" disabled/>\
<div id=\"msgContent\" class=\"w3-card-4\" style=\"margin-left:10px; margin-right:10px; margin-top:10px; margin-bottom:10px;\"></div>\
<p></p>\
</form>";
	return result;
}

String getFormDeviceSettings(){
delay(1);
String result="<div class=\"w3-container w3-teal\"><h2>Устройство</h2></div>\
<form class=\"w3-container\" id=\"settForm\" action=\"\" method=\"POST\">\
<label><b>Время</b></label>\
<input class=\"w3-input w3-border\" name=\"s_time\" type=\"time\" value=\"";
result+=getTimeStr();
result+="\" required/>\
<label><b>Дата</b></label>\
<input class=\"w3-input w3-border\" name=\"s_date\" type=\"date\" value=\"";
result+=getDateStrMinus();
result+="\" required/>\
<label><b>Имя устройства</b></label>\
<input class=\"w3-input w3-border\" name=\"s_DeviceId\" type=\"text\" value=\"";
result+=espSettingsBox.DeviceId;
result+="\"  required/>\
<label><b>Автопереключение дисплея</b></label>\
<input class=\"w3-input w3-border\" name=\"s_displayAutochange\" type=\"number\" value=\"";
result+=espSettingsBox.displayAutochange;
result+="\" min=\"15\" max=\"120\" step=\"1\"/>\
<label><b>Автообновление страницы с датчиками</b></label>\
<input class=\"w3-input w3-border\" name=\"s_pageAutochange\" type=\"number\" value=\"";
result+=espSettingsBox.pageAutochange;
result+="\" min=\"30\" step=\"10\"/>\
<label><b>Интервал обновления датчиков</b></label>\
<input class=\"w3-input w3-border\" name=\"s_refreshInterval\" type=\"number\" value=\"";
result+=espSettingsBox.refreshInterval;
result+="\" min=\"30\" max=\"360\" step=\"10\" />\
<p>\
<input class=\"w3-check\" name=\"s_displayAlvaysOn\" type=\"checkbox\"";
result+=espSettingsBox.displayAlvaysOn?" checked=\"checked\"":"";
result+="/>\
<label><b>Дисплей всегда включен</b></label></p>\
<p>\
<p>Настройки безопасности</p>\
<label><b>Админ</b></label>\
<input class=\"w3-input w3-border\" name=\"s_settingsUser\" type=\"text\" value=\"";
result+=espSettingsBox.settingsUser;
result+="\" required/>\
<label><b>Пароль админа</b></label>\
<input class=\"w3-input w3-border\" name=\"s_settingsPass\" type=\"password\" value=\"";
result+=espSettingsBox.settingsPass;
result+="\" required/>\
<label><b>Пользователь (пустой если открыто всем)</b></label>\
<input class=\"w3-input w3-border\" name=\"s_accessUser\" type=\"text\" value=\"";
result+=espSettingsBox.accessUser;
result+="\"/>\
<label><b>Пароль пользователя</b></label>\
<input class=\"w3-input w3-border\" name=\"s_accessPass\" type=\"text\" value=\"";
result+=espSettingsBox.accessPass;
result+="\"/>\
<div id=\"msgContent\" class=\"w3-card-4\" style=\"margin-left:10px; margin-right:10px; margin-top:10px; margin-bottom:10px;\"></div>\
<p>\
<button class=\"w3-btn w3-teal\" onclick=\"submitForm('device','settForm','msgContent');\">Сохранить</button>\
</p>\
</p>\
</form>";

return result;
}

String getFormAlarmSettins(){
delay(1);
String html="<div class=\"w3-container w3-teal\">\
<h2>Режим тревоги</h2>\
</div>\
<form class=\"w3-container\" id=\"settForm\" action=\"\" method=\"POST\">\
<p>\
<input class=\"w3-check\" name=\"s_alarmSendNotifAlertStart\" type=\"checkbox\"";
html+=espSettingsBox.alarmSendNotifAlertStart?" checked=\"checked\"":"";
html+=">\
<label><b>Отправить сообщение при начале режима тревоги</b></label></p>\
<p>\
<input class=\"w3-check\" name=\"s_alarmSendNotifAlertStop\" type=\"checkbox\"";
html+=espSettingsBox.alarmSendNotifAlertStop?" checked=\"checked\"":"";
html+=">\
<label><b>Отправить сообщение при прекращении режима тревоги</b></label></p>\
<p>\
<input class=\"w3-check\" name=\"s_alarmPlaySound\" type=\"checkbox\"";
html+=espSettingsBox.alarmPlaySound?" checked=\"checked\"":"";
html+=">\
<label><b>Играть сигнал тревоги на устройстве</b></label></p>\
<label><b>Интервал звукового сигнала тревоги</b></label>\
<input class=\"w3-input w3-border\" name=\"s_alamSoundInterval\" type=\"number\" value=\"";
html+=espSettingsBox.alamSoundInterval;
html+="\" required min=\"30\" max=\"1200\" step=\"10\"/>\
<p>Уровни тревоги</p>\
<table class=\"w3-table-all\" style=\"width: 95%;\">\
<thead>\
<tr class=\"w3-teal\">\
<th>Адресс</th>\
<th>Макс</th>\
<th>Мин</th>\
</tr>\
</thead>";
if(dhtActive){
	html+="<tr><td></td><td><b>DHT сенсор</b></td><td></td></tr>\
	<tr><td>";
	html+=sensorHelper.getValueByKey(dhtTemperatureAddress);
	html+="</td><td>";
	html+="<input class=\"w3-input w3-border\" name=\"s_maxDHtTemp\" type=\"number\" value=\"";
	html+=espSettingsBox.maxDHtTemp;
	html+="\" required min=\"-30\" max=\"255\" step=\"1\"/></td><td>";
	html+="<input class=\"w3-input w3-border\" name=\"s_maxDHtTemp\" type=\"number\" value=\"";
	html+=espSettingsBox.minDHtTemp;
	html+="\" required min=\"-30\" max=\"255\" step=\"1\"/></td><td>";
	html+="</tr>";
	html+="<td>";
	html+=sensorHelper.getValueByKey(dhtHumidityAddress);
	html+="</td><td>";
	html+="<input class=\"w3-input w3-border\" name=\"s_maxDHtHum\" type=\"number\" value=\"";
	html+=espSettingsBox.maxDHtHum;
	html+="\" required min=\"0\" max=\"101\" step=\"1\"/></td><td>";
	html+="<input class=\"w3-input w3-border\" name=\"s_minDHtHum\" type=\"number\" value=\"";
	html+=espSettingsBox.minDHtHum;
	html+="\" required min=\"0\" max=\"101\" step=\"1\"/></td><td>";
	html+="</tr>";
}
html+="<tr><td></td><td><b>DS сенсор</b></td><td></td></tr>";
for(uint8_t busI=0;busI<sensorBusCount;busI++){
	uint8_t c=dallasTemperatures[busI].getDeviceCount();

	if(c>0){
		html+="<tr><td></td><td><b>";
		html+=sensorHelper.getValueByKey(sensorPinDescr[busI]);
		html+="</b></td><td></td></tr>";

		for(uint8_t j=0;j<c;j++){
			String devAddrStr="";
			int ht;
			int lt;

			DeviceAddress deviceAddressD;
			dallasTemperatures[busI].getAddress(deviceAddressD, j);

			for(uint8_t k=0;k<sizeof(deviceAddressD);k++){
				devAddrStr+=String(deviceAddressD[k]);
			}

			ht=dallasTemperatures[busI].getHighAlarmTemp(deviceAddressD);
			lt=dallasTemperatures[busI].getLowAlarmTemp(deviceAddressD);

			html+="<tr><td>";
			html+=sensorHelper.getValueByKey(devAddrStr);
			html+="</td><td>";
			html+="<input class=\"w3-input w3-border\" name=\"ht_";
			html+=busI;
			html+="_";
			html+=j;
			html+="\" type=\"text\" value=\"";
			html+=ht;
			html+="\" /></td><td>";
			html+="<input class=\"w3-input w3-border\" name=\"lt_";
			html+=busI;
			html+="_";
			html+=j;
			html+="\" type=\"text\" value=\"";
			html+=lt;
			html+="\" /></td></tr>";
		}
	}

}
html+="</table>";

html+="<div id=\"msgContent\" class=\"w3-card-4\" style=\"margin-left:10px; margin-right:10px; margin-top:10px; margin-bottom:10px;\"></div>\
<p>\
<button class=\"w3-btn w3-teal\" onclick=\"submitForm('alarm','settForm','msgContent');\">Сохранить</button>\
</p>\
</p>\
</form>";

return html;
}

boolean sendDataToThingSpeak(){

	uint8_t sendCount=0;

	if(thingSpeakTrigger.checkTrigger() && !sendToThingSpeakTriggered){
		//Serial.println("------Thing Speak send triggered");
		sendToThingSpeakTriggered=true;
	}

	if(espSettingsBox.thSkWKey.length()==0){
		sendToThingSpeakTriggered=false;
		//Serial.println("Send to Thing Speak impossible");
	}

	while(sendToThingSpeakTriggered){
		if(sendToThingSpeakTriggered){
			String url="http://api.thingspeak.com/update?api_key="+espSettingsBox.thSkWKey;
			boolean needToSend=false;

			if(dhtActive){
				int8_t tsId=sensorHelper.getKeyValueByKey(dhtHumidityAddress).getKind();

				if(tsId>0){
					url+="&field"+String(tsId)+"="+dht.readHumidity(false);
					needToSend=true;
				}

				tsId=sensorHelper.getKeyValueByKey(dhtTemperatureAddress).getKind();

				if(tsId>0){
					url+="&field"+String(tsId)+"="+dht.readTemperature(false, false);
					needToSend=true;
				}
			}

			for(uint8_t i=0;i<sensorBusCount;i++){
				uint8_t c=dallasTemperatures[i].getDeviceCount();

				if(c>0){
					for(uint8_t j=0;j<c;j++){
						String result="";

						DeviceAddress deviceAddressD;
						boolean found=dallasTemperatures[i].getAddress(deviceAddressD, j);

						for(uint8_t k=0;k<sizeof(deviceAddressD);k++){
							result+=String(deviceAddressD[k]);
						}

						if(found){
							int8_t tsId=sensorHelper.getKeyValueByKey(result).getKind();

							if(tsId>0){
								url+="&field"+String(tsId)+"="+dallasTemperatures[i].getTempCByIndex(j);
								needToSend=true;
							}
						}
					}
				}
			}
			if(needToSend){
				Serial.println("StartSend to ThingSpeak");
				Serial.print("Send toTS url=");
				Serial.println(url);
				HTTPClient http;  //Declare an object of class HTTPClient

				http.begin(url);
				Serial.println("send get");
				delay(1);
				int httpCode = http.GET();                                                                  //Send the request
				delay(1);
				if (httpCode > 0) { //Check the returning code

				  String payload = http.getString();
				  Serial.println("-----response from thingSpeak----");
				  Serial.println(payload);
				  Serial.println("---------------------------------");
				  sendToThingSpeakTriggered=false;
				  thingSpeakTrigger.saveTime();
				  sendCount=0;

				}else{
					Serial.print("Send failed status");
					Serial.println(httpCode);
					Serial.println(http.errorToString(httpCode));
					sendCount++;
				}
			}else{
				Serial.println("Nothing to send to ThingSpeak");
				thingSpeakTrigger.saveTime();
				sendToThingSpeakTriggered=false;
			}
		}

		if(sendCount>10){
			sendToThingSpeakTriggered=false;
			Serial.println("----Send TO Thing Speak failed");
		}
	}

	return sendToThingSpeakTriggered;
}

void handleJson(){
	Serial.println("Handle Json");
	refreshSensors(true);

	String result="{\"device\":\"";
	result+=String(espSettingsBox.DeviceId);
	result+="\",\"date\":\"";
	result+=getDateStr();
	result+="\",\"time\": \"";
	result+=getTimeStr();
	result+="\",\"count\":";
	result+=String(totalDetectedSensors);
	result+="\",\"thSId\":";
	result+=String(espSettingsBox.thSkChId);
	result+=",\"sensors\": [";

	uint8_t id=0;

	if(dhtActive){
		result+=sensorHelper.sensorToJson(id, dhtName, dhtHumidityAddress, dht.readHumidity(false), "0");
		result+=",";
		result+=sensorHelper.sensorToJson(id, dhtName, dhtTemperatureAddress, dht.readTemperature(false, false), "0");

		if(totalDetectedSensors>1){
			result+=",";
		}
	}

	for(unsigned int i=0;i<sensorBusCount;i++){
		int length=dallasTemperatures[i].getDeviceCount();

		for(int j=0;j<length;j++){

			DeviceAddress termAddr;
			dallasTemperatures[i].getAddress(termAddr, j);

			String termAddrStr="";

			for(uint8_t k=0;k<sizeof(termAddr);k++){
				termAddrStr+=termAddr[k];
			}

			result+=sensorHelper.sensorToJson(id, sensorPinDescr[j], termAddrStr, dallasTemperatures[i].getTempCByIndex(j), "0");

			if(j!=length-1){
				result+=",";
			}
		}

		if(i!=sensorBusCount-1){
			result+=",";
		}
	}

	result+="]";
	result+="}";

	server.send ( 200, "text/html", result );
}

void handleNotFound() {
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
}

void scanI2C(){
	 Wire.begin();

	 byte error, address;
	  int nDevices;

	  Serial.println("Scanning I2C...");

	  nDevices = 0;
	  for(address = 1; address < 127; address++ )
	  {

	    Wire.beginTransmission(address);
	    error = Wire.endTransmission();

	    if (error == 0)
	    {
	      Serial.print("I2C device found at address 0x");
	      if (address<16)
	        Serial.print("0");
	      Serial.print(address,HEX);
	      Serial.println("  !");

	      nDevices++;
	    }
	    else if (error==4)
	    {
	      Serial.print("Unknow error at address 0x");
	      if (address<16)
	        Serial.print("0");
	      Serial.println(address,HEX);
	    }
	  }
	  if (nDevices == 0)
	    Serial.println("No I2C devices found\n");
	  else
	    Serial.println("done\n");

	  delay(5000);
}


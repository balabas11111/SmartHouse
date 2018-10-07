#include "Arduino.h"
#include "WiFiHelper.h"

WiFiHelper::WiFiHelper(EspSettingsBox *_settingsBox,std::function<void(void)> _clearDisplayFunction,std::function<void(String,int,int)> _displayLineFunction, std::function<void(void)> _ledOnFunction,std::function<void(void)> _ledOffFunction,std::function<void(ESP8266WebServer)> _setupsServerUrlsFunction){
	espSettingsBox=_settingsBox;
	clearDisplayFunction=_clearDisplayFunction;
	displayLineFunction=_displayLineFunction;
	setupsServerUrlsFunction=_setupsServerUrlsFunction;
	ledOnFunction=_ledOnFunction;
	ledOffFunction=_ledOffFunction;

	setup();
}

void WiFiHelper::setup(){
	Serial.println("CHecking is access point");
	clearDisplay();

		if(espSettingsBox->isAccesPoint){
			setupAsAccessPoint();
		}else{
			setupAsClient();
		}

		server=ESP8266WebServer(espSettingsBox->serverPort);

		Serial.println ( "" );

		setupServersUrls();

		server.begin();
		 //MDNS.addService("http", "tcp", 80);
		 //Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", DNS_NAME);
		Serial.println ( "HTTP server started" );

		Serial.print("Status=");
		Serial.println(WiFi.status());
		Serial.print(" Mac=");
		Serial.println(WiFi.macAddress());
}

void WiFiHelper::loop(){
	server.handleClient();
}

void WiFiHelper::connectIfNotConnected(){
	if(WiFi.getMode()==WIFI_STA && !isConnected()){

		unsigned long start=millis();
		ledOn();

		while(!isConnected()){
			connect();
			if(!isConnected()){
				delay(5000);
			}

			if(start+60000<millis()){
				Serial.println("can not connect to vifi. Restart");
				displayLine("ERROR WiFi",0,0);
				delay(600000);
			}
		}
	}
}

void WiFiHelper::connect(){

	//WiFi.begin();
	ledOn();
	delay ( 500 );
	Serial.print ( "status=" );
	Serial.print ( WiFi.status() );
	ledOff();
}

void WiFiHelper::setupAsAccessPoint(){
	WiFi.mode(WIFI_AP);
	displayLine("Start",3,0);
	displayLine("       AccessPoint",5,0);
	Serial.println("STarting as access point");
	/*
	 WiFi.softAPConfig(espSettingsBox.apIp,espSettingsBox.gateIp,
			espSettingsBox.subnetIp);
			*/
	//WiFi.softAP(const_cast<char*>(espSettingsBox.ssidAP.c_str()),const_cast<char*>(espSettingsBox.passwordAP.c_str()));
	WiFi.softAP(const_cast<char*>(espSettingsBox->ssidAP.c_str()));
	//String apName="ESP "+ESP.getChipId();
	//WiFi.softAP(espSettingsBox.ssidAP);

	Serial.println ( "-----------" );
	Serial.print ( "SoftAP " );
	Serial.println ( espSettingsBox->ssidAP);
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.softAPIP() );
	Serial.println ( "-----------" );

	Serial.print("ViFi status");
	Serial.println(WiFi.status());
	Serial.println ( "-----------" );
}

void WiFiHelper::setupAsClient(){
	displayLine("connect to",2,0);
	displayLine(espSettingsBox->ssid,5,0);
	Serial.print("espSettingsBox.ssid=");
	Serial.println(espSettingsBox->ssid);
	Serial.print("espSettingsBox.password=");
	Serial.println(espSettingsBox->password);

	WiFi.mode(WIFI_STA);
	WiFi.config(espSettingsBox->localIp,espSettingsBox->gateIp,
			espSettingsBox->subnetIp,espSettingsBox->dnsIp,espSettingsBox->dnsIp2);
	WiFi.begin ( const_cast<char*>(espSettingsBox->ssid.c_str()),
			const_cast<char*>(espSettingsBox->password.c_str()) );

	// Wait for connection

	ledOn();

	connectIfNotConnected();

	ledOff();

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( WiFi.SSID() );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );
}

boolean WiFiHelper::isConnected(){
	return getWiFi().status()==WL_CONNECTED;
}

WiFiClient WiFiHelper::getClient(){
	return client;
}

Client& WiFiHelper::getClientAs(){
	return client;
}

ESP8266WebServer WiFiHelper::getServer(){
	return server;
}

ESP8266WiFiClass WiFiHelper::getWiFi(){
	return WiFi;
}

void WiFiHelper::clearDisplay(){
	if(clearDisplayFunction!=NULL)
		clearDisplayFunction();
}

void WiFiHelper::displayLine(String message,uint8_t row,uint8_t column){
	if(displayLineFunction!=NULL)
		displayLineFunction(message,row,column);
}

void WiFiHelper::ledOn(){
	if(ledOnFunction!=NULL)
		ledOnFunction();
}

void WiFiHelper::ledOff(){
	if(ledOffFunction!=NULL)
		ledOffFunction();
}

void WiFiHelper::setupServersUrls(){
	if(setupsServerUrlsFunction!=NULL)
		setupsServerUrlsFunction(server);
}

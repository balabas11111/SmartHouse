/*
 * WiFiManagerAsync.cpp
 *
 *  Created on: Feb 25, 2019
 *      Author: Vitaliy_Vlasov
 */

#include "WiFiManagerAsync.h"

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
	//return updater!=nullptr;
	return false;
#endif
}

bool WiFiManagerAsync::startAsSta() {
	WiFi.disconnect(0);
	WiFi.persistent(false);
	WiFi.mode(WIFI_STA);

	registerStaEvents();

	bool confOk=false;
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
	int startOk = WiFi.begin (conf->ssid(),conf->password());

	Serial.print(FPSTR(" ssid="));
	Serial.print(conf->ssid());
	Serial.print(FPSTR(" pass="));
	Serial.print(conf->password());

	Serial.print(FPSTR(" status="));
	Serial.println(WiFi.status());

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
	int count=0;

	while(!isConnected() ){
		if(count==0 || count>=20 || oldWiFistatus!=WiFi.status()){
			WiFiUtils::printWiFiDetails();
			count=1;
		}else{
			//Serial.print(FPSTR("."));
			delay(500);
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

bool WiFiManagerAsync::checkAuth(bool userLogin) {
	return true;
}

void WiFiManagerAsync::startUpdater() {
	/*
#ifdef ESP8266
	Serial.println(FPSTR("Init OTA update"));
	if(updater!=nullptr){
		updater->setup(this->server,conf->adminLogin(),conf->adminPassword());
	}else{
		Serial.println(FPSTR("OTA Update is disabled on this device"));
	}
#endif*/
}

void WiFiManagerAsync::startServer() {
	deployDefaultUrls();
	deployStaticFiles();
	deployTemplates();
	deployServices();
	deployEventSource();

	server->onNotFound([this](AsyncWebServerRequest *request){ notFound(request);});
	server->begin();
}

void WiFiManagerAsync::deployDefaultUrls() {
	Serial.println(FPSTR(" ------------Deploy default Urls-----------"));

	server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request){onRoot(request);});

	server->on("/info", HTTP_GET, [this](AsyncWebServerRequest *request){onInfo(request);});
	server->on("/dir", HTTP_GET, [this](AsyncWebServerRequest *request){onDir(request);});
	server->on("/cat", HTTP_GET, [this](AsyncWebServerRequest *request){onCat(request);});

	server->on("/files", HTTP_DELETE, [this](AsyncWebServerRequest *request){onDelete(request);});

	server->on("/files", HTTP_POST,
			[this](AsyncWebServerRequest *request){
				request->send(200);
			},
			[this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
				onUpload(request, filename, index, data, len, final);
			}
			);
}

void WiFiManagerAsync::deployStaticFiles() {
	Serial.println(FPSTR(" ------------Deploy static Files-----------"));
}

void WiFiManagerAsync::deployTemplates() {
	Serial.println(FPSTR(" ------------Deploy templates-----------"));
	std::function<String(const String&)> AwsTemplateProcessor=[this](const String& var){return processor(var);};
	server->serveStatic("/", SPIFFS, "/deploy/").setTemplateProcessor(AwsTemplateProcessor);
	Serial.println(FPSTR(" deployed"));
}

void WiFiManagerAsync::deployServices() {
	Serial.println(FPSTR(" ------------Deploy services-----------"));
}

void WiFiManagerAsync::deployEventSource() {
	Serial.println(FPSTR(" ------------Deploy eventSources-----------"));
	  events->onConnect([](AsyncEventSourceClient *client){
	    if(client->lastId()){
	      Serial.printf("Client reconnected! Last message ID that it gat is: %u\n", client->lastId());
	    }
	    client->send("hello!",NULL,millis(),1000);
	  });
	  events->setAuthentication(conf->userLogin(), conf->userPassword());
	  server->addHandler(events);

	  dao->setEventSender(this);
}

void WiFiManagerAsync::sendAsEventSource(const char* event,const char* msg) {
	events->send(msg,event,millis());
}

void WiFiManagerAsync::onRoot(AsyncWebServerRequest *request){
	request->send(200,FPSTR(CONTENT_TYPE_TEXT_HTML));
}

void WiFiManagerAsync::onInfo(AsyncWebServerRequest *request){
	request->send(200, FPSTR(CONTENT_TYPE_TEXT_HTML));
}

void WiFiManagerAsync::onDir(AsyncWebServerRequest *request){
	request->send(200, FPSTR(CONTENT_TYPE_TEXT_PLAIN));
}

void WiFiManagerAsync::onCat(AsyncWebServerRequest *request){
	request->send(200, FPSTR(CONTENT_TYPE_TEXT_PLAIN));
}

void WiFiManagerAsync::onDelete(AsyncWebServerRequest *request){
	  if(request->args() == 0) return request->send(500, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "BAD ARGS");
	  size_t i = 0;
	  const String& path = request->arg(i);
	  Serial.print(FPSTR("handleFileDelete: "));
	  Serial.println(path);

	  if(path == "/")
		request->send(500, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "BAD PATH");
	  if(!SPIFFS.exists(path))
		  request->send(404, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "FileNotFound");
	  SPIFFS.remove(path);
	  request->send(200, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "");
}

void  WiFiManagerAsync::onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
	Serial.print(FPSTR("Upload file url="));
	Serial.println(request->url());

	if(!index){
		Serial.printf("UploadStart: %s\n", filename.c_str());
	}

	FileUtils::saveFile(filename.c_str(), data, len);

	if(final){
		Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index+len);
	}
}

void WiFiManagerAsync::notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

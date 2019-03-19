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

	server->on(URL_ROOT, HTTP_GET, [this](AsyncWebServerRequest *request){onRoot(request);});

	server->on(URL_INFO, HTTP_GET, [this](AsyncWebServerRequest *request){onInfo(request);});
	server->on(URL_DIR, HTTP_GET, [this](AsyncWebServerRequest *request){onDir(request);});
	server->on(URL_CAT, HTTP_GET, [this](AsyncWebServerRequest *request){onCat(request);});
	server->on(URL_INFO, HTTP_POST, [this](AsyncWebServerRequest *request){
		Serial.println(FPSTR("post info"));

		AsyncResponseStream *response = request->beginResponseStream("application/json");

		DynamicJsonBuffer buf;
		JsonObject &root = buf.createObject();
		JsonObject& obj=root.createNestedObject("response");

		obj.set("status", "accepted");

		obj.printTo(*response);
		response->setCode(200);
		request->send(response);
	});


	jsonPostHandler = new AsyncCallbackJsonWebHandler(URL_INFO_POST, [this](AsyncWebServerRequest *request, JsonVariant &json) {
		Serial.println(FPSTR("post Json"));
		onEntityPost(request,json);
		request->send(200,CONTENT_TYPE_JSON_UTF8,"{accepted}");
	});
	server->addHandler(jsonPostHandler);

	server->on(URL_FILES, HTTP_DELETE, [this](AsyncWebServerRequest *request){onDelete(request);});
	server->on(URL_FILES, HTTP_POST,
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
	std::function<String(const String&)> AwsTemplateProcessor=[this](const String& var){return processor(var);};
	server
	    ->serveStatic("/", SPIFFS, PATH_MODEL_DATA_JSON_FILE_PATH)
	    .setDefaultFile(PATH_MODEL_DATA_JSON_FILE_NAME)
		.setTemplateProcessor(AwsTemplateProcessor)
	    .setAuthentication(conf->adminLogin(), conf->adminPassword());

	server->on("/index.htm", HTTP_GET, [this](AsyncWebServerRequest *request){onFileRead(request);});
}

void WiFiManagerAsync::deployTemplates() {
	Serial.println(FPSTR(" ------------Deploy templates-----------"));
	std::function<String(const String&)> AwsTemplateProcessor=[this](const String& var){return processor(var);};
	server->serveStatic("/", SPIFFS, PATH_MODEL_DATA_JSON_BY_GROUP).setTemplateProcessor(AwsTemplateProcessor).setAuthentication(conf->userLogin(), conf->userPassword());
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
	events->send(msg,NULL,millis(),1000);
}

void WiFiManagerAsync::onRoot(AsyncWebServerRequest *request){
	onInfo(request);
}

void WiFiManagerAsync::onInfo(AsyncWebServerRequest *request){

	AsyncResponseStream *response = request->beginResponseStream("application/json");

	JsonObject &root = dao->getEntityRoot();

	root.printTo(*response);
	response->setCode(200);
	request->send(response);

	request->send(200, FPSTR(CONTENT_TYPE_TEXT_HTML));
}

void WiFiManagerAsync::onDir(AsyncWebServerRequest *request){
	AsyncJsonResponse * response = new AsyncJsonResponse();
	response->addHeader("Server","ESP Async Web Server");
	JsonObject& root = response->getRoot();

	FileUtils::dirFiles(root.createNestedObject("files"));

	response->setLength();
	request->send(response);
}

void WiFiManagerAsync::onCat(AsyncWebServerRequest *request){
	String fileName=request->arg("file");
	request->send(SPIFFS, fileName);
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
	if(!index){
		Serial.printf("UploadStart: %s url\n", filename.c_str());

		Serial.print(FPSTR(" url="));
		Serial.println(request->url());

		fsUploadFile = SPIFFS.open(filename, "w");
	}

	if(fsUploadFile){
		for(size_t i=0; i<len; i++){
			fsUploadFile.write(data[i]);
		}
	}

	if(final){
		if(fsUploadFile)
			fsUploadFile.close();

		Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index+len);
		Serial.print(FPSTR("ex and has size = "));
		Serial.println(FileUtils::existsAndHasSize(filename));
	}
}

void WiFiManagerAsync::notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void WiFiManagerAsync::onFileRead(AsyncWebServerRequest* request) {
	if(!handleFileGzRead(MESSAGE_WIFIHELPER_INDEX_HTML_PAGE,request)){
			request->send(404, CONTENT_TYPE_TEXT_PLAIN, MESSAGE_WIFIHELPER_HTTP_STATUS_FILE_NOT_FOUND);
	};
}

bool WiFiManagerAsync::handleFileRead(String path,AsyncWebServerRequest* request){
	Serial.println("handleFileRead: " + path);
	  String contentType = getContentType(path,request);
		  if(SPIFFS.exists(path)){
			File file = SPIFFS.open(path, "r");
			request->send(file, path, contentType);
			file.close();
			return true;
		  }
	  return false;
}

bool WiFiManagerAsync::handleFileGzRead(String path,AsyncWebServerRequest* request){
	  Serial.println("handleFileGzRead: " + path);
	  if(path.endsWith("/")) path += "index.htm";
	  String contentType = getContentType(path,request);
	  String pathWithGz = path + ".gz";
	  Serial.println(pathWithGz);
	  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
		if(SPIFFS.exists(pathWithGz)){
		  //path += ".gz";
		 // contentType = getContentType(pathWithGz,request);
		}

		Serial.print(FPSTR("path ="));
		Serial.print(path);
		Serial.print(FPSTR(" pathWithGz ="));
		Serial.print(pathWithGz);
		Serial.print(FPSTR(" contentType ="));
		Serial.println(contentType);
		request->send(SPIFFS, path, contentType,false);
		return true;
	  }
	  return false;
}

void WiFiManagerAsync::onEntityPost(AsyncWebServerRequest *request, JsonVariant &json) {
	Serial.println(FPSTR("Json posted"));
	json.printTo(Serial);
	Serial.println(FPSTR("--------------------"));
	request->send(200);
}

String  WiFiManagerAsync::getContentType(String filename,AsyncWebServerRequest* request){
	  if(request->hasArg("download")) return FPSTR(CONTENT_TYPE_APPLICATION_OCTED_STREAM);
	  else if(filename.endsWith(".htm")) return FPSTR(CONTENT_TYPE_TEXT_HTML);
	  else if(filename.endsWith(".html")) return FPSTR(CONTENT_TYPE_TEXT_HTML);
	  else if(filename.endsWith(".css")) return FPSTR(CONTENT_TYPE_TEXT_CSS);
	  else if(filename.endsWith(".js")) return FPSTR(CONTENT_TYPE_APPLICATION_JAVASCRIPT);
	  else if(filename.endsWith(".png")) return FPSTR(CONTENT_TYPE_IMAGE_PNG);
	  else if(filename.endsWith(".gif")) return FPSTR(CONTENT_TYPE_IMAGE_GIF);
	  else if(filename.endsWith(".jpg")) return FPSTR(CONTENT_TYPE_IMAGE_JPEG);
	  else if(filename.endsWith(".ico")) return FPSTR(CONTENT_TYPE_IMAGE_XICON);
	  else if(filename.endsWith(".xml")) return FPSTR(CONTENT_TYPE_TEXT_XML);
	  else if(filename.endsWith(".pdf")) return FPSTR(CONTENT_TYPE_APPLICATION_XPDF);
	  else if(filename.endsWith(".zip")) return FPSTR(CONTENT_TYPE_APPLICATION_XZIP);
	  else if(filename.endsWith(".gz")) return FPSTR(CONTENT_TYPE_APPLICATION_XGZIP);
	  return FPSTR(CONTENT_TYPE_TEXT_PLAIN);
	}

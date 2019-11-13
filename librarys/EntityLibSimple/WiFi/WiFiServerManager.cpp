/*
 * WiFiServerManager.cpp
 *
 *  Created on: Aug 22, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <WiFi/WiFiServerManager.h>
#include <ArduinoJson.h>

WiFiServerManager::WiFiServerManager(EntityManager* manager, SettingsStorage* conf, int port) {
	this->server = new ESP8266WebServer(port);
	this->updateServer = new ESP8266HTTPUpdateServer(true);
	this->manager = manager;
	this->conf = conf;
}

void WiFiServerManager::begin() {
	this->server->begin();
	this->updateServer->setup(this->server, conf->adminLogin(), conf->adminPassword());

	deployDefaultUrls();
	deployStaticFiles();
}

void WiFiServerManager::deployDefaultUrls() {
	Serial.print(FPSTR("Deploy default Urls"));

	server->on(URL_ROOT, HTTP_GET, [this]() {onRoot();});
	server->on(URL_DIR, HTTP_GET, [this]() {onDir();});
	server->on(URL_INFO, HTTP_GET, [this]() {onInfo();});


	server->on(URL_DATA, HTTP_GET, [this]() {onEntityRequest(REQUEST_GET);});
	server->on(URL_DATA, HTTP_POST, [this]() {onEntityRequest(REQUEST_POST);});

	server->on(URL_FILES, HTTP_DELETE, [this]() {onFileDelete();});
	server->on(URL_FILES, HTTP_POST, [this]() {onFileUpload();});

	Serial.println(FPSTR("...done"));
}

void WiFiServerManager::deployStaticFiles() {
	server->serveStatic(URL_INDEXHTM, SPIFFS, URL_INDEXHTM_GZ);
	server->serveStatic(URL_SETUPHTM, SPIFFS, URL_SETUPHTM_GZ);

	if(conf->hasAdminPassword()){
		server->on(URL_SETUP,HTTP_GET, [this](){
			if(!server->authenticate(conf->adminLogin(), conf->adminPassword()))
			  return server->requestAuthentication(DIGEST_AUTH,
					  AUTH_REALM_ADMIN_MESSAGE, AUTH_FAILED_MESSAGE);
			onFileRead(URL_SETUPHTM_GZ);
		  });
	} else {
		server->on(URL_SETUP, HTTP_GET, [this](){onFileRead(URL_SETUPHTM_GZ);});
	}

	if(conf->hasUserPassword()){
		server->on(URL_SETUP,HTTP_GET, [this](){
			if(!server->authenticate(conf->userLogin(), conf->userPassword()))
			  return server->requestAuthentication(DIGEST_AUTH,
					  AUTH_REALM_USER_MESSAGE, AUTH_FAILED_MESSAGE);
			onFileRead(URL_INDEXHTM_GZ);
		  });
	} else {
		server->on(URL_SETUP, HTTP_GET, [this](){onFileRead(URL_INDEXHTM_GZ);});
	}
}

void WiFiServerManager::onRoot() {
	server->sendHeader(RESPONSE_KEY_Server,getServerName());
	server->sendHeader(HEADER_LOCATION, URL_INDEX, true);
	server->send ( 302, CONTENT_TYPE_TEXT_PLAIN, "");
}

void WiFiServerManager::onDir() {
	EntityJsonRequestResponse * req = new EntityJsonRequestResponse();

	FileUtils::dirFiles(req->getResponse().createNestedObject(JSONKEY_files));

	String response;
	req->printResponseTo(response);

	delete req;

	server->sendHeader(RESPONSE_KEY_Server,getServerName());
	server->send(200, CONTENT_TYPE_TEXT_JSON_UTF8, response);
}

void WiFiServerManager::onInfo() {
	EntityJsonRequestResponse * req = new EntityJsonRequestResponse();

	conf->addDeviceInfoToJson(req->getResponse());
	JsonObjectUtil::setField(req->getResponse(), JSONKEY_heap, ESP.getFreeHeap());

	String response;
	req->printResponseTo(response);

	delete req;

	server->sendHeader(RESPONSE_KEY_Server,getServerName());
	server->send(200, CONTENT_TYPE_TEXT_JSON_UTF8, response);

}

void WiFiServerManager::onFileDelete() {
	if(server->args() == 0) {
		server->send(500, CONTENT_TYPE_TEXT_PLAIN, RESPONSE_MSG_badArgs);
	}
	  size_t i = 0;
	  const String& path = server->arg(i);
	  Serial.print(FPSTR("handleFileDelete: "));
	  Serial.println(path);

	  if(path == "/")
		server->send(500, FPSTR(CONTENT_TYPE_TEXT_PLAIN), RESPONSE_MSG_badPath);
	  if(!SPIFFS.exists(path))
		  server->send(404, FPSTR(CONTENT_TYPE_TEXT_PLAIN), RESPONSE_MSG_fileNotFound);
	  SPIFFS.remove(path);
	  server->send(200, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "");
}

void WiFiServerManager::onFileUpload() {
	  HTTPUpload& upload = server->upload();
	  if(upload.status == UPLOAD_FILE_START){
	    String filename = upload.filename;
	    if(!filename.startsWith("/")) filename = "/"+filename;
	    Serial.print(FPSTR("handleFileUpload Name: "));
	    Serial.println(filename);
	    fsUploadFile = SPIFFS.open(filename, "w");
	    filename = String();
	  } else if(upload.status == UPLOAD_FILE_WRITE){
	    //DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
	    if(fsUploadFile)
	      fsUploadFile.write(upload.buf, upload.currentSize);
	  } else if(upload.status == UPLOAD_FILE_END){
	    if(fsUploadFile)
	      fsUploadFile.close();
	    Serial.print(FPSTR("handleFileUpload Size: "));
	    Serial.println(FPSTR(upload.totalSize));
	  }
}

const char* WiFiServerManager::getServerName() {
	if(conf == nullptr){
		return RESPONSE_MSG_ServerName;
	}

	return conf->deviceId();
}

void WiFiServerManager::onFileRead(const char* fileName) {
	String path = fileName;
	Serial.print(FPSTR("handle File Read: "));
	Serial.println(fileName);
	if(path.endsWith("/")) path += "index.htm";
	String contentType = getContentType(path);
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
	Serial.print(contentType);

	File file = SPIFFS.open(path, "r");
	size_t sent = server->streamFile(file, contentType);
	Serial.print(FPSTR(" sent ="));
	Serial.println(sent);
	file.close();
	return;
	}
	server->send(404, CONTENT_TYPE_TEXT_PLAIN, RESPONSE_MSG_fileNotFound);
}

const char* WiFiServerManager::getContentType(String& path) {
	// if(request->hasArg("download")) return FPSTR(CONTENT_TYPE_APPLICATION_OCTED_STREAM);
	  if(path.endsWith(".htm")) return (CONTENT_TYPE_TEXT_HTML);
	  else if(path.endsWith(".html")) return (CONTENT_TYPE_TEXT_HTML);
	  else if(path.endsWith(".css")) return (CONTENT_TYPE_TEXT_CSS);
	  else if(path.endsWith(".js")) return (CONTENT_TYPE_APPLICATION_JAVASCRIPT);
	  else if(path.endsWith(".png")) return (CONTENT_TYPE_IMAGE_PNG);
	  else if(path.endsWith(".gif")) return (CONTENT_TYPE_IMAGE_GIF);
	  else if(path.endsWith(".jpg")) return (CONTENT_TYPE_IMAGE_JPEG);
	  else if(path.endsWith(".ico")) return (CONTENT_TYPE_IMAGE_XICON);
	  else if(path.endsWith(".xml")) return (CONTENT_TYPE_TEXT_XML);
	  else if(path.endsWith(".pdf")) return (CONTENT_TYPE_APPLICATION_XPDF);
	  else if(path.endsWith(".zip")) return (CONTENT_TYPE_APPLICATION_XZIP);
	  else if(path.endsWith(".gz")) return (CONTENT_TYPE_APPLICATION_XGZIP);
	  return (CONTENT_TYPE_TEXT_PLAIN);
}

bool WiFiServerManager::authenticateRequest(const char* method){
	UNUSED(method);
#ifdef REQUEST_SECURITY_DISABLED
	return true;
#else
	bool result = true;

	if(strcmp(method, REQUEST_GET)==0){
		result = isAuthenticatedRequest(conf->userLogin(), conf->userPassword());
	} else
	if(strcmp(method, REQUEST_POST)==0){
#ifndef SETTINGS_SERVER_CONNECTION_DISABLED
		if(server->hasHeader(HEADER_AUTHORIZATION)){
			result = server->header(HEADER_AUTHORIZATION).equals(conf->getServerAuthorization());
			if(!result){
				Serial.print(FPSTR("Failed auth="));
				Serial.print(server->header(HEADER_AUTHORIZATION));
				Serial.print(FPSTR("; exp="));
				Serial.println(conf->getServerAuthorization());
			}
		}
		if(!result){
			result = isAuthenticatedRequest(conf->adminLogin(), conf->adminPassword());
		}
#else
		result = isAuthenticatedRequest(conf->adminLogin(), conf->adminPassword());
#endif
	}
	Serial.print(FPSTR(" Auth = "));
	Serial.print(result);

	return result;
#endif
}

bool WiFiServerManager::isShrReceived() {
	return this->shrReceived;
}

bool WiFiServerManager::isAuthenticatedRequest(char* login, char* password){
	if(strcmp(login,"")!=0 && strcmp(password,"")!=0){
		return server->authenticate(login, password);
	}
	return true;
}

void WiFiServerManager::onEntityRequest(const char* method) {
/*
	Serial.print(FPSTR("Headers "));
	Serial.print(server->headers());
	Serial.print(FPSTR(" "));

	for(int i=0; i < server->headers(); i++) {
		Serial.print(server->header(i));
		Serial.print(FPSTR("; "));
	}
*/
	if(server->hasArg(HEADER_SREQUEST)) {
		this->shrReceived = true;
		this->lastSrequestTime = millis();
		Serial.print(FPSTR("------SHr "));
	} else {
		Serial.print(FPSTR("------ER "));
	}

	if(!authenticateRequest(method)){
		Serial.println(FPSTR("request authentication"));
		return server->requestAuthentication();
	}

	unsigned long start = millis();
	//Serial.println(FPSTR("Create request"));
	EntityJsonRequestResponse* req = new EntityJsonRequestResponse();
	JsonObject& params = req->getRequest();

	//Serial.println(FPSTR("Fill params"));
	if(server->hasArg(BODY)){
		req->putRequestJsonParam(server->arg(BODY), BODY);
	}

	for(int i = 0;i<server->args();i++){
		if(server->argName(i)!=BODY){
			params.set(server->argName(i), server->arg(i));
		}
	}
	//Serial.println(FPSTR("Execute method"));
	manager->executeMethod(req, method);

	String response;
	req->printResponseTo(response);

	delete req;

	server->sendHeader(RESPONSE_KEY_Server,getServerName());
	server->send(200, CONTENT_TYPE_TEXT_JSON_UTF8, response);

	DeviceUtils::printTimeHeap(start);

	Serial.println(FPSTR("==="));
}

void WiFiServerManager::loop() {
	this->server->handleClient();
}

unsigned long WiFiServerManager::getLastSrequestTime() {
	return this->lastSrequestTime;
}

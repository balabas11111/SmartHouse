/*
 * ServerConnectionManager.cpp
 *
 *  Created on: 28 сент. 2019 г.
 *      Author: Vitaliy
 */

#include <serve/ServerConnectionManager.h>

ServerConnectionManager::ServerConnectionManager(WiFiSettingsBox* conf) {
	this->conf = conf;
}

void ServerConnectionManager::triggerServerPing(bool trigger) {
	this->triggeredServerPing = trigger;
}

void ServerConnectionManager::triggerRegisterOnServer(bool trigger) {
	Serial.println(FPSTR("Trigger registration"));
	this->triggeredServerRegister = trigger;
}

void ServerConnectionManager::triggerDataChanged(bool trigger) {
	this->triggeredDataChange = trigger;
}

void ServerConnectionManager::init(){
	if(strcmp(conf->smartServerAddr(), "")==0){
		Serial.println(FPSTR("ServerConnectionManager DISABLED"));
		return;
	}
	if(!this->initComplete){
		Serial.println(FPSTR("ServerConnectionManager init"));
		generateServerUrls();
		generateTempDeviceKey();
		Serial.print(FPSTR("tempDeviceKey="));
		Serial.println(tempDeviceKey);
		Serial.print(FPSTR("urlPing="));
		Serial.println(urlPing);
		Serial.print(FPSTR("urlRegister="));
		Serial.println(urlRegister);
		Serial.print(FPSTR("urlData="));
		Serial.println(urlData);
		Serial.println(FPSTR("------------------------------"));
		this->initComplete = true;
	}
}

void ServerConnectionManager::generateTempDeviceKey() {
		String tmp = "";
		tmp += random(1, 10000);
		tmp += ESP.getChipId();
		tmp += ESP.getCoreVersion();
		tmp += random(1, 10000);

		this->tempDeviceKey = strdup(sha1(tmp).c_str());

		this->serverKeyHash = sha1(conf->smartServerKey());
}

void ServerConnectionManager::generateServerUrls() {
	String baseUrl = this->conf->smartServerAddr();

	if(!baseUrl.startsWith(HTTP_PREFFIX)){
		baseUrl = HTTP_PREFFIX + baseUrl;
	}

	baseUrl += SMART_HOUSE_SERVER_URL_BASE;

	this->urlPing = baseUrl + SMART_HOUSE_SERVER_URL_PING;
	this->urlRegister = baseUrl + SMART_HOUSE_SERVER_URL_REGISTER;
	this->urlData = baseUrl + SMART_HOUSE_SERVER_URL_ON_DATA_CHANGED_GET + conf->deviceId();

}

void ServerConnectionManager::generateDeviceAuthorization(String& tempServerKey) {
	Serial.print(FPSTR("generate Authorization-------------------------"));
	Serial.print(FPSTR("tempDeviceKey="));
	Serial.println(tempDeviceKey);
	Serial.print(FPSTR("tempServerKey="));
	Serial.println(tempServerKey);
	String tmp = serverKeyHash + tempServerKey + tempDeviceKey;

	Serial.print(FPSTR("unhashedDeviceToken="));
	Serial.println(tmp);

	this->deviceAuthorization = strdup(sha1(tmp).c_str());

	String tmp2 = tempServerKey + conf->smartServerKey() + conf->deviceId();
	conf->setServerAuthorization(sha1(tmp2));

	Serial.print(FPSTR("unhashedServerToken="));
	Serial.println(tmp2);

	Serial.print(FPSTR("deviceHash="));
	Serial.println(this->deviceAuthorization);
	Serial.print(FPSTR("serverHash="));
	Serial.println(conf->getServerAuthorization());
	Serial.println(FPSTR("...done"));
}

void ServerConnectionManager::generateServerAuthorization(String& tempServerKey) {
	//Serial.print(FPSTR("generate serverAuthorization="));

	Serial.print(conf->getServerAuthorization());
	Serial.println(FPSTR("...done"));
}

void ServerConnectionManager::sendPingRequest() {
	this->runsPingRequest = true;
	Serial.print(FPSTR("Ping server ->"));

	HTTPClient http;
	http.begin(urlPing);

	int httpCode = http.GET();

	//Serial.println(httpCode);

	http.end();
	if(httpCode == 200){
		Serial.println(FPSTR("...ok"));
		triggeredServerPing = false;
		serverPinged = true;
	}else{
		Serial.print(FPSTR("...ERROR "));
		Serial.println(httpCode);
		setRequestPostPoned(SERVER_CONNECTION_PING_FAILED_TIMEOUT);
		serverPinged = false;
	}

	this->runsPingRequest = false;
}

void ServerConnectionManager::sendRegisterRequest() {
	Serial.print(FPSTR("Register device"));
	unsigned long start = millis();
	this->runsRegisterRequest = true;
	EntityJsonRequestResponse* req = new EntityJsonRequestResponse();

	JsonObject& request = req->getRequest();

	JsonObjectUtil::setField(request, _DEVICE_ID, this->conf->deviceId());
	JsonObjectUtil::setField(request, _DEVICE_FIRMWARE, this->conf->deviceFirmWare());
	JsonObjectUtil::setField(request, _DEVICE_DESCR, this->conf->deviceDescr());

	String paramsStr;
	request.printTo(paramsStr);

	String resultStr;

	HTTPClient http;

	http.begin(urlRegister);

	http.setAuthorization(tempDeviceKey);
	http.addHeader(HEADER_CONTENT_TYPE, CONTENT_TYPE_APPLICATION_JSON, 0, 1);

	int httpCode = http.POST(paramsStr);

	if(httpCode == 200){
		Serial.println(FPSTR("...ok"));
		String tempServerKey = http.getString();
		http.end();
		this->generateDeviceAuthorization(tempServerKey);
		this->generateServerAuthorization(tempServerKey);
		this->deviceRegistered = true;
		this->triggeredServerRegister = false;
		this->registrationFailures = 0;
	}else{
		http.end();
		this->serverPinged = false;
		this->registrationFailures++;

		Serial.print(FPSTR("...ERROR "));
		Serial.println(httpCode);

#ifdef SERVER_CONNECTION_REGISTRATION_FAILED_MIN_HEAP_TO_RESTART
		if(DeviceUtils::isHeapLessThan(SERVER_CONNECTION_REGISTRATION_FAILED_MIN_HEAP_TO_RESTART)){
			DeviceUtils::restart();
		}
#endif

		if(registrationFailures > SERVER_CONNECTION_NEXT_REGISTRATION_NEXT_COUNT){
			setRequestPostPoned(SERVER_CONNECTION_REGISTRATION_FAILED_FIRST_TIMEOUT);
		}else{
			setRequestPostPoned(SERVER_CONNECTION_REGISTRATION_FAILED_NEXT_TIMEOUT);
		}


	}

	DeviceUtils::printlnTimeHeap(start);
	this->runsRegisterRequest = false;
}

void ServerConnectionManager::sendDataChangedRequest() {
	this->runsDataChangeRequest = true;
	unsigned long start = millis();
	HTTPClient http;
	http.begin(urlData);

	http.setAuthorization(deviceAuthorization);

	int httpCode = http.GET();

	http.end();
	if(httpCode==200){
		triggeredDataChange = false;
	}else {
		Serial.print(FPSTR("Data send ->...ERROR"));
		Serial.println(httpCode);
		DeviceUtils::printlnTimeHeap(start);
		setRequestPostPoned(SERVER_CONNECTION_DATA_UPDATE_REQUEST_FAILED_TIMEOUT);
		this->serverPinged = false;
		this->deviceRegistered = false;
		this->triggeredServerRegister = true;
	}

	this->runsDataChangeRequest = false;
}

void ServerConnectionManager::loop(){
	if(triggeredServerPing && !runsPingRequest && isRequestEnabled() && !isRegistered()){
		sendPingRequest();
	}
	if(triggeredServerRegister && !runsRegisterRequest && isRequestEnabled() ){
		if(!serverPinged){
			triggeredServerPing = (!triggeredServerPing)?true:triggeredServerPing;
		}else{
			sendRegisterRequest();
		}
	}
	if(triggeredDataChange && !runsDataChangeRequest && isRequestEnabled() && isRegistered()){
		sendDataChangedRequest();
	}
	checkRequestPostPonedTime();
}

void ServerConnectionManager::setRequestPostPoned(unsigned long delay) {
	requestPostPoned = true;
	nextReqTime = millis() + delay;

	Serial.println(FPSTR("Req DISABLED"));
}

void ServerConnectionManager::checkRequestPostPonedTime() {
	if(requestPostPoned){
		if(millis()>nextReqTime){
			Serial.println(FPSTR("Req ENABLED"));
			requestPostPoned = false;
		}
	}
}

void ServerConnectionManager::sendDataChangedPostMethod(JsonObject& data) {
}

bool ServerConnectionManager::isRequestEnabled(){
	return initComplete && isConnected() && !requestPostPoned;
}

bool ServerConnectionManager::isConnected() {
	return WiFi.status() == WL_CONNECTED;
}

bool ServerConnectionManager::isRegistered() {
	return this->deviceRegistered;
}


/*
 * SecurityManager.cpp
 *
 *  Created on: 28 сент. 2019 г.
 *      Author: Vitaliy
 */

#include <server/SecurityManager.h>

SecurityManager::SecurityManager(WiFiSettingsBox* conf) {
	this->conf = conf;
}

void SecurityManager::triggerServerPing(bool trigger) {
	this->triggeredServerPing = trigger;
}

void SecurityManager::triggerRegisterOnServer(bool trigger) {
	this->triggeredServerRegister = trigger;
}

void SecurityManager::triggerDataChanged(bool trigger) {
	this->triggeredDataChange = trigger;
}

void SecurityManager::init(){
	if(!this->initComplete){
		generateServerUrls();
		generateTempDeviceKey();

		Serial.println(FPSTR("SecurityManager init"));
		Serial.print(FPSTR("tempDeviceKey="));
		Serial.println(tempDeviceKey);
		Serial.print(FPSTR("urlPing="));
		Serial.println(urlPing);
		Serial.print(FPSTR("urlRegister="));
		Serial.println(urlRegister);
		Serial.print(FPSTR("urlData="));
		Serial.println(urlData);
		Serial.println(FPSTR("------------------------------"));
	}
}

void SecurityManager::generateTempDeviceKey() {
		String tmp = "";
		tmp += random(1, 10000);
		tmp += ESP.getChipId();
		tmp += ESP.getCoreVersion();
		tmp += random(1, 10000);

		this->tempDeviceKey = sha1(tmp);

		this->serverKeyHash = sha1(conf->smartServerKey());
}

void SecurityManager::generateServerUrls() {
	String baseUrl = this->conf->smartServerAddr();

	if(!baseUrl.startsWith(HTTP_PREFFIX)){
		baseUrl = HTTP_PREFFIX + baseUrl;
	}

	baseUrl += SMART_HOUSE_SERVER_URL_BASE;

	this->urlPing = baseUrl + SMART_HOUSE_SERVER_URL_PING;
	this->urlRegister = baseUrl + SMART_HOUSE_SERVER_URL_REGISTER;
	this->urlData = baseUrl + SMART_HOUSE_SERVER_URL_ON_DATA_CHANGED_GET + conf->deviceId();

}

void SecurityManager::generateDeviceAuthorization(String& tempServerKey) {
	String tmp = serverKeyHash + tempServerKey + tempDeviceKey;
	this->deviceAuthorization = strdup(sha1(tmp).c_str());
}

void SecurityManager::generateServerAuthorization(String& tempServerKey) {
	String tmp = tempServerKey + conf->smartServerKey() + conf->deviceId();
	this->serverAuthorization = strdup(sha1(tmp).c_str());
}

bool SecurityManager::validateServerAuthorization(
		String& serverAuthorization) {
	return serverAuthorization = this->serverAuthorization;
}

void SecurityManager::sendPingRequest() {
	this->runsPingRequest = true;
	Serial.print(FPSTR("Ping server ->"));

	HTTPClient http;
	http.begin(urlPing);

	int httpCode = http.GET();

	Serial.println(httpCode);

	http.end();
	if(httpCode == 200){
		triggeredServerPing = false;
		serverPinged = true;
	}else{
		setRequestPostPoned(10000);
		serverPinged = false;
	}

	this->runsPingRequest = false;
}

void SecurityManager::sendRegisterRequest() {
}

void SecurityManager::sendDataChangedRequest() {
	this->runsDataChangeRequest = true;
	Serial.print(FPSTR("Data send ->"));
	HTTPClient http;
	http.begin(urlData);

	http.setAuthorization(deviceAuthorization);

	int httpCode = http.GET();

	http.end();
}

void SecurityManager::loop(){
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
	if(requestPostPoned){
		if(millis()>nextReqTime){
			requestPostPoned = false;
		}
	}
	checkRequestPostPonedTime();
}

void SecurityManager::setRequestPostPoned(unsigned long delay) {
	requestPostPoned = true;
	nextReqTime = millis() + delay;

	Serial.println(FPSTR("Req DISABLED"));
}

void SecurityManager::checkRequestPostPonedTime() {
	if(requestPostPoned){
		if(millis()>nextReqTime){
			Serial.println(FPSTR("Req ENABLED"));
			requestPostPoned = false;
		}
	}
}

void SecurityManager::sendDataChangedPostMethod(JsonObject& data) {
}

bool SecurityManager::isRequestEnabled(){
	return !requestPostPoned && initComplete && isConnected();
}

bool SecurityManager::isConnected() {
	return WiFi.status() == WL_CONNECTED;
}

bool SecurityManager::isRegistered() {
	return this->deviceRegistered;
}


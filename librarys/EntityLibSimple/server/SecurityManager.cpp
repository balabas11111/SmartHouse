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
	if(strcmp(conf->smartServerAddr(), "")==0){
		Serial.println(FPSTR("SecurityManager DISABLED"));
		return;
	}
	if(!this->initComplete){
		Serial.println(FPSTR("SecurityManager init"));
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
	}
}

void SecurityManager::generateTempDeviceKey() {
		String tmp = "";
		tmp += random(1, 10000);
		tmp += ESP.getChipId();
		tmp += ESP.getCoreVersion();
		tmp += random(1, 10000);

		this->tempDeviceKey = strdup(sha1(tmp).c_str());

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
	Serial.print(FPSTR("generate deviceAuthorization="));
	String tmp = serverKeyHash + tempServerKey + tempDeviceKey;
	this->deviceAuthorization = strdup(sha1(tmp).c_str());
	Serial.print(deviceAuthorization);
	Serial.println(FPSTR("...done"));
}

void SecurityManager::generateServerAuthorization(String& tempServerKey) {
	Serial.print(FPSTR("generate deviceAuthorization="));
	String tmp = tempServerKey + conf->smartServerKey() + conf->deviceId();
	conf->setServerAuthorization(sha1(tmp));
	Serial.print(conf->getServerAuthorization());
	Serial.println(FPSTR("...done"));
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

	int httpCode = http.POST(paramsStr);

	if(httpCode == 200){
		String tempServerKey = http.getString();
		this->generateDeviceAuthorization(tempServerKey);
		this->generateServerAuthorization(tempServerKey);
		this->deviceRegistered = true;
		this->triggeredServerRegister = false;
	}else{
		setRequestPostPoned(20000);
	}


	this->runsRegisterRequest = false;
}

void SecurityManager::sendDataChangedRequest() {
	this->runsDataChangeRequest = true;
	Serial.print(FPSTR("Data send ->"));
	HTTPClient http;
	http.begin(urlData);

	http.setAuthorization(deviceAuthorization);

	int httpCode = http.GET();

	http.end();

	if(httpCode==200){

	}else if(httpCode==403){
		setRequestPostPoned(10000);
		this->deviceRegistered = false;
		this->triggeredServerRegister = true;
	}else{
		setRequestPostPoned(10000);
	}

	this->triggeredServerPing = false;

	this->runsDataChangeRequest = false;
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
	return initComplete && isConnected() && !requestPostPoned;
}

bool SecurityManager::isConnected() {
	return WiFi.status() == WL_CONNECTED;
}

bool SecurityManager::isRegistered() {
	return this->deviceRegistered;
}


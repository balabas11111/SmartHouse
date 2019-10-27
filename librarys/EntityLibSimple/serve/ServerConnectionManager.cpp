/*
 * ServerConnectionManager.cpp
 *
 *  Created on: 28 сент. 2019 г.
 *      Author: Vitaliy
 */

#include <serve/ServerConnectionManager.h>

ServerConnectionManager::ServerConnectionManager(SettingsStorage* conf) {
	this->conf = conf;
	this->buffer = new EntityJsonRequestResponse();
}

void ServerConnectionManager::triggerServerIsOnlineCheck(bool trigger) {
	this->triggeredServerIsOnlineCheck = trigger;
}

void ServerConnectionManager::triggerServerPing(bool trigger) {
	this->triggeredServerPing = trigger;
}

void ServerConnectionManager::triggerRegisterOnServer(bool trigger) {
	Serial.println(FPSTR("Trigger registration by ServerConnectionManager"));
	this->triggeredServerRegister = trigger;
}

void ServerConnectionManager::triggerDataChanged(bool trigger) {
	//Serial.println(FPSTR("Trigger data updated request"));
	this->triggeredDataChange = trigger;
}

void ServerConnectionManager::triggerDataChangedDoSend(bool trigger) {
	//Serial.println(FPSTR("Trigger data updated request"));
	this->triggeredDataChange = trigger;
	this -> bufferUnsent = trigger;
}

EntityJsonRequestResponse* ServerConnectionManager::getBuffer() {
	if(!bufferUnsent){
		this->buffer->construct();
	}
	return this->buffer;
}

void ServerConnectionManager::init(std::function<void(void)> onServerRegistered){
	this->onServerRegistered = onServerRegistered;
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
		Serial.print(FPSTR("urlIsOnlineCheck="));
		Serial.println(urlIsOnline);
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

	this->urlPing = baseUrl + SMART_HOUSE_SERVER_URL_PING + conf->deviceId();
	this->urlRegister = baseUrl + SMART_HOUSE_SERVER_URL_REGISTER;
	this->urlIsOnline = baseUrl + SMART_HOUSE_SERVER_URL_ISONLINE + conf->deviceId();
	this->urlData = baseUrl + SMART_HOUSE_SERVER_URL_ON_DATA_CHANGED_GET + conf->deviceId();

}

void ServerConnectionManager::generateAuthorization(String& tempServerKey) {

#ifndef SETTINGS_SERVER_CONNECTION_DISABLED
	conf->generateAuthorization(tempServerKey, this->tempDeviceKey);
#else
	UNUSED(tempServerKey);
#endif
}

void ServerConnectionManager::sendPingRequest() {
	this->runsPingRequest = true;
	Serial.print(FPSTR("Ping server ->"));

	HTTPClient http;
	http.begin(urlPing);

	int httpCode = http.GET();

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

void ServerConnectionManager::sendIsOnlineRequest() {
	this->runsPingRequest = true;
	Serial.print(FPSTR("check on server registration->"));

	HTTPClient http;
	http.begin(urlIsOnline);

	int httpCode = http.GET();

	http.end();
	if(httpCode == 200){
		Serial.println(FPSTR("...ok"));
		this->serverPinged = true;
		this->triggeredServerIsOnlineCheck = false;
	}else if(httpCode == 406){
		Serial.print(FPSTR("...ERROR "));
		Serial.println(httpCode);
		//setRequestPostPoned(SERVER_CONNECTION_PING_FAILED_TIMEOUT);
		this->serverPinged = false;
		this->deviceRegistered = false;
		this->triggeredServerRegister = true;
	} else {
		setRequestPostPoned(SERVER_CONNECTION_PING_FAILED_TIMEOUT);
		this->serverPinged = false;
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
		this->generateAuthorization(tempServerKey);
		this->deviceRegistered = true;
		this->triggeredServerRegister = false;
		this->registrationFailures = 0;

		if(onServerRegistered!=nullptr){
			onServerRegistered();
		} else {
			Serial.println("No on server register listener");
		}
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
	Serial.println(FPSTR("DUR"));
	this->runsDataChangeRequest = true;
	unsigned long start = millis();
	HTTPClient http;
	http.begin(urlData);

	http.setAuthorization(deviceAuthorization);

	int httpCode = http.GET();

	http.end();
	checkServerAnswer(httpCode, start);

	this->runsDataChangeRequest = false;
}

void ServerConnectionManager::sendDataChangedGetMethod() {
	Serial.print(FPSTR("DUR_GM "));
	this->runsDataChangeRequest = true;
	unsigned long start = millis();

	String dataStr;
	this->buffer->printResponseTo(dataStr);

	//dataStr = this->urlData + DEVICE_FIELD_DATA_PARAM_GET + dataStr;

	Serial.println(dataStr);
	HTTPClient http;
	http.begin(this->urlData);

	http.setAuthorization(deviceAuthorization);
	http.addHeader(DEVICE_FIELD_DATA, dataStr);
	http.addHeader(HEADER_CONTENT_TYPE, CONTENT_TYPE_TEXT_JSON_CHARSET_UTF8);

	int httpCode = http.GET();

	http.end();

	checkServerAnswer(httpCode, start);

	this->runsDataChangeRequest = false;
}

void ServerConnectionManager::checkServerAnswer(int httpCode,
		unsigned long start) {
	bool deregister = false;

		if(httpCode==200){
			this->errorCount = 0;
			this->triggeredDataChange = false;
			this->bufferUnsent = false;

			DeviceUtils::printlnTimeHeap(start);
		}else if((httpCode < 600 && httpCode > 399) || httpCode == -1 ){
			this->errorCount++;
			setRequestPostPoned(SERVER_CONNECTION_DATA_UPDATE_REQUEST_FAILED_TIMEOUT);
			deregister = this->errorCount > SERVER_CONNECTION_DATA_UPDATE_REQUEST_FAILED_NOT_DEREGISTER_COUNT;
		}else{
			deregister = true;
		}

		if (deregister)	{
			Serial.print(FPSTR("Data send ->...ERROR "));
			Serial.println(httpCode);
			DeviceUtils::printlnTimeHeap(start);
			setRequestPostPoned(SERVER_CONNECTION_DATA_UPDATE_REQUEST_FAILED_TIMEOUT);
			this->serverPinged = false;
			this->deviceRegistered = false;
			this->triggeredServerRegister = true;
		}

}

void ServerConnectionManager::sendDataChangedPostMethod() {
}


void ServerConnectionManager::loop(){
	if(triggeredServerPing && !runsPingRequest && isRequestEnabled() && !isRegistered()){
		sendPingRequest();
	}
	if(triggeredServerIsOnlineCheck && !runsIsOnlineRequest && isRequestEnabled() &&serverPinged && isRegistered()) {
		sendIsOnlineRequest();
	}
	if(triggeredServerRegister && !runsRegisterRequest && isRequestEnabled() ){
		if(!serverPinged){
			triggeredServerPing = (!triggeredServerPing)?true:triggeredServerPing;
		}else{
			sendRegisterRequest();
		}
	}
	if(triggeredDataChange && !runsDataChangeRequest && isRequestEnabled() && isRegistered()){
	#ifdef SETTINGS_SERVER_SEND_DATA_METHOD_GET
		if(!bufferUnsent){
			sendDataChangedRequest();
		} else {
			sendDataChangedGetMethod();
		}
	#else
		sendDataChangedRequest();
	#endif
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

bool ServerConnectionManager::isRequestEnabled(){
	return initComplete && isConnected() && !requestPostPoned;
}

bool ServerConnectionManager::isConnected() {
	return WiFi.status() == WL_CONNECTED;
}

bool ServerConnectionManager::isRegistered() {
	return this->deviceRegistered;
}


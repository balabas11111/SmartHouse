/*
 * ServerConnector.cpp
 *
 *  Created on: 3 но€б. 2019 г.
 *      Author: Vitaliy
 */

#include <serve/ServerConnector.h>

ServerConnector::ServerConnector(SettingsStorage* conf, std::function<void(void)> onServerRegistered) {
	this->conf = conf;
	this->onServerRegistered = onServerRegistered;
	this->buffer = new EntityJsonRequestResponse();
}

bool ServerConnector::isConnected() {
	return WiFi.status() == WL_CONNECTED;
}

void ServerConnector::checkConnection() {
	requestRuns = true;


	Serial.print(FPSTR("Register device"));
	unsigned long start = millis();
	HTTPClient http;

	http.begin(urlRegister);

	http.addHeader(HEADER_CONTENT_TYPE, CONTENT_TYPE_APPLICATION_JSON, 0, 1);

	int httpCode = http.GET();

	if(httpCode == 200){
		Serial.println(FPSTR("...ok"));
		String tempServerKey = http.getString();
		http.end();
		this->deviceRegistered = true;

		if(onServerRegistered!=nullptr){
			onServerRegistered();
		} else {
			Serial.println("No on server register listener");
		}
	}else{
		http.end();
		this->deviceRegistered = false;
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

	requestRuns = false;
}

void ServerConnector::setRequestPostPoned(unsigned long delay) {
	requestPostPoned = true;
	nextReqTime = millis() + delay;

	Serial.println(FPSTR("Req DISABLED"));
}

void ServerConnector::init() {
	String baseUrl = this->conf->smartServerAddr();

	if(!baseUrl.startsWith(HTTP_PREFFIX)){
		baseUrl = HTTP_PREFFIX + baseUrl;
	}

	this->urlRegister = baseUrl + SMART_HOUSE_SERVER_URL_REGISTER + conf->deviceId();

	String authorization = sha1(conf->smartServerKey());

	conf->setServerAuthorization(authorization);
	conf->setDeviceAuthorization(authorization);

	initComplete = true;
}

void ServerConnector::triggerCheckConnection() {
	this->triggeredRequest = true;
}

bool ServerConnector::isRequestEnabled(){
	return initComplete && isConnected() && !requestPostPoned;
}

void ServerConnector::loop() {
	if(triggeredRequest && !requestRuns && isRequestEnabled() ){
		checkConnection() ;
	}
}

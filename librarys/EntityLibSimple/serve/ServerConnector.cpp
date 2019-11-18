/*
 * ServerConnector.cpp
 *
 *  Created on: 3 ����. 2019 �.
 *      Author: Vitaliy
 */

#include <serve/ServerConnector.h>

ServerConnector::ServerConnector(SettingsStorage* conf,
		std::function<void(void)> onServerRegistered) {
	this->conf = conf;
	this->onServerRegistered = onServerRegistered;

	String baseUrl = this->conf->smartServerAddr();

	if (!baseUrl.startsWith(HTTP_PREFFIX)) {
		baseUrl = HTTP_PREFFIX + baseUrl;
	}

	this->urlRegister = baseUrl + SMART_HOUSE_SERVER_URL_REGISTER
			+ conf->deviceId();
	this->urlDataChanged = baseUrl + SMART_HOUSE_SERVER_URL_DATA_CHANGED
			+ conf->deviceId();

	String authorization = HashUtils::encode_sha1(conf->smartServerKey());

	conf->setServerAuthorization(authorization);
	conf->setDeviceAuthorization(authorization);

	initComplete = true;

	Serial.print(FPSTR("ServerConnector init urlReg="));
	Serial.print(this->urlRegister);
	Serial.print(FPSTR(" urlDataChanged="));
	Serial.println(this->urlDataChanged);
}

bool ServerConnector::isConnected() {
	return WiFi.status() == WL_CONNECTED;
}

void ServerConnector::checkRegistrationOnServer() {
	requestRuns = true;

	unsigned long start = millis();
	HTTPClient http;

	http.begin(urlRegister);

	http.addHeader(HEADER_CONTENT_TYPE, CONTENT_TYPE_APPLICATION_JSON, 0, 1);

	int httpCode = http.GET();
	http.end();

	if (httpCode == 200 || httpCode == 201) {
		Serial.println(FPSTR("Registered...ok"));

		this->registrationFailures = 0;
		this->deviceRegistered = true;
		this->triggeredCheckRegistrationOnServer = false;
		this->nextReqTime = millis() + 60000;

		if (onServerRegistered != nullptr) {
			onServerRegistered();
		}
	} else {http.end();
		this->deviceRegistered = false;
		this->registrationFailures++;

		Serial.print(FPSTR("Register device...ERROR "));
		Serial.println(httpCode);

#ifdef SERVER_CONNECTION_REGISTRATION_FAILED_MIN_HEAP_TO_RESTART
		if (DeviceUtils::isHeapLessThan(
				SERVER_CONNECTION_REGISTRATION_FAILED_MIN_HEAP_TO_RESTART)) {
			DeviceUtils::restart();
		}
#endif

		if (registrationFailures
				> SERVER_CONNECTION_NEXT_REGISTRATION_NEXT_COUNT) {
			setRequestPostPoned(
					SERVER_CONNECTION_REGISTRATION_FAILED_NEXT_TIMEOUT);
			this->registrationFailures = 0;
		} else {
			setRequestPostPoned(
					SERVER_CONNECTION_REGISTRATION_FAILED_FIRST_TIMEOUT);
		}

		DeviceUtils::printlnTimeHeap(start);
	}

	requestRuns = false;
}

void ServerConnector::dispatchDataChanged() {
	if(initComplete && isConnected()) {
		requestRuns = true;

		unsigned long start = millis();
		HTTPClient http;

		http.begin(urlDataChanged);

		int httpCode = http.GET();
		http.end();

		if (httpCode == 200) {
			Serial.println(FPSTR("DataChanged dispatch...ok"));
		} else {
			Serial.print(FPSTR("DataChanged dispatch...ERROR "));
			Serial.println(httpCode);
		}

		DeviceUtils::printlnTimeHeap(start);

		requestRuns = false;
	}
}

void ServerConnector::setRequestPostPoned(unsigned long delay) {
	requestPostPoned = true;
	nextReqTime = millis() + delay;

	Serial.print(FPSTR("Req DISABLED delay "));
	Serial.println(delay);
}

void ServerConnector::checkCancelRequestPostPoned() {
	if (requestPostPoned) {
		if (millis() > nextReqTime) {
			Serial.println(FPSTR("Req ENABLED"));
			requestPostPoned = false;
		}
	}
}

void ServerConnector::triggerCheckRegistrationOnServer() {
	if(!this->triggeredCheckRegistrationOnServer) {
		Serial.println(FPSTR("REGISTRATION triggered"));
	}
	this->triggeredCheckRegistrationOnServer = true;
}

bool ServerConnector::isRequestEnabled() {
	return initComplete && isConnected() && !requestPostPoned;
}

void ServerConnector::loop() {
	checkCancelRequestPostPoned();
	if (this->triggeredCheckRegistrationOnServer && !requestRuns && isRequestEnabled()) {
		checkRegistrationOnServer();
	}
}

bool ServerConnector::isLastRequestFromServerTimedOut(bool shrReceived,
		unsigned long lastFromServerRequestTime) {
	if(shrReceived) {
		return lastFromServerRequestTime + LAST_FROM_SERVER_REQUEST_TIMED_OUT < millis();
	}
	return !this->deviceRegistered	&& nextReqTime < millis();
}

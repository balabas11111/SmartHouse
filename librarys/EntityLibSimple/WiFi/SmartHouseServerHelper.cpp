/*
 * SmartHouseServerHelper.cpp
 *
 *  Created on: 21 сент. 2019 г.
 *      Author: Vitaliy
 */

#include <WiFi/SmartHouseServerHelper.h>

SmartHouseServerHelper::SmartHouseServerHelper(WiFiSettingsBox* conf) {
	this->conf = conf;
}

void SmartHouseServerHelper::loop() {
	this->executeOnServerRegisterIfTriggered();
	this->executeOnServerDataChangedIfTriggered();
}

int SmartHouseServerHelper::sendGetDataRequest(EntityJsonRequestResponse* req) {
	addServerUrlToJson(req->getRequest(), "/devices/", JSONKEY_url);

	JsonObjectUtil::printWithPreffix("GetDataRequest =", req->getRequest());

	return HttpUtils::executeGetRequest(req, JSONKEY_url);
}

int SmartHouseServerHelper::sendRegisterRequest(EntityJsonRequestResponse* req) {
	prepareRegisterRequest(req->getRequest());

	JsonObjectUtil::printWithPreffix("RegisterRequest =", req->getRequest());

	return HttpUtils::executePostRequest(req, JSONKEY_url);
}

int SmartHouseServerHelper::sendDataUpdateRequest(EntityJsonRequestResponse* req) {
	prepareSendDataUpdateRequest(req->getRequest());

	return HttpUtils::executePostRequest(req, JSONKEY_url);
}

void SmartHouseServerHelper::triggerOnServerRegister() {
	this->triggeredOnServerRegister = true;
}

void SmartHouseServerHelper::triggerOnServerDeviceDataChanged() {
	this->triggeredOnServerDataChanged = true;
}

void SmartHouseServerHelper::executeOnServerRegisterIfTriggered() {
	if(WiFi.status() == WL_CONNECTED){
		if(!this->registered){

			if(timeoutExecuteRegister && nextRegisterExecution-millis()>0){
				return;
			}

			if(this->triggeredOnServerRegister){
				if(!serverRegisterRuns){
					this->serverRegisterRuns = true;
					unsigned long start = millis();
					Serial.println(FPSTR("-------------------------"));
					Serial.println(FPSTR("Server register triggered"));
					EntityJsonRequestResponse* req = new EntityJsonRequestResponse();

					createDeviceRegistrationRequest(req->getRequest());

					int status = HttpUtils::executePostRequest(req, JSONKEY_url);

					if(status == 200){
						this->registered = true;
						this->registerErrors = 0;
						this->timeoutExecuteRegister = false;
						Serial.println(FPSTR("Device was REGISTERED on server"));
					}else{
						Serial.println(FPSTR("ERROR REGISTER on server"));
						this->registerErrors++;
					}

					delete req;

					ObjectUtils::printTimeHeap(start);
					Serial.println(FPSTR("-------------------------"));

					this->triggeredOnServerRegister = false;
					this->serverRegisterRuns = false;
				}
			}

			if(registerErrors>MAX_REGISTER_ERRORS){
				this->registerErrors = 0;
				this->timeoutExecuteRegister = true;
				this->nextRegisterExecution = millis() + MAX_CONNECTION_ERRORS_REACHED_TIMEOUT;

				Serial.println(FPSTR("Registration will be timedOut"));
			}
		}
	}
}

void SmartHouseServerHelper::executeOnServerDataChangedIfTriggered() {
	if(WiFi.status() == WL_CONNECTED){
		if(this->registered){

			if(this->triggeredOnServerDataChanged){
				if(!serverDataChangedRuns){
					this->serverDataChangedRuns = true;
					unsigned long start = millis();
					Serial.println(FPSTR("-------------------------"));
					Serial.println(FPSTR("Trigerring DataChanged"));
					EntityJsonRequestResponse* req = new EntityJsonRequestResponse();

					createDeviceDataChangedRequest(req->getRequest());

					JsonObjectUtil::printWithPreffix("DataChanged Request =", req->getRequest());

					int status = HttpUtils::executePostRequest(req, JSONKEY_url);

					delete req;
					ObjectUtils::printTimeHeap(start);
					if(status == 200){
						Serial.println(FPSTR("DataChange was dispatched"));
						this->connectionErrors = 0;
					}else{
						Serial.println(FPSTR("DataChange dispatch error"));
						this->connectionErrors++;
					}
					Serial.println(FPSTR("-------------------------"));

					this->triggeredOnServerDataChanged = false;
					this->serverDataChangedRuns = false;
				}
			}

			if(this->connectionErrors>MAX_CONNECTION_ERRORS){
				this->connectionErrors = 0;
				this->registered = false;
			}
		}
	}
}

void SmartHouseServerHelper::prepareSendDataUpdateRequest(JsonObject& json) {
	addServerUrlToJson(json, SMART_HOUSE_SERVER_URL_SENDDATA, JSONKEY_url);
}

void SmartHouseServerHelper::prepareRegisterRequestTest(JsonObject& json,
		const char* urlPath) {

	JsonObject& device = json.createNestedObject(_DEVICE);
	addServerKeyHashToJson(device);
	addServerUrlToJson(json, urlPath, JSONKEY_url);

	//JsonObjectUtil::printWithPreffix("Prepared vals =", json);
}


void SmartHouseServerHelper::createDeviceRegistrationRequest(
		JsonObject& request) {

	JsonObjectUtil::setField(request, _DEVICE_ID, conf->deviceId());
	JsonObjectUtil::setField(request, _DEVICE_FIRMWARE, conf->deviceFirmWare());
	JsonObjectUtil::setField(request, _DEVICE_DESCR, conf->deviceDescr());
	JsonObjectUtil::setField(request, KEY_SERVER_KEY, getServerKeyHashed());
	JsonObjectUtil::setField(request, KEY_DEVICE_KEY, getDeviceKeyHashed());

	JsonObjectUtil::setField(request, KEY_ROOT_URL, getDeviceRootUrl());
	JsonObjectUtil::setField(request, KEY_DATA_URL, getDeviceDataUrl());

	addServerUrlToJson(request, SMART_HOUSE_SERVER_URL_REGISTERSIMPLE, JSONKEY_url);
}

void SmartHouseServerHelper::createDeviceDataChangedRequest(
		JsonObject& request) {
	JsonObjectUtil::setField(request, _DEVICE_ID, conf->deviceId());
	JsonObjectUtil::setField(request, KEY_DEVICE_KEY, getDeviceKeyHashed());

	addServerUrlToJson(request, SMART_HOUSE_SERVER_URL_ON_DATA_CHANGED, JSONKEY_url);
}

void SmartHouseServerHelper::prepareRegisterRequest(JsonObject& json) {
	JsonObject& device = json.createNestedObject(_DEVICE);

	addDeviceInfoToJson(device);
	addDeviceUrlsToJson(device);
	addServerKeyHashToJson(device);

	addServerUrlToJson(json, SMART_HOUSE_SERVER_URL_REGISTER, JSONKEY_url);

	JsonObjectUtil::printWithPreffix("Prepared vals =", json);
}


void SmartHouseServerHelper::getServerKeyHashed(String& serverKeyHash) {
	serverKeyHash = (sha1(conf->smartServerKey()));
}

char* SmartHouseServerHelper::getServersToken() {
	return this->serverToken;
}

void SmartHouseServerHelper::setServersToken(String& serverToken) {
	this->serverToken = strdup(serverToken.c_str());
}

void SmartHouseServerHelper::addDeviceInfoToJson(JsonObject& json) {
	JsonObjectUtil::setField(json, _DEVICE_ID, conf->deviceId());
	JsonObjectUtil::setField(json, _DEVICE_FIRMWARE, conf->deviceFirmWare());
	JsonObjectUtil::setField(json, _DEVICE_DESCR, conf->deviceDescr());
	JsonObjectUtil::setField(json, KEY_IP, conf->getCurrentIp());

	addDeviceTokenToJson(json);
}

void SmartHouseServerHelper::addServerTokenToJson(JsonObject& json) {
	JsonObjectUtil::setField(json, KEY_SERVER_TOKEN, getServersToken());
}

void SmartHouseServerHelper::addServerKeyHashToJson(JsonObject& json) {
	String serverKeyHash;
	getServerKeyHashed(serverKeyHash);
/*
	Serial.print(FPSTR("serverKeyHash="));
	Serial.println(serverKeyHash);
*/
	JsonObjectUtil::setField(json, KEY_SERVER_KEY, strdup(serverKeyHash.c_str()));
}

void SmartHouseServerHelper::addServerUrlToJson(JsonObject& json, const char* serviceUrl, const char* jsonKey) {
	String serverUrl;

	getServerBaseUrl(serverUrl);
	serverUrl += serviceUrl;
/*
	Serial.print(FPSTR("serviceUrl ="));
	Serial.println(serviceUrl);
	Serial.print(FPSTR("jsonKey ="));
	Serial.println(jsonKey);
	Serial.print(FPSTR("serverUrl ="));
	Serial.println(serverUrl);
*/
	JsonObjectUtil::setField(json, jsonKey, strdup(serverUrl.c_str()));
}

void SmartHouseServerHelper::addDeviceUrlsToJson(JsonObject& json) {
	JsonObjectUtil::setField(json, KEY_ROOT_URL, getDeviceRootUrl());
	JsonObjectUtil::setField(json, KEY_DATA_URL, getDeviceDataUrl());
}

void SmartHouseServerHelper::getServerBaseUrl(String& baseUrl) {
	baseUrl = conf->smartServerAddr();

	if(!baseUrl.startsWith(HTTP_PREFFIX)){
		baseUrl = HTTP_PREFFIX + baseUrl;
	}

	baseUrl += SMART_HOUSE_SERVER_URL_BASE;
/*
	Serial.print(FPSTR("server baseUrl="));
	Serial.println(baseUrl);
	*/
}

char* SmartHouseServerHelper::getDeviceBaseUrl() {
	/*Serial.print(FPSTR("getDeviceBaseUrl currentIP ="));
	Serial.print(conf->getCurrentIp());
*/
	if(this->deviceBaseUrl == nullptr){
		//Serial.print(FPSTR(" construct "));
		String deviceBaseUrlStr=HTTP_PREFFIX;
		deviceBaseUrlStr += conf->getCurrentIp();
		this->deviceBaseUrl = strdup(deviceBaseUrlStr.c_str());
	}
	/*Serial.print(FPSTR(" deviceBaseUrl ="));
	Serial.println(this->deviceBaseUrl);
*/
	return this->deviceBaseUrl;
}

char* SmartHouseServerHelper::getDeviceRootUrl() {
	if(this->deviceRootUrl == nullptr){
		this->deviceRootUrl = strdup((String(getDeviceBaseUrl()) + URL_ROOT).c_str());
	}
	return this->deviceRootUrl;
}

char* SmartHouseServerHelper::getDeviceDataUrl() {
	if(this->deviceDataUrl == nullptr){
			this->deviceDataUrl = strdup((String(getDeviceBaseUrl()) + URL_DATA).c_str());
		}
		return this->deviceDataUrl;
}

bool SmartHouseServerHelper::validateServersToken(String& serverToken) {
	return serverToken == this->serverToken;
}

char* SmartHouseServerHelper::getServerKeyHashed() {
	if(this->serverToken == nullptr){
		String serverTokenHash = sha1(conf->smartServerKey());
		this->serverToken = strdup(serverTokenHash.c_str());
	}
	return this->serverToken;
}

char* SmartHouseServerHelper::getDeviceKeyHashed() {
	if(this->deviceToken == nullptr){
		generateDeviceToken();
	}
	return this->deviceToken;
}

void SmartHouseServerHelper::generateDeviceToken() {
	Serial.print(FPSTR("Generate deviceToken ="));
	String deviceToken = "";
	deviceToken += ESP.getChipId();
	deviceToken += ESP.getCoreVersion();
	deviceToken += conf->deviceId();
	deviceToken += conf->deviceFirmWare();
	deviceToken += random(1, 10000);

	Serial.print(deviceToken);

	String deviceTokenHash = sha1(deviceToken);

	this->deviceToken = strdup(deviceTokenHash.c_str());

	Serial.print(FPSTR(" hash ="));
	Serial.println(this->deviceToken);
}

bool SmartHouseServerHelper::validateDeviceToken(String& deviceToken) {
	return deviceToken == this->deviceToken;
}

void SmartHouseServerHelper::addDeviceTokenToJson(JsonObject& json) {
	JsonObjectUtil::setField(json, KEY_DEVICE_TOKEN, getDeviceKeyHashed());
}




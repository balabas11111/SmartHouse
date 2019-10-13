/*
 * MqttManager.h
 *
 *  Created on: Oct 11, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_SERVE_MQTTMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_SERVE_MQTTMANAGER_H_

#include "Arduino.h"
#include "ArduinoJson.h"
#include "DeviceConfig.h"
#include "WiFi/HttpConstants.h"
#include "functional"
#include "SettingsStorage.h"
#include "EntityJsonRequestResponse.h"

#include "ESP8266WiFi.h"
#include "PubSubClient.h"
#include "DeviceUtils.h"

#define MQTT_REGISTRATION_TOPIC     "fromDevice/register"
#define MQTT_TO_DEVICE_TOPIC_TMPL   "toDevice/"
#define MQTT_FROM_DEVICE_TOPIC_TMPL "fromDevice/"
#define MQTT_GOOD_STATUS "\"status\":200"
#define MQTT_GOOD_STATUS_STR "\"status\":\"200\""
#define MQTT_SLASH_SUFFIX "/"

class MqttManager {
public:
	MqttManager(SettingsStorage* conf);
	virtual ~MqttManager(){};

	void init(EntityJsonRequestResponse* buf, bool registered = true);
	void loop();

	EntityJsonRequestResponse* getBuffer();

	bool publishBuffer();
private:
	bool bufferUnsent = false;
	SettingsStorage* conf;
	EntityJsonRequestResponse* buffer;

	//IPAddress adress;
	WiFiClient* wclient;
	PubSubClient* client;

	boolean initDone = false;

	char* rootUrl = (char*)"";
	char* dataUrl = (char*)"";

	String toServerTopic;
	char* fromDeviceTopic;//strdup((String(MQTT_FROM_DEVICE_TOPIC_TMPL) + conf->deviceId()).c_str());
	char* toDeviceTopic;//strdup((String(MQTT_TO_DEVICE_TOPIC_TMPL) + conf->deviceId()).c_str());

	unsigned long lastReconnectAttempt;
	unsigned long lastRegisterAttempt;

	bool registered = false;
	bool serverSubscribed = false;
	bool serverResponseReceived = false;
	bool registrationRequestBuilt = false;

	void callback(char* topic, uint8_t* payload, unsigned int length);

	bool connectMqtt();

	bool buildRegistrationRequest();
	bool sendRegistrationRequest();
	bool processRegistrationResponse();
	bool sendDataRequest();

	bool subscribe(char* topic, bool showLog = false);
	bool publish(char* topic, JsonObject& data, bool showLog = false);

	bool isDataSendEnabled();
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_SERVE_MQTTMANAGER_H_ */

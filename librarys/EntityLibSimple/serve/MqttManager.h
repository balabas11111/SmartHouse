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
#include "utils/DeviceUtils.h"

#define MQTT_REGISTRATION_TOPIC     "fromDevice/register"
#define MQTT_TO_DEVICE_TOPIC   "to/"
#define MQTT_FROM_DEVICE_TOPIC "from/"
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

	bool isBufferUnsent();
private:
	bool bufferUnsent = false;
	SettingsStorage* conf = nullptr;
	EntityJsonRequestResponse* buffer = nullptr;

	WiFiClient* wclient = nullptr;
	PubSubClient* client = nullptr;

	boolean initDone = false;

	char* rootUrl = (char*)"";
	char* dataUrl = (char*)"";

	String toServerTopic;
	char* fromDeviceTopic;//strdup((String(MQTT_FROM_DEVICE_TOPIC_TMPL) + conf->deviceId()).c_str());
	char* toDeviceTopic;//strdup((String(MQTT_TO_DEVICE_TOPIC_TMPL) + conf->deviceId()).c_str());

	unsigned long lastReconnectAttempt = 0;
	unsigned long lastRegisterAttempt;

	bool registered = false;
	bool toDeviceTopicSubscribed = false;
	bool serverResponseReceived = false;
	bool registrationRequestBuilt = false;

	void callback(char* topic, uint8_t* payload, unsigned int length);

	bool connectMqtt();

	bool subscribeToDeviceTopic();

	bool buildRegistrationRequest();
	bool sendRegistrationRequest();
	bool processRegistrationResponse();
	bool sendDataRequest();

	bool subscribe(char* topic, bool showLog = false);
	bool publish(char* topic, JsonObject& data, bool showLog = true);

	bool isDataSendEnabled();

	DynamicJsonBuffer entityFieldsBuffer;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_SERVE_MQTTMANAGER_H_ */

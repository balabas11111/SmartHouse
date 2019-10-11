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
#include "functional"
#include "SettingsStorage.h"
#include "EntityJsonRequestResponse.h"
#include "PubSubClient.h"
#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "DeviceUtils.h"

#define MQTT_REGISTRATION_TOPIC     "fromDevice/register"
#define MQTT_TO_DEVICE_TOPIC_TMPL   "toDevice/"
#define MQTT_FROM_DEVICE_TOPIC_TMPL "fromDevice/"

class MqttManager {
public:
	MqttManager(SettingsStorage* conf);
	virtual ~MqttManager(){};

	void init();

	EntityJsonRequestResponse* getBuffer();

	bool publishBuffer();
private:
	bool bufferUnsent = false;
	EntityJsonRequestResponse* buffer = nullptr;
	PubSubClient* client = nullptr;

	char* toServerQueue = (char*)"";
	char* toDeviceQueue = (char*)"";

	WiFiClient wclient;

	unsigned long lastReconnectAttempt;

	bool registered = false;

	void callback(char* topic, uint8_t* payload, unsigned int length);

	bool connectMqtt();
	bool sendBuffer();
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_SERVE_MQTTMANAGER_H_ */

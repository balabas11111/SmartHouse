/*
 * ButtonB.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_MQTTHELPER_H_
#define LIBRARIES_TIMETRIGGER_MQTTHELPER_H_

#include "Arduino.h"
#include <EspSettingsBox.h>
#include <PubSubClient.h>
#include "PinEvent.h"
#include "Loopable.h"

class MqttHelper: public Loopable{

public:
	MqttHelper(EspSettingsBox *_settingsBox,String* _subscribeTopics,std::function<void(PinEvent)> externalPinEventFunction,std::function<void(String topic,String message)> externalCallbackFunction);
	virtual ~MqttHelper();

	void init(Client& client);

	boolean isConnected();
	boolean connectIfNotConnected();
	void subscribe(String topicName);
	boolean publish(char* topicName,String message);

	boolean publish(String message);

	String getName();
	String displayDetails();
	boolean loop();

	void callback(char* topic, uint8_t* payload, unsigned int length);
	PubSubClient getClient();

private:
	String* subscribeTopics;
	std::function<void(PinEvent)> externalPinEventFunction;
	std::function<void(String topic,String message)> externalCallbackFunction;
	PubSubClient client;

	char* mqtt_server;
	uint16_t mqtt_port;
	char* mqtt_user;
	char* mqtt_pass;
	char* mqtt_topic;
	char* mqtt_startMessage;

	boolean active;

	Client* wiFiClient;

	void connect();
};


#endif /* LIBRARIES_TIMETRIGGER_MQTTHELPER_H_ */

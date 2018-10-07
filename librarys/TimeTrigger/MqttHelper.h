/*
 * ButtonB.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_MQTTHELPER_H_
#define LIBRARIES_TIMETRIGGER_MQTTHELPER_H_

#include "Arduino.h"
#include "EspSettingsBox.h"
#include <PubSubClient.h>
#include "AbstractEvent.h"

class MqttHelper{

public:
	MqttHelper(EspSettingsBox *_settingsBox,String* _subscribeTopics,std::function<void(AbstractEvent)> afunc,Client& _client);
	void init();
	void connect();
	boolean isConnected();
	boolean connectIfNotConnected();
	void subscribe(String topicName);
	boolean publish(String topicName,String message);

	void loop();

	void callback(char* topic, uint8_t* payload, unsigned int length);
	PubSubClient getClient();

private:
	String* subscribeTopics;
	std::function<void(AbstractEvent)> externalhandlerFunction;
	EspSettingsBox *settingsBox;
	PubSubClient client;
};


#endif /* LIBRARIES_TIMETRIGGER_MQTTHELPER_H_ */

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
#include "interfaces/Loopable.h"
#include "interfaces/Initializable.h"
#include <functional>

class MqttHelper: public Loopable{

public:
	MqttHelper(EspSettingsBox *_settingsBox,Client& _client,std::function<void(String topic,String message)> _externalCallbackFunction);

	virtual ~MqttHelper();

	boolean begin(AbstractItem** items,uint8_t count);

	virtual boolean initialize();

	boolean isConnected();
	boolean connectIfNotConnected();
	void subscribe(String topicName);
	boolean publish(String topicName,String message);

	String getName();
	void displayDetails();
	boolean loop();

	void callback(char* topic, uint8_t* payload, unsigned int length);
	PubSubClient getClient();

	void senDAbstractItemToMqtt(AbstractItem* item);

	void processMqttEvent(String topic,String message,AbstractItem** items,uint8_t count);

private:
	boolean initialized;
	uint8_t topicCount;
	String* subscribeTopics;
	std::function<void(String topic,String message)> externalCallbackFunction;
	PubSubClient client;

	EspSettingsBox *espSettingsBox;
/*
	char* mqtt_server;
	uint16_t mqtt_port;
	char* mqtt_user;
	char* mqtt_pass;
	char* mqtt_topic;
	char* mqtt_startMessage;
	char* deviceId;
*/
	Client* wiFiClient;

	void connect();
	void printCurrentQueues();

};


#endif /* LIBRARIES_TIMETRIGGER_MQTTHELPER_H_ */

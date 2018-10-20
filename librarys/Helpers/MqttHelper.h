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
#include "Initializable.h"
#include "PinEventProcessor.h"

class MqttHelper: public Loopable,public Initializable{

public:
	MqttHelper(EspSettingsBox *_settingsBox,String* _subscribeTopics,uint8_t _topicCount,Client& _client,PinEventProcessor *eventProcessors[],uint8_t procSize,std::function<void(String topic,String message)> _externalCallbackFunction);

	PinEvent processEvent(PinEvent event);
	virtual ~MqttHelper();

	virtual boolean initialize(boolean _init) override;

	boolean isConnected();
	boolean connectIfNotConnected();
	void subscribe(String topicName);
	boolean publish(char* topicName,String message);

	boolean publish(PinEvent event);

	String getName();
	String displayDetails();
	boolean loop();

	void callback(char* topic, uint8_t* payload, unsigned int length);
	PubSubClient getClient();

private:
	PinEventProcessor **eventProcessors;
	uint8_t procSize;

	uint8_t topicCount;
	String* subscribeTopics;
	//std::function<void(PinEvent)> externalPinEventFunction;
	std::function<void(String topic,String message)> externalCallbackFunction;
	PubSubClient client;

	EspSettingsBox *settingsBox;
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
	boolean publish(String message);
};


#endif /* LIBRARIES_TIMETRIGGER_MQTTHELPER_H_ */

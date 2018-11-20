#include "Arduino.h"
#include "MqttHelper.h"
#include <PubSubClient.h>
#include "AbstractItem.h"
#include "ESP_Consts.h"

#include <ESP8266WiFi.h>


#define BUFFER_SIZE 100

MqttHelper::MqttHelper(EspSettingsBox *_settingsBox,Client& _client,std::function<void(String topic,String message)> _externalCallbackFunction){
	Serial.println(FPSTR(MESSAGE_MQTTHELPER_CREATE_STARTED));
	externalCallbackFunction=_externalCallbackFunction;

	topicCount=0;
	//subscribeTopics=_subscribeTopics;

	espSettingsBox=_settingsBox;

	client=PubSubClient((char*)_settingsBox->mqtt_server.c_str(), _settingsBox->mqtt_port, [this](char* topic, uint8_t* payload, unsigned int length){callback(topic,payload,length);}, _client);

	initialized=false;
	displayDetails();

	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}

MqttHelper::~MqttHelper(){
}

boolean MqttHelper::begin(AbstractItem** items,uint8_t count){
	topicCount=0;

	if(!espSettingsBox->isMqttEnabled){
		Serial.println(FPSTR(MESSAGE_MQTTHELPER_SEND_IS_DISABLED));
		return false;
	}
	Serial.println(FPSTR(MESSAGE_MQTTHELPER_BEGIN));

	for(uint8_t i=0;i<count;i++){
		if(items[i]->getAutoCreateChannel()){
			for(uint8_t j=0;j<items[i]->getItemCount();j++){
				if(items[i]->getQueue(j)!=""){
					topicCount++;
				}
			}
		}
	}

	Serial.print(FPSTR(MESSAGE_MQTTHELPER_TOPIC_COUNT));
	Serial.println(topicCount);

	if(topicCount>0){

		subscribeTopics=new String[topicCount];
		uint8_t index=0;

		for(uint8_t i=0;i<count;i++){
			if(items[i]->getAutoCreateChannel()){
				for(uint8_t j=0;j<items[i]->getItemCount();j++){
					if(items[i]->getQueue(j)!=""){
						subscribeTopics[index]=items[i]->getQueue(j);
						Serial.print(FPSTR(MESSAGE_MQTTHELPER_TOPIC_ADDED_TO_SUBSCRIBED));
						Serial.println(subscribeTopics[index]);
						index++;
					}
				}
			}
		}
	}

	initialize();

	return topicCount!=0;
}

boolean MqttHelper::initialize(){
	if(topicCount>0){
		initialized=true;
		Serial.println(FPSTR(MESSAGE_MQTTHELPER_INIT_STARTED));
		//displayDetails();

		connectIfNotConnected();
		Serial.println(FPSTR(MESSAGE_MQTTHELPER_INIT_COMPLETE));
		displayDetails();
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
	}
	return initialized;
}

String MqttHelper::displayDetails(){

	Serial.print(FPSTR(MESSAGE_MQTTHELPER_NAME));

	Serial.print(FPSTR(MESSAGE_MQTTHELPER_MQTT_USER_EQ));
	Serial.print(espSettingsBox->mqtt_user);
	Serial.print(FPSTR(MESSAGE_SPACE));

	Serial.print(FPSTR(MESSAGE_MQTTHELPER_MQTT_PASS_EQ));
	Serial.print(espSettingsBox->mqtt_pass);
	Serial.print(FPSTR(MESSAGE_SPACE));

	Serial.print(FPSTR(MESSAGE_MQTTHELPER_TOPIC_COUNT));
	Serial.println(topicCount);
	Serial.print(FPSTR(MESSAGE_SPACE));

	Serial.println(FPSTR(MESSAGE_MQTTHELPER_MQTT_CONNECTED_EQ));
	Serial.print(client.connected());
	Serial.print(FPSTR(MESSAGE_SPACE));

	Serial.println();
	printCurrentQueues();

	Serial.println();

	return "";
}

void MqttHelper::connect(){
	if (!client.connected()) {
	  Serial.println(FPSTR(MESSAGE_MQTTHELPER_MQTT_START_CONNECT_EQ));
	  String clientName;
	  clientName += String(espSettingsBox->DeviceId)+millis();

	if (client.connect((char*) clientName.c_str(),(char*)espSettingsBox->mqtt_user.c_str(),(char*)espSettingsBox->mqtt_pass.c_str())) {
		Serial.println(FPSTR(MESSAGE_MQTTHELPER_MQTT_CONNECTED_EQ));
		Serial.print(client.connected());

		if(espSettingsBox->mqtt_topic!="" && espSettingsBox->mqtt_topic!="none"){
			if (publish((char*)espSettingsBox->mqtt_topic.c_str(), espSettingsBox->DeviceId)) {
			  Serial.println(FPSTR(MESSAGE_MQTTHELPER_MQTT_TEST_PUBLISH_OK));
			}
			else {
				Serial.println(FPSTR(MESSAGE_MQTTHELPER_MQTT_TEST_PUBLISH_FAILED));
			}
		}

		for(int i=0;i<topicCount;i++){
			subscribe(subscribeTopics[i]);
		}

	  }
	  else {
		  Serial.println(FPSTR(MESSAGE_MQTTHELPER_MQTT_CONNECT_FAILED));
		delay(5000);
		//abort();
	  }
	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}
}

void MqttHelper::printCurrentQueues(){
	for(int i=0;i<topicCount;i++){
		Serial.print(subscribeTopics[i]);
		Serial.print(";");
	}
}

boolean MqttHelper::isConnected(){
	return client.connected();
}

boolean MqttHelper::connectIfNotConnected(){
	if(WiFi.status()!=WL_CONNECTED){
		Serial.print(FPSTR(MESSAGE_MQTTHELPER_MQTT_WIFI_CONNECT_FAILED));
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_EQ));
		Serial.println(WiFi.status());
		return false;
	}

	connect();

	return isConnected();
}

void MqttHelper::subscribe(String topicName){
	boolean res=client.subscribe((char*) topicName.c_str());

	Serial.print(FPSTR(MESSAGE_MQTTHELPER_TOPIC_SUBSCRIBED));
	Serial.print(topicName);
	Serial.print(FPSTR(MESSAGE_MQTTHELPER_TOPIC_SUBSCRIBED_RES_EQ));
	Serial.println(res);
}

boolean MqttHelper::publish(String topicName,String message){
	if(!initialized){
		Serial.println(FPSTR(MESSAGE_MQTTHELPER_ERR_NOT_INITIALIZED));
		return false;
	}
	if(!connectIfNotConnected()){
		Serial.println(FPSTR(MESSAGE_MQTTHELPER_ERR_NOT_CONNECTED));
		return false;
	}

	boolean result=client.publish( (char*)topicName.c_str(),(char*) message.c_str());
	return result;
}

boolean MqttHelper::publish(String message){
	if(initialized){
		if(!connectIfNotConnected()){
			return false;
		}

		boolean result=client.publish( (char*)espSettingsBox->mqtt_topic.c_str(),(char*) message.c_str());
		if(result){
			Serial.println(FPSTR(MESSAGE_MQTTHELPER_SENT));
		}else{
			Serial.println(FPSTR(MESSAGE_MQTTHELPER_SEND_FAILED));
		}
		return result;
	}
	Serial.println(FPSTR(MESSAGE_MQTTHELPER_ERR_NOT_INITIALIZED));

	return false;

}

String MqttHelper::getName(){
	return espSettingsBox->DeviceId;
}

boolean MqttHelper::loop(){
	if(initialized){
		connectIfNotConnected();

		if (client.connected()){
		  return client.loop();
		}

		return false;
	}

	return false;
}

void MqttHelper::callback(char* topic, uint8_t* payload, unsigned int _length) {

	String payloadIn = (char*)payload;
	String topicIn=String(topic);

	String msg;
		for(unsigned int i=0;i<_length;i++){
			msg+=(char)payload[i];
		}

	//String messageIn="Message received payload="+msg+" topic="+topic +" length="+_length;
	//Serial.println(messageIn);

	if(externalCallbackFunction!=nullptr)
			externalCallbackFunction(topicIn,msg);


}

PubSubClient MqttHelper::getClient(){
	return getClient();
}

void MqttHelper::senDAbstractItemToMqtt(AbstractItem* item){
	Serial.println(FPSTR(MESSAGE_MQTTHELPER_SENDING_MESSAGE));
	for(uint8_t i=0;i<item->getItemCount();i++){
		String queue=item->getItem(i).queue;

		if(queue.length()!=0){
			String val=item->getValStr(i);

			Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_QUEUE_EQ));
			Serial.println(queue);

			queue.replace(espSettingsBox->thSkRKey, espSettingsBox->thSkWKey);
			queue.replace("subscribe", "publish");

			//channels/623698/subscribe/fields/field2/N9EQ8RTYQ7ZXYR8T
			//channels/<channelID>/publish/fields/field<fieldnumber>/<apikey>


			Serial.print(FPSTR(MESSAGE_MQTTHELPER_TARGET_QUEUE_EQ));
			Serial.print(queue);

			Serial.print(FPSTR(MESSAGE_MQTTHELPER_VAL_EQ));
			Serial.println(val);

			if(espSettingsBox->isMqttEnabled){
				boolean res=publish(queue, val);
				Serial.print(FPSTR(MESSAGE_MQTTHELPER_RESULT_EQ));
				Serial.println(res);
			}else{
				Serial.println(FPSTR(MESSAGE_MQTTHELPER_ERR_NOT_ENABLED));
			}
			/*
			if(espSettingsBox.sendItemsToBaseQUeue){
				mqttHelper.publish(item->getJson());
			}
			*/
		}
	}
	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}

void MqttHelper::processMqttEvent(String topic, String message,
		AbstractItem** items, uint8_t count) {

	Serial.println(FPSTR(MESSAGE_MQTTHELPER_PROCESS_ABSTRACT_ITEM_MESSAGE_RECEIVED));
	Serial.print(FPSTR(MESSAGE_MQTTHELPER_TOPIC_EQ));
	Serial.print(topic);
	Serial.print(FPSTR(MESSAGE_MQTTHELPER_MESSAGE_EQ));
	Serial.print(message);
	Serial.println(FPSTR(MESSAGE_MQTTHELPER_MESSAGE_END));

	boolean result=false;

	for(uint8_t i=0;i<count;i++){
		result=items[i]->processMqValue(topic, message);
		if(result){
			break;
		}
	}

	Serial.print(FPSTR(MESSAGE_MQTTHELPER_RESULT_EQ));
	Serial.println(result);

	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}

#include "Arduino.h"
#include "MqttHelper.h"
#include <PubSubClient.h>
#include "AbstractSensor.h"
#include "ESP_Consts.h"
#include <FunctionalInterrupt.h>
#include <EspSettingsBox.h>
#include "extraBoxes/EspSett_Device.h"
#include "extraBoxes/EspSett_Mq.h"

#ifdef ESP8266
	#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
	#include <WiFi.h>
#endif


#define BUFFER_SIZE 100

MqttHelper::MqttHelper(EspSettingsBox *_settingsBox,Client& _client,std::function<void(String topic,String message)> _externalCallbackFunction){
	Serial.println(FPSTR(MESSAGE_MQTTHELPER_CREATE_STARTED));
	externalCallbackFunction=_externalCallbackFunction;

	topicCount=0;
	//subscribeTopics=_subscribeTopics;

	espSettingsBox=_settingsBox;

	client=PubSubClient((char*)espSettingsBox->getExtraValue(ExtraBox_mqtt, MQTT_server).c_str(),espSettingsBox->getExtraValueInt(ExtraBox_mqtt, MQTT_port), [this](char* topic, uint8_t* payload, unsigned int length){callback(topic,payload,length);}, _client);
	enabled=espSettingsBox->getExtraValueBoolean(ExtraBox_mqtt, MQTTE_enabled);

	initialized=false;
	displayDetails();

	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}

MqttHelper::~MqttHelper(){
}

boolean MqttHelper::begin(AbstractSensor** items,uint8_t count){
	topicCount=0;

	if(!espSettingsBox->getExtraValueBoolean(ExtraBox_mqtt, MQTTE_enabled)){
		Serial.println(FPSTR(MESSAGE_MQTTHELPER_SEND_IS_DISABLED));
		return false;
	}
	Serial.println(FPSTR(MESSAGE_MQTTHELPER_BEGIN));

	for(uint8_t i=0;i<count;i++){
		if(items[i]->getAutoCreateChannel()){
			for(uint8_t j=0;j<items[i]->getItemCount();j++){
				/*if(items[i]->getQueue(j)!=""){
					topicCount++;
				}*/
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
					/*if(items[i]->getQueue(j)!=""){
						subscribeTopics[index]=items[i]->getQueue(j);
						Serial.print(FPSTR(MESSAGE_MQTTHELPER_TOPIC_ADDED_TO_SUBSCRIBED));
						Serial.println(subscribeTopics[index]);
						index++;
					}*/
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

void MqttHelper::displayDetails(){

	Serial.print(FPSTR(MESSAGE_MQTTHELPER_NAME));

	Serial.print(FPSTR(MESSAGE_MQTTHELPER_MQTT_USER_EQ));
	Serial.print(espSettingsBox->getExtraValue(ExtraBox_mqtt, MQTT_user));
	Serial.print(FPSTR(MESSAGE_SPACE));

	Serial.print(FPSTR(MESSAGE_MQTTHELPER_MQTT_PASS_EQ));
	Serial.print(espSettingsBox->getExtraValue(ExtraBox_mqtt, MQTT_password));
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
}

void MqttHelper::connect(){
	if (!client.connected()) {
	  Serial.println(FPSTR(MESSAGE_MQTTHELPER_MQTT_START_CONNECT_EQ));
	  String clientName;
	  clientName += espSettingsBox->getExtraValue(FPSTR(DEVICE_SETTINGS_BOX_NAME), DEVICE_id)+millis();

	if (client.connect((char*) clientName.c_str(),(char*)espSettingsBox->getExtraValue(ExtraBox_mqtt, MQTT_user).c_str(),(char*)espSettingsBox->getExtraValue(ExtraBox_mqtt, MQTT_password).c_str())) {
		Serial.println(FPSTR(MESSAGE_MQTTHELPER_MQTT_CONNECTED_EQ));
		Serial.print(client.connected());

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

String MqttHelper::getName(){
	return FPSTR("MqttHelper");
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

void MqttHelper::senDAbstractSensorToMqtt(AbstractSensor* item){
	Serial.println(FPSTR(MESSAGE_MQTTHELPER_SENDING_MESSAGE));
	for(uint8_t i=0;i<item->getItemCount();i++){
		/*String queue=item->getItem(i).queue;

		if(queue.length()!=0){
			String val=item->getValStr(i);

			Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_QUEUE_EQ));
			Serial.println(queue);

			String wKey=espSettingsBox->getExtraValue(ExtraBox_thingSpeak, 3);
			String rKey=espSettingsBox->getExtraValue(ExtraBox_thingSpeak, 4);

			queue.replace(wKey, rKey);
			queue.replace("subscribe", "publish");

			//channels/623698/subscribe/fields/field2/N9EQ8RTYQ7ZXYR8T
			//channels/<channelID>/publish/fields/field<fieldnumber>/<apikey>


			Serial.print(FPSTR(MESSAGE_MQTTHELPER_TARGET_QUEUE_EQ));
			Serial.print(queue);

			Serial.print(FPSTR(MESSAGE_MQTTHELPER_VAL_EQ));
			Serial.println(val);

			if(enabled){
				boolean res=publish(queue, val);
				Serial.print(FPSTR(MESSAGE_MQTTHELPER_RESULT_EQ));
				Serial.println(res);
			}else{
				Serial.println(FPSTR(MESSAGE_MQTTHELPER_ERR_NOT_ENABLED));
			}

		}*/
	}
	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}

void MqttHelper::processMqttEvent(String topic, String message,
		AbstractSensor** items, uint8_t count) {

	Serial.println(FPSTR(MESSAGE_MQTTHELPER_PROCESS_ABSTRACT_ITEM_MESSAGE_RECEIVED));
	Serial.print(FPSTR(MESSAGE_MQTTHELPER_TOPIC_EQ));
	Serial.print(topic);
	Serial.print(FPSTR(MESSAGE_MQTTHELPER_MESSAGE_EQ));
	Serial.print(message);
	Serial.println(FPSTR(MESSAGE_MQTTHELPER_MESSAGE_END));

	boolean result=false;

	for(uint8_t i=0;i<count;i++){
		/*result=items[i]->processMqValue(topic, message);
		if(result){
			break;
		}
		*/
	}

	Serial.print(FPSTR(MESSAGE_MQTTHELPER_RESULT_EQ));
	Serial.println(result);

	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}

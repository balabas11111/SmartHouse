#include "Arduino.h"
#include "MqttHelper.h"
#include <PubSubClient.h>

#include <ESP8266WiFi.h>


#define BUFFER_SIZE 100

MqttHelper::MqttHelper(EspSettingsBox *_settingsBox,String* _subscribeTopics,uint8_t _topicCount,Client& _client,std::function<void(String topic,String message)> _externalCallbackFunction){
	Serial.println("-------------------------------");
	Serial.println("Initialize MqttHelper");
	externalCallbackFunction=_externalCallbackFunction;

	topicCount=_topicCount;
	subscribeTopics=_subscribeTopics;

	settingsBox=_settingsBox;

	client=PubSubClient((char*)_settingsBox->mqtt_server.c_str(), _settingsBox->mqtt_port, [this](char* topic, uint8_t* payload, unsigned int length){callback(topic,payload,length);}, _client);

	initialized=false;
	displayDetails();

	Serial.println("-------------------------------");
}

MqttHelper::~MqttHelper(){
	Serial.println("MqttHelper destructed");
}

boolean MqttHelper::initialize(boolean _init){
	if(_init){
		Serial.println("------Mqtt client init--------");
		displayDetails();

		connectIfNotConnected();
		Serial.println("Init completed");
		displayDetails();
		Serial.println("-------------------------------");
	}
	initialized=_init;
	return initialized;
}

String MqttHelper::displayDetails(){
	String wfConnected=String(client.connected());

	String res="MqttHelper mqtt_user="+String((char*)(settingsBox->mqtt_user).c_str())+" mqtt_pass="+String((char*)(settingsBox->mqtt_pass).c_str())+" mqtt_topic="+settingsBox->mqtt_topic+" mqtt_startMessage="+(settingsBox->DeviceId)+" topicCount="+String(topicCount)+" connected="+String(wfConnected);
	Serial.println(res);

	for(int i=0;i<topicCount;i++){
		Serial.print(subscribeTopics[i]);
		Serial.print(";");
	}

	return res;
}

void MqttHelper::connect(){
	if (!client.connected()) {
	  Serial.println("Start connect mqttClient");
	  String clientName;
	  clientName += String(settingsBox->DeviceId)+millis();

	if (client.connect((char*) clientName.c_str(),(char*)settingsBox->mqtt_user.c_str(),(char*)settingsBox->mqtt_pass.c_str())) {
		Serial.println("Connected to MQTT broker");
		Serial.print("Publish Topic is: ");

		if (publish((char*)settingsBox->mqtt_topic.c_str(), settingsBox->DeviceId)) {
		  Serial.println("Publish ok");
		}
		else {
		  Serial.println("Publish failed");
		}

		for(int i=0;i<topicCount;i++){
			subscribe(subscribeTopics[i]);
		}

	  }
	  else {
		Serial.println("MQTT connect failed");
		Serial.println("Will reset and try again...");
		delay(5000);
		//abort();
	  }
}
}
boolean MqttHelper::isConnected(){
	return client.connected();
}

boolean MqttHelper::connectIfNotConnected(){
	if(WiFi.status()!=WL_CONNECTED){
		Serial.println("No wiFi connection");
		Serial.println("WiFi.status="+WiFi.status());
		return false;
	}

	connect();

	return isConnected();
}

void MqttHelper::subscribe(String topicName){
	Serial.print("subscribe to "+(topicName));
	boolean res=client.subscribe((char*) topicName.c_str());
	String resStr=res?" OK":" FAILED";

	Serial.println(" ..."+resStr);
}

boolean MqttHelper::publish(char* topicName,String message){
	if(!connectIfNotConnected()){
		return false;
	}

	boolean result=client.publish( topicName,(char*) message.c_str());
	return result;
}

boolean MqttHelper::publish(String message){
	if(initialized){
		if(!connectIfNotConnected()){
			return false;
		}

		boolean result=client.publish( (char*)settingsBox->mqtt_topic.c_str(),(char*) message.c_str());
		if(result){
			Serial.println(" sent");
		}else{
			Serial.println(" mq FAILED");
		}
		return result;
	}
	Serial.println(" MqttHelper is not initialized");

	return false;

}

String MqttHelper::getName(){
	return settingsBox->DeviceId;
}

boolean MqttHelper::loop(){
	if(initialized){
		connectIfNotConnected();

		boolean result=false;

		if (client.connected()){
		  result=result || client.loop();
		}

		return result;
	}

	return false;
}

void MqttHelper::callback(char* topic, uint8_t* payload, unsigned int _length) {

	String payloadIn = (char*)payload;
	String topicIn=String(topic);

	String msg;
		for(int i=0;i<_length;i++){
			msg+=(char)payload[i];
		}

	String messageIn="Message received payload="+msg+" topic="+topic +" length="+_length;

	Serial.println(messageIn);

	if(externalCallbackFunction!=nullptr)
			externalCallbackFunction(topicIn,msg);


}

PubSubClient MqttHelper::getClient(){
	return getClient();
}

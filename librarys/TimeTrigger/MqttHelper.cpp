#include "Arduino.h"
#include "MqttHelper.h"
#include "AbstractEvent.h"
#include <PubSubClient.h>

MqttHelper::MqttHelper(EspSettingsBox *_settingsBox,String* _subscribeTopics,std::function<void(AbstractEvent)> _externalhandlerFunction,Client& _client){
	settingsBox=_settingsBox;
	externalhandlerFunction=_externalhandlerFunction;
	subscribeTopics=_subscribeTopics;

	std::function<void(char*, uint8_t*, unsigned int)> afunc =
				(void(*)(char*, uint8_t*, unsigned int))&MqttHelper::callback;

	const char *mqtt_server = settingsBox->mqtt_server.c_str(); // ��� ������� MQTT
	const int mqtt_port = settingsBox->mqtt_port;

	client=PubSubClient(mqtt_server,mqtt_port, afunc, _client);
	//client=PubSubClient(_client);
}

void MqttHelper::init(){
	std::function<void(char*, uint8_t*, unsigned int)> afunc =
			(void(*)(char*, uint8_t*, unsigned int))&MqttHelper::callback;

	const char *mqtt_server = settingsBox->mqtt_server.c_str(); // ��� ������� MQTT
	const int mqtt_port = settingsBox->mqtt_port;

	connect();
}

void MqttHelper::connect(){
	if (!client.connected()) {

	  String clientName;
	  clientName += "esp8266 "+millis();

	if (client.connect((char*) clientName.c_str(),(char*)(settingsBox->mqtt_user).c_str(),(char*)(settingsBox->mqtt_pass).c_str())) {
		Serial.println("Connected to MQTT broker");
		Serial.print("Publish Topic is: ");

		if (publish(settingsBox->mqtt_topic, "hello from ESP8266")) {
		  Serial.println("Publish ok");
		}
		else {
		  Serial.println("Publish failed");
		}

		for(int i=0;i<sizeof(subscribeTopics);i++){
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
	while(!isConnected()){
		connect();

		if(!isConnected()){
			delay(10000);
		}
	}

	return isConnected();
}

void MqttHelper::subscribe(String topicName){
	Serial.println("subscribe to "+topicName);
	client.subscribe((char*) topicName.c_str());
}

boolean MqttHelper::publish(String topicName,String message){
	boolean result=client.publish((char*) topicName.c_str(),(char*) message.c_str());
	return result;
}

void MqttHelper::loop(){
	if (client.connected()){
	  client.loop();
	  //sendData();
	}
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

	externalhandlerFunction(AbstractEvent(msg));
}

PubSubClient MqttHelper::getClient(){
	return getClient();
}

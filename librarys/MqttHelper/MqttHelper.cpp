#include "Arduino.h"
#include "MqttHelper.h"
#include "PinEvent.h"
#include <PubSubClient.h>

MqttHelper::MqttHelper(EspSettingsBox *_settingsBox,String* _subscribeTopics,std::function<void(PinEvent)> _externalPinEventFunction,std::function<void(String topic,String message)> _externalCallbackFunction,Client& _client){
	//settingsBox=_settingsBox;
	externalPinEventFunction=_externalPinEventFunction;
	externalCallbackFunction=_externalCallbackFunction;

	subscribeTopics=_subscribeTopics;

	std::function<void(char*, uint8_t*, unsigned int)> afunc =
				(void(*)(char*, uint8_t*, unsigned int))&MqttHelper::callback;

	char* mqtt_server = (char*)_settingsBox->mqtt_server.c_str(); // Имя сервера MQTT
	uint16_t mqtt_port = _settingsBox->mqtt_port;
	mqtt_user = (char*)(_settingsBox->mqtt_user).c_str();
	mqtt_pass = (char*)(_settingsBox->mqtt_pass).c_str();
	mqtt_topic= (char*)(_settingsBox->mqtt_topic).c_str();
	mqtt_startMessage = (char*)(_settingsBox->DeviceId).c_str();

	client=PubSubClient(mqtt_server,mqtt_port, afunc, _client);
}

void MqttHelper::connect(){
	if (!client.connected()) {

	  String clientName;
	  clientName += String(mqtt_startMessage)+millis();

	if (client.connect((char*) clientName.c_str(),mqtt_user,mqtt_pass)) {
		Serial.println("Connected to MQTT broker");
		Serial.print("Publish Topic is: ");

		if (publish(mqtt_topic, mqtt_startMessage)) {
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

boolean MqttHelper::publish(char* topicName,String message){
	boolean result=client.publish( topicName,(char*) message.c_str());
	return result;
}

void MqttHelper::loop(){
	connectIfNotConnected();

	if (client.connected()){
	  client.loop();
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

	if(externalPinEventFunction!=nullptr && msg.length()>3 && msg.startsWith(PIN_EVENT_PREFFIX))
		externalPinEventFunction(PinEvent(msg));

	if(externalCallbackFunction!=nullptr)
		externalCallbackFunction(topicIn,msg);
}

PubSubClient MqttHelper::getClient(){
	return getClient();
}

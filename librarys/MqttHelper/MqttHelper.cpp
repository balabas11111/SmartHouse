#include "Arduino.h"
#include "MqttHelper.h"
#include "PinEvent.h"
#include <PubSubClient.h>

#include <ESP8266WiFi.h>

#define BUFFER_SIZE 100

MqttHelper::MqttHelper(EspSettingsBox *_settingsBox,String* _subscribeTopics,uint8_t _topicCount,std::function<void(char*, uint8_t*, unsigned int)> _callback,Client& _client){
	Serial.println("-------------------------------");
	Serial.println("Initialize MqttHelper");
	//settingsBox=_settingsBox;
	//externalPinEventFunction=_externalPinEventFunction;
	//externalCallbackFunction=_externalCallbackFunction;

	//std::function<void(char*, uint8_t*, unsigned int)> afunc =
	//					(void(*)(char*, uint8_t*, unsigned int))&MqttHelper::callback;

	topicCount=_topicCount;
	subscribeTopics=_subscribeTopics;

	mqtt_server = (char*)_settingsBox->mqtt_server.c_str(); // Имя сервера MQTT
	mqtt_port = _settingsBox->mqtt_port;
	mqtt_user = (char*)(_settingsBox->mqtt_user).c_str();
	mqtt_pass = (char*)(_settingsBox->mqtt_pass).c_str();
	mqtt_topic= (char*)(_settingsBox->mqtt_topic).c_str();
	mqtt_startMessage = (char*)(_settingsBox->DeviceId).c_str();

	client=PubSubClient(mqtt_server,mqtt_port, _callback, _client);

	active=false;
	displayDetails();

	Serial.println("-------------------------------");
}

MqttHelper::~MqttHelper(){
	Serial.println("MqttHelper destructed");
}

void MqttHelper::init(){
	Serial.println("Init mqtt client");
	displayDetails();

	connectIfNotConnected();
	Serial.println("Init completed");
	displayDetails();
	Serial.println("-------------------------------");

	active=true;
}

String MqttHelper::displayDetails(){
	String wfConnected=String(client.connected());

	String res="MqttHelper mqtt_user="+String(mqtt_user)+" mqtt_pass="+String(mqtt_pass)+" mqtt_topic="+String(mqtt_topic)+" mqtt_startMessage="+String(mqtt_startMessage)+" topicCount="+String(topicCount)+" connected="+String(wfConnected);
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
	if(!connectIfNotConnected()){
		return false;
	}

	boolean result=client.publish( mqtt_topic,(char*) message.c_str());
	return result;
}

String MqttHelper::getName(){
	return String(mqtt_startMessage);
}

boolean MqttHelper::loop(){
	if(active){
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

	if(externalPinEventFunction!=nullptr && msg.length()>3 && msg.startsWith(PIN_EVENT_PREFFIX))
		externalPinEventFunction(PinEvent(msg));

	if(externalCallbackFunction!=nullptr)
		externalCallbackFunction(topicIn,msg);
}

PubSubClient MqttHelper::getClient(){
	return getClient();
}

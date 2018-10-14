#include "Arduino.h"
#include "MqttHelper.h"
#include "PinEvent.h"
#include <PubSubClient.h>

#include <ESP8266WiFi.h>

MqttHelper::MqttHelper(EspSettingsBox *_settingsBox,String* _subscribeTopics,std::function<void(PinEvent)> _externalPinEventFunction,std::function<void(String topic,String message)> _externalCallbackFunction){
	Serial.println("-------------------------------");
	Serial.println("Initialize MqttHelper");
	//settingsBox=_settingsBox;
	externalPinEventFunction=_externalPinEventFunction;
	externalCallbackFunction=_externalCallbackFunction;

	subscribeTopics=_subscribeTopics;

	mqtt_server = (char*)_settingsBox->mqtt_server.c_str(); // Имя сервера MQTT
	mqtt_port = _settingsBox->mqtt_port;
	mqtt_user = (char*)(_settingsBox->mqtt_user).c_str();
	mqtt_pass = (char*)(_settingsBox->mqtt_pass).c_str();
	mqtt_topic= (char*)(_settingsBox->mqtt_topic).c_str();
	mqtt_startMessage = (char*)(_settingsBox->DeviceId).c_str();

	active=false;

	Serial.println("-------------------------------");
}

MqttHelper::~MqttHelper(){
	Serial.println("MqttHelper destructed");
}

void MqttHelper::init(Client& _client){
	std::function<void(char*, uint8_t*, unsigned int)> afunc =
					(void(*)(char*, uint8_t*, unsigned int))&MqttHelper::callback;

	Serial.println("Init mqtt client");
	client=PubSubClient(mqtt_server,mqtt_port, afunc, _client);

	Serial.println("Connect to mq");
	connectIfNotConnected();

	wiFiClient=&_client;

	active=true;
}

String MqttHelper::displayDetails(){
	int topicCount=sizeof(subscribeTopics);
	String wfConnected=String(client.connected());

	for(int i=0;i<topicCount;i++){
		Serial.print(subscribeTopics[i]);
		Serial.print(";");
	}

	String res="MqttHelper mqtt_user="+String(mqtt_user)+" mqtt_pass="+String(mqtt_pass)+" mqtt_topic="+String(mqtt_topic)+" mqtt_startMessage="+String(mqtt_startMessage)+" topicCount="+String(topicCount+" connected="+wfConnected);
	Serial.println(res);

	return res;
}

void MqttHelper::connect(){
	Serial.println("Start connect mqttClient");
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
	Serial.println("WiFi.status="+WiFi.status());

	if(WiFi.status()!=WL_CONNECTED){
		Serial.println("Wait for wiFi connection");
		return false;
	}

	connect();

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

boolean MqttHelper::publish(String message){
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

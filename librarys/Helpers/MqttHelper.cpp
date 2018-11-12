#include "Arduino.h"
#include "MqttHelper.h"
#include <PubSubClient.h>
#include "AbstractItem.h"

#include <ESP8266WiFi.h>


#define BUFFER_SIZE 100

MqttHelper::MqttHelper(EspSettingsBox *_settingsBox,Client& _client,std::function<void(String topic,String message)> _externalCallbackFunction){
	Serial.println("-------------------------------");
	Serial.println("Initialize MqttHelper");
	externalCallbackFunction=_externalCallbackFunction;

	topicCount=0;
	//subscribeTopics=_subscribeTopics;

	espSettingsBox=_settingsBox;

	client=PubSubClient((char*)_settingsBox->mqtt_server.c_str(), _settingsBox->mqtt_port, [this](char* topic, uint8_t* payload, unsigned int length){callback(topic,payload,length);}, _client);

	initialized=false;
	displayDetails();

	Serial.println("-------------------------------");
}

MqttHelper::~MqttHelper(){
	Serial.println("MqttHelper destructed");
}

boolean MqttHelper::begin(AbstractItem** items,uint8_t count){
	topicCount=0;

	Serial.println("------Mqtt BEGIN--------");

	for(uint8_t i=0;i<count;i++){
		if(items[i]->getAutoCreateChannel()){
			for(uint8_t j=0;j<items[i]->getItemCount();j++){
				if(items[i]->getQueue(j)!=""){
					topicCount++;
				}
			}
		}
	}

	Serial.print("Subscribes count=");
	Serial.println(topicCount);

	if(topicCount>0){

		subscribeTopics=new String[topicCount];
		uint8_t index=0;

		for(uint8_t i=0;i<count;i++){
			if(items[i]->getAutoCreateChannel()){
				for(uint8_t j=0;j<items[i]->getItemCount();j++){
					if(items[i]->getQueue(j)!=""){
						subscribeTopics[index]=items[i]->getQueue(j);
						Serial.print("added topic=");
						Serial.println(subscribeTopics[index]);
						index++;
					}
				}
			}
		}
		Serial.println("Topics added");
	}
	Serial.println("-----------------------------------------");

	initialize();

	return topicCount!=0;
}

boolean MqttHelper::initialize(){
	if(topicCount>0){
		initialized=true;
		Serial.println("------Mqtt client init--------");
		//displayDetails();

		connectIfNotConnected();
		Serial.println("Init completed");
		displayDetails();
		Serial.println("-------------------------------");
	}
	return initialized;
}

String MqttHelper::displayDetails(){
	String wfConnected=String(client.connected());

	Serial.print("MqttHelper ");

	Serial.print("mqtt_user=");
	Serial.print("'");
	Serial.print(espSettingsBox->mqtt_user);
	Serial.print("'; (");
	Serial.print((espSettingsBox->mqtt_user).c_str());
	Serial.print("); ");

	Serial.print("mqtt_pass=");
	Serial.print("'");
	Serial.print(espSettingsBox->mqtt_pass);
	Serial.print("'; (");
	Serial.print((espSettingsBox->mqtt_pass).c_str());
	Serial.print("); ");

	Serial.print("topicCount=");
	Serial.print("'");
	Serial.print(topicCount);
	Serial.print("'; ");

	Serial.print("mqtt_topic=");
	Serial.print("'");
	Serial.print(espSettingsBox->mqtt_topic);
	Serial.print("'; ");

	Serial.println("connected=");
	Serial.print("'");
	Serial.print(wfConnected);
	Serial.print("'; ");

	Serial.println();
	printCurrentQueues();

	Serial.println();

	return "";
}

void MqttHelper::connect(){
	if (!client.connected()) {
	  Serial.println("==============Start connect mqttClient================");
	  String clientName;
	  clientName += String(espSettingsBox->DeviceId)+millis();

	if (client.connect((char*) clientName.c_str(),(char*)espSettingsBox->mqtt_user.c_str(),(char*)espSettingsBox->mqtt_pass.c_str())) {
		Serial.println("Connected to MQTT broker");
		Serial.print("Publish Topic is: ");
		Serial.println(espSettingsBox->mqtt_topic);

		if(espSettingsBox->mqtt_topic!="" && espSettingsBox->mqtt_topic!="none"){
			if (publish((char*)espSettingsBox->mqtt_topic.c_str(), espSettingsBox->DeviceId)) {
			  Serial.println("Publish ok");
			}
			else {
			  Serial.println("Publish failed");
			}
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
	Serial.println("================================");
}
}

void MqttHelper::printMqttDiagnostic(){
	Serial.println("-----------------Mqtt Status----------------");
	Serial.print("server=");
	Serial.println("server=");
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

boolean MqttHelper::publish(String topicName,String message){
	if(!initialized){
		Serial.println("Not initialized");
		return false;
	}
	if(!connectIfNotConnected()){
		Serial.println("Not connected");
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
		for(int i=0;i<_length;i++){
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

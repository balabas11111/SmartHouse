#include "Arduino.h"
#include "MqttHelper.h"
#include "PinEvent.h"
#include <PubSubClient.h>

#include <ESP8266WiFi.h>


#define BUFFER_SIZE 100

MqttHelper::MqttHelper(EspSettingsBox *_settingsBox,String* _subscribeTopics,uint8_t _topicCount,Client& _client,PinEventProcessor *_eventProcessors[],uint8_t _procSize,std::function<void(String topic,String message)> _externalCallbackFunction){
	Serial.println("-------------------------------");
	Serial.println("Initialize MqttHelper");
	//settingsBox=_settingsBox;
	//externalPinEventFunction=_externalPinEventFunction;
	externalCallbackFunction=_externalCallbackFunction;

	//std::function<void(char*, uint8_t*, unsigned int)> afunc =
	//					(void(*)(char*, uint8_t*, unsigned int))&MqttHelper::callback;
	eventProcessors=_eventProcessors;
	procSize=_procSize;
	topicCount=_topicCount;
	subscribeTopics=_subscribeTopics;

	settingsBox=_settingsBox;

	client=PubSubClient((char*)_settingsBox->mqtt_server.c_str(), _settingsBox->mqtt_port, [this](char* topic, uint8_t* payload, unsigned int length){callback(topic,payload,length);}, _client);

	initialized=false;
	displayDetails();

	Serial.println("-------------------------------");
}

PinEvent MqttHelper::processEvent(PinEvent event){
	/*
	 //kind:bubble:pinId:oldVal:val:strVal:dispatcherName:targetName:

	UPDATESTate command

		http://192.168.0.100/runCommand?command=PE_SS:1:14:0:1:PE_SS:http:lampLeft:
		http://192.168.0.100/runCommand?command=PE_SS:1:14:1:0:PE_SS:http:lampLeft:
		http://192.168.0.100/runCommand?command=PE_SS:1:15:0:1:PE_SS:http:lampRight:
		http://192.168.0.100/runCommand?command=PE_SS:1:15:1:0:PE_SS:http:lampRight:

	GETState measurers command

		http://192.168.0.100/runCommand?command=PE_SG:1:0:0:0:PE_SG:http:bmeMeasurer:
		http://192.168.0.100/runCommand?command=PE_SG:1:0:0:0:PE_SG:http:luxMeasurer:
	*/
	Serial.println("-----------------------------------------");
	Serial.println(" ProcessEvent "+event.getText());
	PinEvent newEvent=PinEvent();

	if(!event.isValid() || !event.isNotEmpty()){
		return newEvent;
	}

	if(event.isBubble()){
		publish(event);
		event.setIsBubble(false);
	}

		boolean processed=false;

		for(uint8_t i=0;i<procSize;i++){
			newEvent=eventProcessors[i]->processEventNow(event);
			if(newEvent.isNotEmpty()){
				Serial.println(eventProcessors[i]->printProcessInfo(event)+" new->"+newEvent.getText());
				processed=true;
				break;
			}
		}

	if(newEvent.isNotEmpty()){
		//Serial.print("New event ="+newEvent.getText());
		if(newEvent.isValid()){

			if(newEvent.isBubble()){
				publish(newEvent);
				newEvent.setIsBubble(false);
			}else{
				//Serial.println("...not sent");
			}

			processEvent(newEvent);
		}else{
			//Serial.println("...not valid");
		}
	}
	if(!processed){
		//Serial.println("NO PROCESSOR FOUND "+newEvent.getText());
	}

	return newEvent;
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

boolean MqttHelper::publish(PinEvent event){
	event.setSendDevice(settingsBox->DeviceId);

	String mess=event.getText();
	Serial.print(mess+"...sending");
	boolean res=publish(mess);

	Serial.println("..."+res?"OK":"FAILED");
	return res;
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

	if(msg.length()>3 && msg.startsWith(PIN_EVENT_PREFFIX)){
		processEvent(PinEvent(msg));
	}else{
		if(externalCallbackFunction!=nullptr)
				externalCallbackFunction(topicIn,msg);
		/*if(externalPinEventFunction!=nullptr)
			externalPinEventFunction(PinEvent(msg));
			*/
	}


}

PubSubClient MqttHelper::getClient(){
	return getClient();
}

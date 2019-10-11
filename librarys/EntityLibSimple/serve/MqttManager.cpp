/*
 * MqttManager.cpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <serve/MqttManager.h>

MqttManager::MqttManager(SettingsStorage* conf) {
	this->buffer = new EntityJsonRequestResponse();

	this->toServerQueue = strdup((String(MQTT_FROM_DEVICE_TOPIC_TMPL) + conf->deviceId()).c_str());
	this->toDeviceQueue = strdup((String(MQTT_TO_DEVICE_TOPIC_TMPL) + conf->deviceId()).c_str());

	this->lastReconnectAttempt = millis();
}

void MqttManager::init() {
	this->client = new PubSubClient(MQTT_HOST, MQTT_PORT,
			/*[this](char* topic, uint8_t* payload, unsigned int length){ callback(topic, payload, length);},*/
					this->wclient);
}

EntityJsonRequestResponse* MqttManager::getBuffer() {
		if(!bufferUnsent){
			this->buffer->construct();
		}
		return this->buffer;
}

bool MqttManager::publishBuffer() {
	unsigned long start = millis();
	Serial.print(FPSTR("MQTT sent="));

	if(connectMqtt()){
		String msg;
		this->buffer->printResponseTo(msg);
		bool sent = client->publish(((registered)?toServerQueue:MQTT_REGISTRATION_TOPIC), msg.c_str());
		this->bufferUnsent = !sent;
		Serial.print(sent);
		Serial.print(FPSTR(" "));

		return sent;
	} else {
		Serial.print(FPSTR("nc "));
		return false;
	}

	DeviceUtils::printlnTimeHeap(start);
}

void MqttManager::callback(char* topic, uint8_t* payload, unsigned int length) {
	String payloadIn = (char*)payload;
			String topicIn=String(topic);

			String msg;
				for(int i=0;i<length;i++){
					msg+=(char)payload[i];
				}

			String messageIn="Message received payload="+msg+" topic="+topic +" length="+length;
}

bool MqttManager::connectMqtt() {
	if( WiFi.status() == WL_CONNECTED){
		if (!client->connected()) {
		    long now = millis();
		    if (now - lastReconnectAttempt > 5000) {
		      lastReconnectAttempt = now;

		      this->client->connect(MQTT_PUBLISHER, MQTT_USER, MQTT_PASSWORD);

		      if (this->client->connected()) {
		        lastReconnectAttempt = 0;
		        return true;
		      }
		    }
		  } else{
			  return true;
		  }
	}
	return false;
}

/*
 * MqttManager.cpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <serve/MqttManager.h>

MqttManager::MqttManager(SettingsStorage* conf) {
	Serial.print(FPSTR("constructor MqttManager"));

	this->conf = conf;
	/*this->toServerTopic = strdup((String(MQTT_FROM_DEVICE_TOPIC_TMPL) + conf->deviceId()).c_str());
	this->toDeviceTopic = strdup((String(MQTT_TO_DEVICE_TOPIC_TMPL) + conf->deviceId()).c_str());
*/
	this->toServerTopic = String(conf->deviceId()) + MQTT_SLASH_SUFFIX;
	this->toDeviceTopic = strdup((String(MQTT_TO_DEVICE_TOPIC) + conf->deviceId()).c_str());
	this->fromDeviceTopic = strdup((String(MQTT_FROM_DEVICE_TOPIC) + conf->deviceId()).c_str());

	Serial.print(FPSTR("MqttManager toDeviceTopic ="));
	Serial.print(toDeviceTopic);
	Serial.print(FPSTR(" entityTopics ="));
	Serial.print(toServerTopic);
	Serial.print(FPSTR("<Entity.name()>"));
	Serial.print(FPSTR("  fromDeviceTopic ="));
	Serial.println(fromDeviceTopic);

	this->lastReconnectAttempt = millis();
	this->lastRegisterAttempt = millis();
}

void MqttManager::init(EntityJsonRequestResponse* buf, bool registered) {
	if(buf!=nullptr){
		this->buffer = buf;
	} else {
		this->buffer = new EntityJsonRequestResponse();
	}

		this->lastReconnectAttempt = millis();
		this->lastRegisterAttempt = millis();

		IPAddress adress = IPAddress(192,168,0,101);

		this->wclient = new WiFiClient();
		Serial.print(FPSTR(" wifiClient "));
		this->client = new PubSubClient(adress, MQTT_PORT,
					[this](char* topic, uint8_t* payload,
							unsigned int length){ callback(topic, payload, length);},
							*this->wclient);

		this->initDone = true;
		this->registered = registered;

		subscribeToDeviceTopic();

		Serial.print(FPSTR("host ="));
		Serial.print(adress.toString());
		Serial.print(FPSTR(":"));
		Serial.print(MQTT_PORT);
		Serial.print(FPSTR("  serverSubscribed ="));
		Serial.println(toDeviceTopicSubscribed);

		Serial.println(FPSTR("MqttManager initialized"));
}


EntityJsonRequestResponse* MqttManager::getBuffer() {
		if(!bufferUnsent && registered){
			this->buffer->construct();
		}
		return this->buffer;
}

bool MqttManager::publishBuffer() {
	if(!this->initDone){return false;}

	if(this->registered){

		if(!toDeviceTopicSubscribed){
			subscribeToDeviceTopic();
		} else {
			if(!this->serverResponseReceived){
				sendRegistrationRequest();
			}
		}

		if(this->serverResponseReceived){
			return sendDataRequest();
		}else{
			return false;
		}
	} else {
		#ifdef MQTT_REGISTRATION_DISABLED
			return false;
		#else
			return sendRegistrationRequest();
		#endif
	}
}

bool MqttManager::subscribeToDeviceTopic(){
	this->toDeviceTopicSubscribed = subscribe(toDeviceTopic, true);
	if(this->toDeviceTopicSubscribed){
		Serial.println(FPSTR("toDeviceTopicSubscribed"));
	}
	return this->toDeviceTopicSubscribed;
}

bool MqttManager::buildRegistrationRequest() {
	if(!registrationRequestBuilt){

		JsonObject& req = getBuffer()->getRequest();

		req[ENTITY_FIELD_ID] = this->conf->deviceId();
		req[ENTITY_FIELD_IP] = conf->getCurrentIp();
		req[ENTITY_FIELD_ROOT] = URL_ROOT;
		req[ENTITY_FIELD_DATA] = URL_DATA;

		registrationRequestBuilt = true;

		JsonObjectUtil::printWithPreffix("registr req=", req);
	}
	return true;
}

bool MqttManager::sendRegistrationRequest() {
	unsigned long now = millis();
	if (now - lastRegisterAttempt < 30000){
		return false;
	}
	Serial.println(FPSTR("MQ regReq"));
	if(connectMqtt()){
		buildRegistrationRequest();
		//bool subscribed = subscribe(toDeviceTopic);
		bool published = publish((char*)MQTT_REGISTRATION_TOPIC, this->buffer->getRequest());

		lastRegisterAttempt = millis();

		return published;
	}
	return false;
}

bool MqttManager::processRegistrationResponse() {
	Serial.println(FPSTR("Server response received MQTT enabled"));
	this->registered = true;
	this->serverResponseReceived = true;
	return true;
}

bool MqttManager::sendDataRequest() {
	unsigned long start = millis();

	this->bufferUnsent = true;

	JsonObject& bufFrom = getBuffer()->getResponse();
	JsonObject& bufTo = getBuffer()->getRequest();

	bool sentResult = true;

	for (const auto& groupkvp : bufFrom) {
	  bool groupResult = true;

	  JsonObject& bufFromGroup = bufFrom.get<JsonObject>(groupkvp.key);
	  JsonObject& bufToGroup = bufTo.createNestedObject(groupkvp.key);

		  for (const auto& entitykvp : bufFromGroup) {
			  const char* key = entitykvp.key;

			  String targetTopicStr = toServerTopic + entitykvp.key;
			  char* targetTopic = (char*) targetTopicStr.c_str();

			  JsonObject& bufFromEntity = bufFromGroup.get<JsonObject>(key);
			  bool result = publish(targetTopic, bufFromEntity);

			  if(result){
				 bufFromGroup.remove(key);
			  } else {
				  JsonObject& bufToEntity = bufToGroup.createNestedObject(key);
				  bool resultByField = true;

				  for (const auto& entityFieldkvp : bufFromEntity) {
					  bufToEntity.set(entityFieldkvp.key, entityFieldkvp.value);

					  bool result2 = publish(targetTopic, bufToEntity);

					  if(result2){
						  bufFromEntity.remove(entityFieldkvp.key);
					  }

					  	  bufToEntity.remove(entityFieldkvp.key);

					  	  resultByField = resultByField && result2;
				  }

				  result = resultByField;
			  }


			  bufToGroup.remove(key);

			  sentResult = sentResult && result;
			  groupResult = groupResult && result;
		  }

		  if(groupResult){
			  bufFrom.remove(groupkvp.key);
		  }

		  bufTo.remove(groupkvp.key);
	}

	if(sentResult){
		this->bufferUnsent = true;
		this->buffer->construct();
	}
	Serial.print(FPSTR("SB pub="));
	Serial.print(sentResult);
	Serial.print(FPSTR(" "));
	DeviceUtils::printlnTimeHeap(start);
	return sentResult;
}

void MqttManager::callback(char* topic, uint8_t* payload, unsigned int length) {
	Serial.print(FPSTR("topic ="));
	Serial.print(topic);
	Serial.print(FPSTR(" message="));

	String payloadIn = (char*)payload;

	String msg;
	for(unsigned int i=0;i<length;i++){
		msg+=(char)payload[i];
	}

	Serial.println(msg);

	if(strcmp(topic, toDeviceTopic)==0){
		if(msg.indexOf(MQTT_GOOD_STATUS)>-1 || msg.indexOf(MQTT_GOOD_STATUS_STR)>-1){
			Serial.println(FPSTR("registered"));
			processRegistrationResponse();
		} else{
			Serial.println(FPSTR("Bad registration response"));
		}
	}
}

void MqttManager::loop() {
	if(!initDone){
		return;
	}
	client->loop();
}

bool MqttManager::subscribe(char* topic, bool showLog) {
	if(connectMqtt()){
		bool sub = client->subscribe(topic);

		if(!sub || showLog){
			Serial.print(FPSTR("SUB t="));
			Serial.print(topic);
			Serial.print(FPSTR(" sub="));
			Serial.print(sub);
			Serial.print(FPSTR(" "));
		}
		return sub;
	} else {
		Serial.print(FPSTR("NOT CONNECTED"));
		return false;
	}
}

bool MqttManager::publish(char* topic, JsonObject& data, bool showLog) {
	if(connectMqtt()){
		//unsigned long start = millis();
		String msg;
		data.printTo(msg);

		bool pub = client->publish(topic, msg.c_str());
		this->bufferUnsent = !pub;

		if(!pub || showLog){
			Serial.print(FPSTR("PUB t="));
			Serial.print(topic);
			Serial.print(FPSTR(" data:"));
			Serial.print(msg);
			Serial.print(FPSTR(" pub="));
			Serial.println(pub);
		}
		//Serial.println(FPSTR(" "));
		//DeviceUtils::printlnTimeHeap(start);

		return pub;
	} else {
		Serial.print(FPSTR("NOT CONNECTED"));
		return false;
	}
}

bool MqttManager::connectMqtt() {
	if( WiFi.status() == WL_CONNECTED){
		if (!client->connected()) {
		    long now = millis();
		    if (now - lastReconnectAttempt > 5000) {
		      lastReconnectAttempt = now;

		      Serial.print(FPSTR("Connect..."));
		      bool result = this->client->connect(MQTT_PUBLISHER, MQTT_USER, MQTT_PASSWORD);

		      Serial.print(FPSTR("res="));
		      Serial.println(result);

		      if (this->client->connected()) {
		        lastReconnectAttempt = 0;
		        //Serial.println(FPSTR("Client connected 1"));
		        return true;
		      }
		    }
		  } else{
			  return true;
		  }
	}
	return false;
}


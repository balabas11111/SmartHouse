package com.balabas.smarthouse.server.mqtt.subscribers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.mqtt.MqttMessageSubscribtion;
import com.balabas.smarthouse.server.entity.service.IDeviceMqService;
import com.balabas.smarthouse.server.mqtt.MessageService;

@Component
public class RegisterDeviceSubscribtion extends MqttMessageSubscribtion {

	@Autowired
	private IDeviceMqService service;
	
	public RegisterDeviceSubscribtion(){
		this.topicName = MessageService.MQTT_TOPIC_REGISTRATION;
	}
	
	@Override
	public void onMessageReceived(String topic, String message) {
		service.onRegistrationTopicMessageReceived(message);
	}

}

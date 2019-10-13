package com.balabas.smarthouse.server.mqtt.subscribers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.mqtt.MqttMessageSubscriber;
import com.balabas.smarthouse.server.mqtt.MqttServiceImpl;
import com.balabas.smarthouse.server.service.IDeviceMessageService;

@Component
public class RegisterDeviceSubscriber extends MqttMessageSubscriber {

	@Autowired
	private IDeviceMessageService service;
	
	public RegisterDeviceSubscriber(){
		this.topicName = MqttServiceImpl.MQTT_TOPIC_REGISTRATION;
	}
	
	@Override
	public boolean onMessageReceived(String topic, String message) {
		return service.registerDevice(message);
	}

}

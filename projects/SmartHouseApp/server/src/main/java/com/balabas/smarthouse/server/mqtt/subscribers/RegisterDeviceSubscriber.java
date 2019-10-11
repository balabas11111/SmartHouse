package com.balabas.smarthouse.server.mqtt.subscribers;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.mqtt.MqttMessageSubscriberAbstract;
import com.balabas.smarthouse.server.mqtt.MqttServiceImpl;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Component
public class RegisterDeviceSubscriber extends MqttMessageSubscriberAbstract {

	public RegisterDeviceSubscriber(){
		this.topicName = MqttServiceImpl.MQTT_REGISTRATION_TOPIC;
	}
	
	@Override
	public boolean onMessageReceived(String message) {
		log.info("Message received : "+message);
		return true;
	}

}

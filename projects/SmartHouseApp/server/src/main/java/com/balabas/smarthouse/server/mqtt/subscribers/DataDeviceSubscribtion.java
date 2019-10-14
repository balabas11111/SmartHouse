package com.balabas.smarthouse.server.mqtt.subscribers;

import com.balabas.smarthouse.server.mqtt.MqttMessageSubscribtion;
import com.balabas.smarthouse.server.service.IDeviceMessageService;

import lombok.extern.log4j.Log4j2;

@Log4j2
public class DataDeviceSubscribtion extends MqttMessageSubscribtion {

	@SuppressWarnings("unused")
	private IDeviceMessageService service;
	
	public DataDeviceSubscribtion(String topicName, IDeviceMessageService messageService){
		this.topicName = topicName;
		this.service = messageService;
	}
	
	@Override
	public boolean onMessageReceived(String topic, String message) {
		log.info("Data from topic " + topicName + " message: " + message);
		return true;
	}

}

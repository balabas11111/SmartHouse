package com.balabas.smarthouse.server.mqtt.subscribers;

import com.balabas.smarthouse.server.mqtt.MqttMessageSubscriber;
import com.balabas.smarthouse.server.service.IDeviceMessageService;

public class DataDeviceSubscriber extends MqttMessageSubscriber {

	private IDeviceMessageService service;
	
	public DataDeviceSubscriber(String topicName, IDeviceMessageService messageService){
		this.topicName = topicName;
		this.service = messageService;
	}
	
	@Override
	public boolean onMessageReceived(String topic, String message) {
		return service.processData(topic, message);
	}

}
